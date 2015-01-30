#include <algorithm>
#include <string>
#include <cstdint>
#include <utility>
#include <array>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "RandomGenerator.h"
#include "TreeFunctions.h"

using namespace std;

namespace ex{


	/* 17.1 Write a function to swap a number in place (that is, without temporary variables). */
	/*Although this version does not use temporary variables, it may be slower
	than the temp approach. Also, it does not work for many kinds of data structures, including
	floating point numbers.
	http://en.wikipedia.org/wiki/XOR_swap_algorithm#Reasons_for_avoidance_in_practice*/
	template <class T> void inplaceSwap(T &a, T &b){
		b ^= a;
		a ^= b;
		b ^= a;

		//Example
		//100101  a
		//110110  b

		//010011  b ^= a
		//110110  a = b ^ a
		//100101  b = b ^ a
	}

	/*17.2 Design an algorithm to figure out if someone has won a game of tic-tac-toe.*/
	class TicTacToeBoard{

	private:
		int length;
		char **board;

	public:

		TicTacToeBoard(int length = 3){
			this->length = length;

			board = new char*[length];
			for (int i = 0; i < length; ++i)
				board[i] = new char[length]();
		}
		~TicTacToeBoard(){
			for (int i = 0; i < length; ++i)
				delete[] board[i];
			delete[] board;
		}

		void reset(){
			for (int r = 0; r < length; ++r)
				for (int c = 0; c < length; ++c)
					board[r][c] = 0;
		}

		void set(int row, int col, char val){
			if (row > length || col > length){
				throw "Error at 'TicTacToeBoard::set()' row and col must be less then the board's length.\n";
			}
			else{
				board[row][col] = val > 0 ? 1 : val < 0 ? -1 : 0;
			}
		}

		/*The sign of the return type indicates which player has won
		-1 returned = player A won
		+1 returned = player B won
		0 returned = nobody has won yet

		This function not only checks if a winner exists, but also returns
		which one has won.
		*/
		char hasWinner(){

			char sum = 0;

			//Check all rows
			for (int r = 0; r < length; ++r){
				sum = 0;
				for (int c = 0; c < length; ++c){
					sum += board[r][c];
				}
				if (sum == length || sum == -length)
					return sum / length;
			}

			//Check all cols
			for (int c = 0; c < length; ++c){
				sum = 0;
				for (int r = 0; r < length; ++r){
					sum += board[r][c];
				}
				if (sum == length || sum == -length)
					return sum / length;
			}

			//Check first diagonal
			sum = 0;
			for (int r = 0, c = 0; r < length; ++c, ++r){
				sum += board[r][c];
			}
			if (sum == length || sum == -length)
				return sum / length;

			//Check second diagonal
			sum = 0;
			for (int r = 0, c = length - 1; r < length; --c, ++r){
				sum += board[r][c];
			}
			if (sum == length || sum == -length)
				return sum / length;

			//In case nothing has been returned at this point, this means no winner existes yet
			return 0;
		}
	};

	//17.3 Write an algorithm which computes the number of trailing zeros in n factorial
	/*
	What generates a trailing zero are the following two situations:

	Multiplication by 0-terminated number
	Multiplication by number containing units 2 and 5*/
	std::uint32_t getNumTrailingZerosInFactorial(std::uint32_t n){
		
		/*Number of zero terminate values and number of 2 and 5*/
		int numTrail = (n / 10) * 2;
		/*Now we sum any remaining 2 and 5 in the units*/
		numTrail += (n % 10) >= 5;

		return numTrail;
	}


	/*Write a method which finds the maximum of two numbers. You should not use
	if-else or any other comparison operator*/
	int getMax(int a, int b){
		a -= b;
		a &= (~a) >> 31;
		a += b;
		
		return a;
	}
	/*Min version*/
	int getMin(int a, int b){
		a -= b;
		a &= a >> 31;
		a += b;

		return a;
	}

