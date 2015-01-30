#include <iostream>
#include <vector>
#include <list>
#include <bitset>
#include <RandomGenerator.h>
#include <unordered_map>
#include <fstream>
#include <cstdint> 

namespace ex{

	using namespace std;

	/*18.1 Write a function that adds two numbers. You should not use + or any arithmetic
	operators.*/
	int add(int a, int b)
	{
		int i = 0;
		int result = 0;
		bool carryBit = 0;

		while (a > 0 || b > 0){
			bool aBit = int(1) & a;
			bool bBit = int(1) & b;

			b >>= 1;
			a >>= 1;

			if (aBit && bBit && carryBit){
				carryBit = 1;
				result |= 1 << i;
			}
			else if (aBit && bBit){
				carryBit = 1;
			}
			else if (aBit && carryBit){
				carryBit = 1;
			}
			else if (bBit && carryBit){
				carryBit = 1;
			}
			else if (!aBit && !bBit && !carryBit){
				carryBit = 0;
			}
			else{
				carryBit = 0;
				result |= 1 << i;
			}
			++i;
		}
		if(carryBit)
			result |= 1 << i;

		return result;
	}



	/*18.2 Write a method to shuffle a deck of cards. It must be a perfect shuffle—in other
	words, each of the 52! permutations of the deck has to be equally likely. Assume that
	you are given a random number generator which is perfect.*/
	template <class T> std::vector<T> getShuffledCopy(const std::vector<T> &in){	
		ex::IntegerGenerator rd(0, 0);
		std::vector<int> out(in.size());
		for (int i = 0; i < out.size(); ++i){
			rd.set(0, i);
			int ind = rd();
			if (i != ind)
				out[i] = out[ind]; //Move element to be overwritten to 'i', so that it does not get overwritten
			out[ind] = in[i];
		}
		return out;
	}
	template <class T> void shuffle(std::vector<T> &in){
		ex::IntegerGenerator rd(0, 0);
	
		/*We do not iterate until the first position (the first one is ignored).*/
		for (int i = in.size() -1 ; i > 0; --i){
			rd.set(0, i);
			int ind = rd();
			std::swap(in[i], in[ind]);
		}
	}

	/*18.3 Write a method to randomly generate a set of m integers from an array of size n.
	Each element must have equal probability of being chosen.*/
	std::vector<int> getShufledSection(std::vector<int> &in, int sectionLen){
		if (sectionLen > in.size())
			sectionLen = in.size();

		ex::IntegerGenerator rd(0, 0);
		std::vector<int> out(sectionLen);
		for (int i = in.size() - 1; i >= in.size() - sectionLen; --i){
			rd.set(0, i);
			int ind = rd();
			
			std::swap(in[i], in[ind]);
			out[in.size() - 1 - i] = in[i];
		}
		return out;
	}


	/*1 8.4 Write a method to count the number of 2s between 0 and n
	
													   
	 02  12  20  21  22  23  24  25  26  27  28  29  32  42  52  62  72  82  92 
	102 112 120 121 122 123 124 125 126 127 128 129 132 142 152 162 172 182 192

	92-45 = 47
	92-51 = 41
	92-52 = 40
	92-53 = 39
	
	92-71 = 21
	
	19/12 = 1;
	12/12 = 1;
	11/12 = 0;
	
	 2/12 = 0;
	 
	 12355
	*/


	int get2Count(int n){
		
		int num2s = 0;
		
		
		int lastTwoDigits = n % 100;

		if (lastTwoDigits > 29){
			num2s += 19 - ((92 - lastTwoDigits) / 10 + 1);
		}
		else if (lastTwoDigits > 19){
			num2s += lastTwoDigits - 20 + 3;
		}
		else if(lastTwoDigits > 11){
			num2s += 2;
		}
		else if (lastTwoDigits > 1){
			num2s += 1;
		}
		
		int frontDigits = n - lastTwoDigits;
		int i = 0;


		//num2s += (frontDigits / 100 - 1) * 19;

		frontDigits /= 100;
		int mult = 1;
		//12500
		int dec = 100;


		while (frontDigits > 0){
			int unit = frontDigits % 10;
			//12355
			
			num2s += unit; //wrong

			if (unit == 2)
				num2s += n % dec;
			else if(unit > 2)
				num2s += dec + n % dec;

			frontDigits /= 10;
			++i;
			mult *= 10;
			dec *= 10;
		}
		return num2s;
	}


	/*18.5 You have a large text file containing words. Given any two words, find the shortest
	distance (in terms of number of words) between them in the file. If the operation
	will be repeated many times for the same file (but different pairs of words), can you
	optimize your solution?*/
	void getFileAsMap(std::ifstream &file, std::unordered_map<std::string, std::list<std::int64_t>> &map){
		std::int64_t ind = 0;
		std::string str;
		while (file >> str){
			map[str].push_back(ind);
			++ind;
		}
	}
	std::int64_t getMinDist(std::list<std::int64_t> &list1, std::list<std::int64_t> &list2)
	{
		std::int64_t minDist = std::numeric_limits<std::int64_t>::max();
		for (std::int64_t pos1 : list1){
			for (std::int64_t pos2 : list2){
				std::int64_t dist = std::abs(pos1 - pos2);
				if (dist < minDist)
					minDist = dist;
			}
		}
		return minDist;
	}

	std::int64_t getShortestDistanceBetweenWord(std::string word1, std::string word2,
		std::unordered_map<std::string, std::list<std::int64_t>> &map)
	{
		if (word1 == word2)
			return 0;

		std::list<std::int64_t> &list1 = map[word1];
		std::list<std::int64_t> &list2 = map[word2];

		if (list1.empty() || list2.empty()){
			return -1;
		}
		else{
			return ex::getMinDist(list1, list2);
		}		
	}
	std::int64_t getShortestDistanceBetweenWord(std::string word1, std::string word2,
		std::ifstream &file)
	{
		if (word1 == word2)
			return 0;

		std::list<std::int64_t> list1, list2;
		std::int64_t ind = 0;
		std::string str;
		while (file >> str){
			if (str == word1)
				list1.push_back(ind);
			if (str == word2)
				list2.push_back(ind);
			++ind;
		}

		if (list1.empty() || list2.empty()){
			return -1;
		}
		else{
			return ex::getMinDist(list1, list2);
		}
	}


	/*18.6 Describe an algorithm to find the smallest one million numbers in one billion
	numbers. Assume that the computer memory can hold all one billion numbers*/
	template<class T> std::list<T> findNSmallestAmongK(std::vector<T> numbers, size_t N){

		size_t K = numbers.size();

		if (N > K){
			throw std::string("Error at 'findNSmallestAmongK(...)': size of numbers must be higher than 'N'.\n");
		}


		/*Using a list in this case is far more efficient than a std::vector, because we do not need to move memomry
		when inserting an element at the midle of the container.*/
		std::list<T> smallest;

		smallest.push_back(numbers.front());

		for (size_t i = 1; i < K; i++){
			T val = numbers[i];

			/*In both cases below we do not need to iterate, but simply insert the element*/
			if (val <= smallest.front()){
				smallest.push_front(val);    //Insert at the list's head, in this case
			}
			else if (val > smallest.back()){
				if (smallest.size() < N){    //Insert at the list's tail, in this case
					smallest.push_back(val);
				}
			}
			else{
				/*Find position to insert the element*/
				auto pos = std::find_if(smallest.begin(), smallest.end(), [&](T &v){
					return val < v;
				});
				/*Inserts the element at the found position*/
				smallest.insert(pos, val);
			}
			/*If list grew larger than N, we remove the last element (which will be the largest among all N in the list).*/
			if (smallest.size() > N){
				smallest.pop_back();
			}
		}
		return smallest;
	}





};