	/*17.5 The Game of Master Mind is played as follows:
	The computer has four slots, and each slot will contain a ball that is red (R), yellow
	(Y), green (G) or blue (B). For example, the computer might have RGGB {Slot #1 is
	red, Slots #2 and #3 are green, Slot #4 is blue).
	You, the user, are trying to guess the solution. You might, for example, guess
	YRGB.
	When you guess the correct color for the correct slot, you get a "hit." If you guess
	a color that exists but is in the wrong slot, you get a "pseudo-hit." Note that a slot
	that is a hit can never count as a pseudo-hit.
	For example, if the actual solution is RGBY and you guess GGRR, you have one
	hit and one pseudo-hit.
	Write a method that, given a guess and a solution, returns the number of hits
	and pseudo-hits.*/
	template <size_t size> int getHits(std::array<int, size> &&guess, std::array<int, size> &&solution, int &numPseudoHits){

		std::array<int, size> hits;
		
		int numHits = 0;
		numPseudoHits = 0;

		for (int i = 0; i < size; ++i){
			hits[i] = guess[i] == solution[i];
		}

		for (int i = 0; i < size; ++i){
			if (!hits[i]){
				for (int j = 0; j < size; ++j){
					if (!hits[j]){
						numPseudoHits += guess[i] == solution[j];
					}
				}
			}
			else
				++numHits;
		}

		return numHits;
	}

	
	/*17.6 Given an array of integers, write a method to find indices m and n such that if you
	sorted elements m through n, the entire array would be sorted. Minimize n - m
	(that is, find the smallest such sequence).
	EXAMPLE
	Input: 1, 2, 4, 7, 10, 11, 7, 12, 6, 7, 16, 18, 19
	Output: (3, 9)*/
	std::pair<int, int> getRangeToSort(std::vector<int> &&vec){	
		if (vec.size() == 1)
			return std::pair<int, int>(-1, -1);
		
		int m = -1;
		int n = -1;

		int max = -std::numeric_limits<int>::max();
		int ind = -1;

		for (int i = 0; i < vec.size(); ++i){
			if (vec[i] > max){ 
				max = vec[i];
				ind = i;
			}
			else{ //when max is not the next element, we have to find where such element must fit by going backwards in the array
				
				if (m == -1 || vec[i] < vec[m]){
					
					int j;
					
					if (m == -1) j = i - 2; //If first run
					else		 j = m - 1; //If just updating m, we do not need to check the entire range [m i], only [0 m-1] backwards

					while (j >= 0 && vec[i] < vec[j]){
						--j;
					}

					m = j + 1;
					n = i;
				}
				else if(vec[i] >= vec[m]){
					n = i; //no need to touch m, just update n
				}				
			}
		}
		return std::pair<int, int>(m, n);
	}

	/*17.7 Given any integer, print an English phrase that describes the integer (e.g., "One
	Thousand, Two Hundred Thirty Four").*/

	
	std::string getUnitName(size_t n){
		switch (n){
			case 0: return "";
			case 1: return "one";
			case 2: return "two";
			case 3: return "three";
			case 4: return "four";
			case 5: return "five";
			case 6: return "six";
			case 7: return "seven";
			case 8: return "eight";
			case 9: return "nine";
		}
	}
	std::string getDozenName(size_t n){
		if		 (n < 10) return getUnitName(n);
		else if (n == 10) return "ten";
		else if (n == 11) return "eleven";
		else if (n == 12) return "twelve";
		else if (n < 20) return getUnitName(n % 10) + "teen";
		else if (n < 30) return "twenty"	+ std::string(std::string("-")) + getUnitName(n % 10);
		else if (n < 40) return "thirty"	+ std::string("-") + getUnitName(n % 10);
		else if (n < 50) return "forty"		+ std::string("-") + getUnitName(n % 10);
		else if (n < 60) return "fifty"		+ std::string("-") + getUnitName(n % 10);
		else if (n < 70) return "sixty"		+ std::string("-") + getUnitName(n % 10);
		else if (n < 80) return "seventy"	+ std::string("-") + getUnitName(n % 10);
		else if (n < 90) return "eighty"	+ std::string("-") + getUnitName(n % 10);
		else if (n <100) return "ninenty"	+ std::string("-") + getUnitName(n % 10);
	}

	std::string getSuffix(size_t n){
		if		(n < 1e3)	return "";
		else if (n < 1e6)   return "thousand";
		else if (n < 1e9)   return "million";
		else if (n < 1e12)  return "billion";
		else if (n < 1e15)  return "trillion";
		else if (n < 1e18)  return "quadrillion";
		else if (n < 1e21)  return "quintillion";
		else if (n < 1e24)  return "sextillion";
		else if (n < 1e27)  return "septillion";
	}
	std::string getNumberName(size_t n){

		std::string name("");

		size_t count = 100;
		while (n > 0){
			std::string tmpStr("");
			size_t val = n % 1000;

			if (val > 99)
				tmpStr = getUnitName(val / 100) + "-" + "hundred ";

			tmpStr += getDozenName(val % 100) + " ";

			//tmpStr += getUnitName(val % 10);
			tmpStr += getSuffix(count) + " "; //thousand, million, billion ...
			name = tmpStr + name;

			n /= 1000;
			count *= 1000;;
		}
		return name;
	}


	/*17.8 You are given an array of integers (both positive and negative). Find the contiguous
	sequence with the largest sum. Return the sum
	Input: 2, -8, 3, -2, 4, -10
	Outputs (i.e., {3, -2, 4})*/
	std::pair<int, int> getMaxSumRange(std::vector<int> &&vec){
		
		std::pair<int, int> sumRange, maxSumRange;
		int sum = 0, maxSum = std::numeric_limits<int>::lowest();

		sumRange.first = 0;

		for (int i = 0; i < vec.size(); ++i){
			sum += vec[i];
			sumRange.second = i;
			if (sum > maxSum){
				maxSum = sum;
				maxSumRange = sumRange;
			}
			else if (sum < 0){
				sum = 0;
				sumRange = { i+1, i+1 };
			}
		}
		return maxSumRange;
	}

	/*17.9 Design a method to find the frequency of occurrences of any given word in a book
	Questions to interviwer: differ capital letters?
	Will this method be called frequently? If so, we create a hash_table for the frequency ocurrence
	of all words in the book, this way, successive calls would cost O(1);*/

	/*We assume:
	all strings are ASCII encoded
	equivalent strings with different casing are considered equal
	commas and periods are not considered*/
	class OcurrenceCounter{
	
	public:
		OcurrenceCounter(std::ifstream &file){
			std::string str;
			while (file >> str){
				this->removeSpecialChars(str);
				std::transform(str.begin(), str.end(), str.begin(), ::tolower);
				wordCount[str]++;
			}
		}
		~OcurrenceCounter(){}

		size_t getCount(std::string str){
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
			auto itToElement = wordCount.find(str);

			if (itToElement == wordCount.end())
				return 0;
			else
				return itToElement->second;
		}

		void removeSpecialChars(std::string &str){
					
			auto newEnd =
			std::remove_if(str.begin(), str.end(), [](char c)
			{
				return  c == '.' ||
					c == ',' ||
					c == '!' ||
					c == '?' ||
					c == ';';
			});
			str.erase(newEnd, str.end());
		}
		
	private:

		std::unordered_map<std::string, size_t> wordCount;

	};


	/*17.10 Since XML is very verbose, you are given a way of encoding it where each
	tag gets mapped to a pre-defined integer value. The language/grammar is as
	follows:
	Element --> Tag Attributes END Children END
	Attribute --> Tag Value
	END --> 0
	Tag --> some predefined mapping to int
	Value --> string value END
	For example, the following XML might be converted into the compressed string
	below (assuming a mapping of family -> 1, person ->2, firstName ->
	3, lastName -> 4, state -> 5).
	<family lastName="McDowell" state="CA">
	<person firstName="Gayle">Some Message</person>
	</family>
	Becomes:
	1 4 McDowell 5 CA 0 2 3 Gayle 0 Some Message 0 0.
	Write code to print the encoded version of an XML element (passed in Element
	and Attribute objects).*/
	struct XmlAttribute{
		
		XmlAttribute(std::string tag, std::string attributeValue)
			: tag(tag), attributeValue(attributeValue){}

		std::string getEncodeTag(){
			if (tag == "firstName")
				return "3";
			else if (tag == "lastName")
				return "4";
			else if (tag == "state")
				return "5";
			else
				return "#"; //Character used to specify an error.
		}

		/*Tag originally used in the XML file to declare this attribute*/
		std::string tag;
		std::string attributeValue;
	};
	struct XmlElement{		

		XmlElement(std::string tag, std::string elementValue, std::vector<XmlAttribute> attributes = std::vector<XmlAttribute>())
			: tag(tag), elementValue(elementValue)
		{
			this->attributes.assign(attributes.begin(), attributes.end());
		}

		XmlElement(std::string tag, std::vector<XmlAttribute> attributes = std::vector<XmlAttribute>()) 
			: tag(tag)
		{
			this->attributes.assign(attributes.begin(), attributes.end());
		}

		XmlElement(std::string tag, std::vector<XmlElement*> &children, std::vector<XmlAttribute> attributes = std::vector<XmlAttribute>()) 
			: tag(tag)
		{
			this->attributes.assign(attributes.begin(), attributes.end());
			this->children.assign(children.begin(), children.end());
		}

		std::string getEncodeTag(){
			if (tag == "family")
				return "1";
			else if (tag == "person")
				return "2";
			else
				return "#"; //Character used to specify an error.
		}

		/*Attributes of this element*/
		std::vector<XmlAttribute> attributes;
		/*Tag originally used in the XML file to declare this element*/
		std::string tag;
		/*Value of this element*/
		std::string elementValue;

		/*Children that are nested to this element. If they exist, no elementValue
		string should exist for this element.*/
		std::vector<XmlElement*> children;
	};


	void encode(XmlAttribute &attr, std::ostream &output){
		output << attr.getEncodeTag() << " " << attr.attributeValue << " ";
	}
	void encode(XmlElement &root, std::ostream &output){
		output << root.getEncodeTag() << " ";
		for (XmlAttribute &attr : root.attributes){
			encode(attr, output);
		}
		output << "0 ";
		if (root.children.empty()){
			output << root.elementValue << " ";
		}
		else{
			for (XmlElement *&child : root.children){
				encode(*child, output);
			}
		}
		output << "0 ";			
	}

	/*17.11 Implement a method rand7() given rand5(). That is, given a method that
	generates a random number between 0 and 4 (inclusive), write a method that
	generates a random number between 0 and 6 (inclusive).*/
	int rand7()
	{
		static ex::IntegerGenerator rand5(0, 4);
		while (true) {
			int num = 5 * rand5() + rand5();
			if (num < 21) {
				return num % 7;
			}
		}
	}

	/*17.12 Design an algorithm to find all pairs of integers within an array which sum to a
	specified value.
	
	{1, 2, 3, 0, 5, 7, 4, 3, 1, 9, 10}
	
	This is the fastest possible in-place solution if the array is unsorted.
	If it is sorted, this could be done in O(n) time, also in-place.
	*/
	std::list<std::pair<int, int>> getPairsThatFormSum(std::vector<int> vec, int sum){
		
		std::list<std::pair<int, int>> pairsIndexes;

		for (int i = 0; i < vec.size(); ++i){
			if (vec[i] <= sum){
				for (int j = i + 1; j < vec.size(); ++j){
					if (vec[i] + vec[j] == sum){
						pairsIndexes.push_back({ i, j });
					}
				}
			}
		}
		return pairsIndexes;
	}

	/*17.13 Consider a simple node-like data structure called BiNode, which has pointers to two
	other nodes. The data structure BiNode could be used to represent both a binary
	tree (where nodel is the left node and node2 is the right node) or a doubly linked
	list (where nodel is the previous node and node2 is the next node). Implement a
	method to convert a binary search tree (implemented with BiNode) into a doubly
	linked list. The values should be kept in order and the operation should be performed
	in place (that is, on the original data structure).*/

	/* Turn the following...
				______________34______________
               /                              \
        _______8______                  ______56______
       /              \                /              \
    _-34__             9              50             200
   /      \             
  -50      -2           
         /
        -9   
   Into:
   [-50] <-> [-34] <-> [-9] <-> [-2] <-> [8] <-> [9] <-> [34] <-> [50] <-> [56] <-> [200]
   */
	template<class T> TreeNode<T> *getMinNode(TreeNode<T> *n1, TreeNode<T> *n2){
		if (!n1 && !n2)	return nullptr;
		else if (!n1)	return n2;
		else if (!n2)	return n1;
		else			return n1->element < n2->element ? n1 : n2;
	}
	template<class T> TreeNode<T> *getMaxNode(TreeNode<T> *n1, TreeNode<T> *n2){
		if (!n1 && !n2)	return nullptr;
		else if (!n1)	return n2;
		else if (!n2)	return n1;
		else			return n1->element >= n2->element ? n1 : n2;
	}
	template<class T> TreeNode<T> *treeToList(TreeNode<T> *root, TreeNode<T> *&head){
		
		if (!root)
			return nullptr;

		TreeNode<T> *left = treeToList(root->left, head);
		
		if (left){
			head = getMaxNode(head, left);
		}

		if (head){
			head->right = root;
			root->left = head;
			head = root;
		}

		treeToList(root->right, head);
		return root;
	}
	template<class T> void treeToList(TreeNode<T> *&root, bool returnTail = false){
		
		ex::TreeNode<int> *head = nullptr;
		ex::treeToList(root, head);
		
		if (returnTail){
			root = head; //head here will actually be tail after the above call to 'treeToList()'
		}
		else{
			/*This method returns at the tree's original root. Since this is
			a linked list now, we must go to its head.*/
			while (root->left){
				root = root->left;
			}
		}
	}

	/*17.14 Oh, no! You have just completed a lengthy document when you have an unfortunate
	Find/Replace mishap. You have accidentally removed all spaces, punctuation,
	and capitalization in the document. A sentence like "I reset the computer. It still
	didn't boot!" would become "iresetthecomputeritstilldidntboot". You figure that you
	can add back in the punctation and capitalization later, once you get the individual
	words properly separated. Most of the words will be in a dictionary, but some strings,
	like proper names, will not.
	Given a dictionary (a list of words), design an algorithm to find the optimal way of
	"unconcatenating" a sequence of words. In this case, "optimal" is defined to be the
	parsing which minimizes the number of unrecognized sequences of characters.
	For example, the string "ladyjesslookedjustliketimherbrother" would be optimally parsed
	as "lady JESS looked just like TIM her brother". This parsing has seven unrecognized characters,
	which we have capitalized for clarity.*/

	template <class StrIterator> bool mayHaveString(std::string str, StrIterator begin, StrIterator end){
		for (StrIterator it = begin; it != end; ++it){
			if (it->find(str) == 0)
				return true;
			else if (str < *it)
				return false;
		}
		return false;
	}
	void setRanges(std::list<std::string> &dictionary,
		std::array<std::pair<std::list<std::string>::iterator, std::list<std::string>::iterator>, 26> &ranges)
	{
		int i = 0;

		auto it = dictionary.begin();
		char c = it->at(0);
		ranges[c-'a'].first = it;
		++it;

		for (; it != dictionary.end(); ++it){
			if (it->at(0) > c){
				ranges[c - 'a'].second = it; //we use 'it' instead of 'std::prev(it)', because we want the post-the-end element to be the end.
				c = it->at(0);
				ranges[c - 'a'].first = it;
			}
		}
		ranges[c - 'a'].second = it; //'it == dictionary.end()' in here
	}
	std::string getUncocatenatedString(std::string &str, std::list<std::string> &dictionary){
		
		using strIter = std::list<std::string>::iterator;

		std::string newStr, tmpStr, unknownString;

		/*Make words are in alphabetic order.*/
		dictionary.sort();
		
		/*Pointers (iterators) to the longest and shortest strings (one for each letter) in the above dictionary*/
		//std::array<strIter, 26> longestWords, shortestWords;
		/*Iterators to start and end of the words begining with each alphabet letter.*/
		std::array<std::pair<strIter, strIter>, 26> ranges;
		ex::setRanges(dictionary,ranges);

		/*If true, it means we found a string, but we are looking for a larger one composed by the
		substring found.*/
		bool finding = false;
		bool tryingToFind = false;
		
		for (size_t i = 0; i < str.size(); ++i){
			
			tmpStr += str[i];
			strIter &beg = ranges[tmpStr[0] - 'a'].first;
			strIter &end = ranges[tmpStr[0] - 'a'].second;
			strIter  res = std::find(beg, end, tmpStr);
			
			if (res != end){
				if (finding){
					newStr += str[i];
				}
				else{
					if (tryingToFind)
						newStr += " ";
					newStr += tmpStr;
				}
				finding = true;
				tryingToFind = false;
				unknownString = "";
			}
			else{
				/*If transition ocurred, we add a space, and reset the temporary string*/
				if (finding){
					newStr += " ";
					tmpStr = "";
					--i;
				}
				else{
					
					unknownString += str[i];
					strIter &beg = ranges[unknownString[0] - 'a'].first;
					strIter &end = ranges[unknownString[0] - 'a'].second;
					if (!mayHaveString(unknownString, beg, end)){
						newStr += toupper(unknownString[0]);
						i -= unknownString.length()-1;
						unknownString = "";
						tmpStr = "";
						tryingToFind = true;
					}
				}
				finding = false;
			}
		}
		return newStr;
	}

	

	std::list<std::string> readFileAsStringList(std::ifstream &file, bool convertToLowerCase = true){
		std::string str;
		std::list<std::string> l;
		while (file >> str){
			if (convertToLowerCase)
				std::transform(str.begin(), str.end(), str.begin(), ::tolower);
			if (str.size() == 1){ //make sure no single consonants are added
				if (str.find_first_of("ai") != std::string::npos){
					l.push_back(str);
				}
			}
			else{
				l.push_back(str);
			}
				
		}
		return l;
	}
	std::string getUnformatedString(const std::string &str){
		std::string newStr;

		for (size_t i = 0; i < str.size(); ++i){
			int c = str[i];
			if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')){
				newStr += tolower(c);
			}
		}
		return newStr;
	}

	
};