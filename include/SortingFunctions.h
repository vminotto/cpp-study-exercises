#ifndef SORTING_FUNCTIONS_H
#define SORTING_FUNCTIONS_H

#include <cstdint>
#include <memory> 
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>
#include <type_traits>
#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/dynamic_bitset.hpp>
#include <cstdint> 
#include <fstream>

namespace ex{

	/*11.7 A circus is designing a tower routine consisting of people standing atop one another's
	shoulders. For practical and aesthetic reasons, each person must be both shorter
	and lighter than the person below him or her. Given the heights and weights of
	each person in the circus, write a method to compute the largest possible number
	of people in such a tower*/
	struct CircusPerson{
		CircusPerson(std::uint32_t height, std::uint32_t weight) :
			height(height), weight(weight){}
		CircusPerson() : 
			height(0), weight(0){}
		std::uint32_t height;
		std::uint32_t weight;

		bool operator ==(const CircusPerson &r){
			return this->weight == r.weight && this->height == r.height;
		}
	};

	/*
	[10, 11, 12, 8, 9, 15, 16, 17, 18]
	*/

	void findLongestIncreasingSequence(std::vector<CircusPerson> &perVec, std::vector<std::vector<CircusPerson>> &sol, int myIndex){
		
		if (myIndex >= perVec.size())
			return;

		bool added = false;
		for (int i = 0; i < sol.size(); ++i){
			if (sol[i].back().height < perVec[myIndex].height){
				sol[i].push_back(perVec[myIndex]);
				added = true;
			}
		}
		if (!added){
			sol.push_back(std::vector<CircusPerson>());
			sol.back().push_back(perVec.at(myIndex));
		}
		findLongestIncreasingSequence(perVec, sol, myIndex + 1);
	}

	std::vector<CircusPerson> findLongestIncreasingSequence(std::vector<CircusPerson> &perVec){
		std::vector<std::vector<CircusPerson>> sol;
		findLongestIncreasingSequence(perVec, sol, 0);

		std::vector<CircusPerson> &longest = sol.front();
		for_each(sol.begin(), sol.end(), [&](std::vector<CircusPerson> &vec) {
			if (vec.size() > longest.size())
				longest = vec;
		});

		return std::vector<CircusPerson>(longest);
	}

	std::vector<CircusPerson> findBestTower(std::vector<CircusPerson> &perVec)
	{
		/*Sort by either criteria (we chose weight)*/
		std::sort(perVec.begin(), perVec.end(), [](ex::CircusPerson &l, ex::CircusPerson &r){return l.weight < r.weight; });
		
		/*Find the longet increasing sequence by the remaining criterium (height, in this case)*/
		return findLongestIncreasingSequence(perVec);
	}
	
	template <class T> T **alloc2D(int fdLen, int sdLen){
		T **buffer = new T*[fdLen];
		for (int i = 0; i < fdLen; i++){
			buffer[i] = new T[sdLen]();
		}
		return buffer;
	}
	template <class T> void delete2D(T **&buffer, int fdLen){
		for (int i = 0; i < fdLen; i++){
			delete[] buffer[i];
		}
		delete[] buffer;
		buffer = nullptr;
	}

	/*	{ 0,  1,  2,  3,  4 }
		{ 1,  5, 10, 15, 16 } 
		{ 2, 11, 20, 22, 41 } 
		{ 3, 12, 40, 55, 56 } 
		{ 4, 30, 55, 80, 91 } 
		{ 5, 62, 63, 81, 99 }
	11.6 Given an MX N matrix in which each row and each column is sorted in ascending
	order, write a method to find an element.*/
	std::pair<int,int> searchInMatrix(int mat[][5], int val, int yStart, int yEnd, int xStart, int xEnd){
		
		if (xStart > xEnd || yStart > yEnd)
			return std::pair<int, int>(-1, -1);

		int yMid = (yStart + yEnd) / 2;
		int xMid = (xStart + xEnd) / 2;
		std::pair<int, int> res;

		if (val > mat[yMid][xMid]){
			res = searchInMatrix(mat, val, yMid + 1, yEnd, xStart, xEnd); //Search bottom
			if(res.first == -1 || res.second == -1)
				res = searchInMatrix(mat, val, yStart, yMid, xMid+1, xEnd); //Search rigth (without bottom)
		}
		else if (val < mat[yMid][xMid]){
			res = searchInMatrix(mat, val, yStart, yMid-1, xStart, xEnd); //Search top
			if (res.first == -1 || res.second == -1)
				res = searchInMatrix(mat, val, yMid, yEnd, xStart, xMid-1); //Search left (without top)
		}
		else{
			return std::pair<int, int>(yMid, xMid);
		}
		return res;
	}

	/*11.5 Given a sorted array of strings which is interspersed with empty strings, write a
	method to find the location of a given string.*/
	std::vector<std::string>::iterator findString(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, const std::string &val){
		
		if (std::distance(begin,end) <= 0)
			return std::vector<std::string>::iterator();

		size_t size = std::distance(begin, end);
		auto midL = std::next(begin, size / 2);
		auto midR = midL;

		while (*midR == "" && midR != std::prev(end)){
			++midR;
		}
		//while (*midL == "" && midL != begin){
		//	--midL;
		//}
		
		if (*midR != ""){
			if		(val > *midR)  return findString(std::next(midR), end, val); //Go right
			else if (val == *midR) return midR;
		}

		while (*midL == "" && midL != begin){
			--midL;
		}
		//while (*midR == "" && midR != std::prev(end)){
		//	++midR;
		//}
		
		if (*midL != ""){
			if		(val < *midL)  return findString(begin, midL, val); //Go left
			else if (val == *midL) return midL;
		}

		if (*midL != "" && *midR != "")
			return std::vector<std::string>::iterator();

	}

	/*11.4 Imagine you have a 20 GB file with one string per line. Explain bow you would sort
	the file.*/
	struct FileLine {

		FileLine(std::uint64_t firstCharPos, std::uint64_t stringSize/*, std::uint64_t actualChar*/) :
			firstCharPos(firstCharPos), stringSize(stringSize)/*, actualChar(actualChar)*/ {}

		/*Position of the first character in this line, in relation to the beggining of the file*/
		std::uint64_t firstCharPos;
		/*length of the line*/
		std::uint64_t stringSize;
		/*Actual character being visited*/
		//std::uint64_t actualChar;
	};
	void sortLinesInFile(std::ifstream &file, int maxDistinctChars, std::string outFileName){
		
		std::vector<std::queue<ex::FileLine, std::list<ex::FileLine>>> buckets(maxDistinctChars);
		std::vector<size_t> bucketSizes(maxDistinctChars, 0);

		char c;
		std::uint64_t lineLen;
		std::uint64_t firstCharPos;
		std::uint64_t charIndex = 0;

		bool good = true;
		while (file >> c){
			firstCharPos = file.tellg();
			--firstCharPos;
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			lineLen = file.gcount() + 1; //+1 here because we extracted the first character before
			buckets[c].push(FileLine(firstCharPos, lineLen));
		}

		/*Clear all state flags that were set to false due to the 
		while above.*/
		file.clear();

		bool done = false;		
		while (!done){
			done = true;

			for (int i = 0; i < maxDistinctChars; ++i){
				bucketSizes[i] = buckets[i].size();
			}
						
			for (int i = 0; i < maxDistinctChars; ++i){
				for (int j = 0; j < bucketSizes[i]; ++j){
					FileLine line = buckets[i].front();
					buckets[i].pop();					
					if (charIndex >= line.stringSize-1){
						buckets[i].push(line);
					}
					else{
						file.seekg(line.firstCharPos + line.stringSize - charIndex - 2) >> c;
						buckets[c].push(line);						
						done = false;						
					}					
				}
			}
			++charIndex;
		}
		std::string str;
		std::ofstream outFile(outFileName);
		for (int i = 0; i < maxDistinctChars; ++i){
			while(!buckets[i].empty()){
				FileLine line = buckets[i].front();
				buckets[i].pop();
				std::getline(file.seekg(line.firstCharPos), str);
				outFile << str << std::endl;
			}
		}

	}

	template <class Iterator> Iterator findInSortedArray(Iterator begin, Iterator end, typename std::iterator_traits<Iterator>::value_type val){
		
		if (begin == end){
			return Iterator();
		}

		Iterator mid = std::next(begin, std::distance(begin, end) / 2);

		if (val < *mid)
			return findInSortedArray(begin, mid, val);
		else if (val > *mid)
			return findInSortedArray(std::next(mid), end, val);
		else
			return mid;
	}

	/*11.3 Given a sorted array of n integers that has been rotated an unknown number of
	times, write code to find an element in the array. You may assume that the array
	was originally sorted in increasing order.
	EXAMPLE
	Input: find 5 in {15, 16, 19, 20, 25, 1, 3, 4, 5, 7, 10, 14}
	Output: 8 (the index of 5 in the array)
	
	{0,1,2,3,4,5,6,7,8}

	{7,8,0,1,2,3,4,5,6} 
	         *
	{3,4,5,6,7,8,0,1,2}
	         *
	*/
	template <class Iterator> Iterator findInRotatedArray(Iterator begin, Iterator end, typename std::iterator_traits<Iterator>::value_type val){

		if (begin == end){
			return Iterator();
		}

		Iterator mid = std::next(begin, std::distance(begin, end) / 2);
		Iterator left = begin;
		Iterator right = std::prev(end);

		if (*mid <= *right && *left >= *mid)		//Right ok, left rotated
		{ 
			if (val > *mid && val < *right)		//Go right
				return findInSortedArray(std::next(mid), end, val); //Calls normal binary search
			else if (val > *mid && val > *right)//Go left
				return findInRotatedArray(left, mid, val);
			else if (val < *mid)				//Go left
				return findInRotatedArray(left, mid, val);
			else
				return mid;
		}
		else if (*mid >= *right && *left <= *mid) //Right rotated, left ok
		{
			if (val < *mid && val < *right)		//Go right
				return findInRotatedArray(std::next(mid), end, val);
			else if (val < *mid && val > *right)//Go left
				return findInSortedArray(left, mid, val);   //Calls normal binary search
			else if (val > *mid)				//Go right
				return findInRotatedArray(std::next(mid), end, val);
			else
				return mid;
		}
	}
	template <class T> bool isBetween(T left, T mid, T right){
		return mid >= left && mid <= right;
	}

	/*11.2 Write a method to sort an array of strings so that all the anagrams are next to
	each other.
	
	Move semantics:
	http://stackoverflow.com/questions/3106110/what-is-move-semantics
	*/
	template <class StringIterator> void anagramSort(StringIterator begin, StringIterator end){
		
		static_assert(std::is_same<std::iterator_traits<StringIterator>::value_type, std::string>::value,
			"The 'anagramSort()' function only accepts iterators to std::string.");
		
	
		std::list<std::unordered_map<char, std::uint16_t>> maps;

		for (StringIterator it = begin; it != end; ++it){
			maps.push_back(ex::getCharCount(*it));
		}

		auto mapI = maps.begin();
		auto strI = begin;
		while (mapI != maps.end()){

			auto mapJ = std::next(mapI);
			auto strJ = std::next(strI);
			
			while (mapJ != maps.end()){

				if (*mapI == *mapJ){
					std::swap(*mapJ, *(++mapI));
					std::swap(*strJ, *(++strI));
				}
				++mapJ;
				++strJ;
			}
			++mapI;
			++strI;
		}
	}
	

	std::unordered_map<char, std::uint16_t> getCharCount(const std::string &str){
		std::unordered_map<char, std::uint16_t> count;
		for (char c : str){
			++count[c];
		}
		return count;
	}

	bool areAnagrams(const std::unordered_map<char, std::uint16_t> &stringA, const std::unordered_map<char, std::uint16_t> &stringB){
		return stringA == stringB;
	}


	template <class Iterator> void printRange(Iterator begin, Iterator end, std::string separator = " "){
		std::copy(begin, end, std::ostream_iterator<std::iterator_traits<Iterator>::value_type>(std::cout, separator.c_str()));
		cout << endl;
	}

	/*11.1 You are given two sorted arrays, A and B, where A has a large enough buffer at
	the end to hold B. Write a method to merge B into A in sorted order.*/
	/*
	A: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
	B: bbbbbbbbbbbbbbbbbbbbbbbbbbbbb
	*/
	template<class Iterator> void mergeBintoA(Iterator begA, Iterator endA, Iterator begB, Iterator endB){
		
		Iterator newBegA = std::next(begA, std::distance(begB, endB));
		Iterator newEndA = std::next(newBegA, std::distance(begA, endA));
			
		std::move(begA, endA, newBegA);

		Iterator i = newBegA;
		Iterator j = begB;
		Iterator it = begA;


		while (i != newEndA || j != endB){
			if (i != newEndA && *i < *j){
				*it = *i;
				++i;
			}
			else if (j != endB){
				*it = *j;
				++j;
			}
			++it;
		}
	}

	template <class T> void merge(T *inTemp, size_t leftA, size_t rightA, size_t leftB, size_t rightB, T *inOriginal){
		size_t i = leftA, j = leftB;
		for (size_t n = leftA; n <= rightB; ++n){
			if (i > rightA)						inOriginal[n] = inTemp[j++];
			else if (j > rightB)				inOriginal[n] = inTemp[i++];
			else if (inTemp[i] < inTemp[j])		inOriginal[n] = inTemp[i++];
			else								inOriginal[n] = inTemp[j++];
		}
		/*Update the temporary pointer so that the results are propagate correctly through the
		recursive calls.*/
		std::copy(inOriginal + leftA, inOriginal + rightB + 1, inTemp + leftA);
	}
	/*0 1 2 3 4 5 6 7 8*/
	template <class T> void mergeSort(T *in, size_t first, size_t last, T *inTemp){

		if (std::abs(long(last - first)) == 0)
			return;

		size_t mid = (last + first) / 2;

		/*Left sub-array*/
		mergeSort(in, first, mid, inTemp);
		/*Right sub-array*/
		mergeSort(in, mid + 1, last, inTemp);
		/*Merge sorted arrays.*/
		merge(inTemp, first, mid, mid + 1, last, in);
	}
	/*Function that actually must be called for the mergeSort to happen*/
	template <class T> void mergeSort(T *in, size_t size){
		T *inTemp = new T[size];
		std::copy(in, in + size, inTemp);
		mergeSort(in, 0, size - 1, inTemp);
		delete[] inTemp;
	}

	/*Note that 'last' is not 'end', that is, it isn't the past-the-end element*/
	template <class It1, class It2> void merge(It2 begin, It2 mid, It2 last, It1 beginOriginal, It1 lastOriginal){

		It2 i = begin;
		It2 j = std::next(mid);

		bool doneI = false;
		bool doneJ = false;

		/*Making this iterator point to the post-the-end element*/
		++lastOriginal;

		for (auto it = beginOriginal; it != lastOriginal; ++it){

			if (doneI){ *it = *j; ++j; }
			else if (doneJ){ *it = *i; ++i; }

			else if (*i < *j){
				*it = *i;
				if (!doneI && std::distance(i, mid) == 0)
					doneI = true;
				++i;
			}
			else{
				*it = *j;
				if (!doneJ && std::distance(j, last) == 0)
					doneJ = true;
				++j;
			}
		}
		std::copy(beginOriginal, lastOriginal, begin);
	}

	/*Node that last and tempLast are not end(), but in fact --end()*/
	template <class It1, class It2> void mergeSort(It1 begin, It1 last, It2 tempBegin, It2 tempLast){

		size_t dist = std::distance(begin, last);

		if (dist == 0)
			return;

		It1 mid = std::next(begin, dist / 2);
		It2 tempMid = std::next(tempBegin, dist / 2);

		/*Go to left*/
		mergeSort(begin, mid, tempBegin, tempMid);
		/*Go to right*/
		mergeSort(std::next(mid), last, std::next(tempMid), tempLast);
		/*Merge*/
		merge(tempBegin, tempMid, tempLast, begin, last);

	}
	/*O(N log(N)) time, O(N) space*/
	template <class Iterator> void mergeSort(Iterator begin, Iterator end){	
		
		using pointedType = std::iterator_traits<Iterator>::value_type;
		std::vector<pointedType> tempVec(begin, end);
		//std::vector<Iterator::value_type> tempVec(begin, end);
		ex::mergeSort(begin, --end, tempVec.begin(), --tempVec.end());
	}

	/*O(N^2) time, O(1) space*/
	template <class Iterator> void bubbleSort(Iterator begin, Iterator end){
		for (Iterator i = begin; i != end; ++i){
			for (Iterator j = begin; j != end; ++j){
				if (*j > *i)
					std::swap(*i, *j);
			}
		}
	}
	template <class Iterator> void selectionSort(Iterator begin, Iterator end){
		for (Iterator i = begin; i != end; ++i){
			Iterator j = std::min_element(i, end);
			std::swap(*i, *j);
		}
	}

	template <class Iterator> void quickSort(Iterator begin, Iterator end){
		_quickSort(begin, --end);
	}
	template <class Iterator> void _quickSort(Iterator begin, Iterator last){

		size_t dist = std::distance(begin, last);

		Iterator pivot = std::next(begin, (dist) / 2);
			
		pivot = partition(begin, last, pivot);

		if (std::distance(begin, pivot) > 1)
			_quickSort(begin, std::prev(pivot));

		if (std::distance(pivot, last) > 1)
			_quickSort(std::next(pivot), last);
	}

	template <class Iterator> void pr(Iterator b, Iterator l){
		++l;
		std::copy(b, l, std::ostream_iterator<int>(std::cout, " "));
		cout << endl;
	}
		
	template <class Iterator> Iterator partition(Iterator begin, Iterator last, Iterator pivot){

		Iterator i = begin;
		Iterator j = last;
		Iterator::value_type p = *pivot;
		int dist = std::distance(begin, last);


		while (i<=j) {
			while (*i < p){
				++i; --dist;
			}
			while (*j > p){
				--j; --dist;
			}
			if (i <= j) {
				if (i == pivot)
					pivot = j;
				else if (j == pivot)
					pivot = i;
				std::iter_swap(i, j);
				i++;
				j--;
				----dist;
				if (p != *pivot){
					int putaerror = 111;
				}
			}
		}
		return pivot;
	}

	template< typename BidirectionalIterator, typename Compare >
	void quick_sort(BidirectionalIterator first, BidirectionalIterator last, Compare cmp) {
		if (first != last) {
			BidirectionalIterator left = first;
			BidirectionalIterator right = last;
			BidirectionalIterator pivot = left++;

			while (left != right) {
				if (cmp(*left, *pivot)) {
					++left;
				}
				else {
					while ((left != right) && cmp(*pivot, *right))
						--right;
					std::iter_swap(left, right);
				}
			}

			--left;
			std::iter_swap(pivot, left);

			quick_sort(first, left, cmp);
			quick_sort(right, last, cmp);
		}
	}

	void quickSort(int arr[], int left, int right) {
		int i = left, j = right;
		int tmp;
		int pivot = arr[(left + right) / 2];

		/* partition */
		while (i <= j) {
			while (arr[i] < pivot)
				i++;
			while (arr[j] > pivot)
				j--;
			if (i <= j) {
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
				i++;
				j--;
			}
		};

		/* recursion */
		if (left < j)
			quickSort(arr, left, j);
		if (i < right)
			quickSort(arr, i, right);
	}

	template <class T> int getInd(T el, int div){
		return (el % div) / (div / 10) + 10 * std::numeric_limits<T>::is_signed;
	}

	template <class Iterator> void radixSort(Iterator begin, Iterator end)
	{
		
		using intType = std::iterator_traits<Iterator>::value_type;

		static_assert(std::numeric_limits<intType>::is_integer, "The 'radixSort' algorithm accepts only integral types.\n");

		const char numDigits = 10 + 10 * std::numeric_limits<intType>::is_signed;

		bool done = false;		
		std::vector<std::queue<intType, std::list<intType>>> buckets(numDigits);
		int ind;
		int div = 10;
		int sizes[numDigits];

		/*Transfering all elements to the buckets*/
		for(Iterator it = begin; it != end; ++it){
			ind = getInd(*it, div);
			buckets.at(ind).push(*it);
		}

		/*Ordering all elements inside the buckets*/
		while (!done){
			done = true;
			for (int i = 0; i < numDigits; ++i){
				sizes[i] = buckets[i].size();
			}

			div *= 10;
			for (int i = 0; i < numDigits; ++i)
			{
				for (int j = 0; j < sizes[i]; ++j)
				{
					int element = buckets[i].front();
					buckets[i].pop();
					ind = getInd(element, div);
					buckets[ind].push(element);

					done &= (element % div) == element;
				}
			}
		}

		/*Transfering all elements back to the original array, already ordered.*/
		int n = 0;
		Iterator it = begin;
		for (int i = 0; i < numDigits; ++i)
		{
			while (!buckets[i].empty())
			{
				*it = buckets[i].front();
				buckets[i].pop();
				++it;
			}
		}
	}
	
	template <class Iterator> void heapify(Iterator begin, Iterator end){
		
		static_assert(
			std::is_same<std::random_access_iterator_tag,
			typename std::iterator_traits<Iterator>::iterator_category>::value,
			"The mySort() function only accepts random access iterators or raw pointers to an array.\n");
		using rIterator = std::reverse_iterator<Iterator>;
		
		auto getParent = [](Iterator i, Iterator &begin) -> Iterator
		{
			--i;
			i -= std::distance(begin, i) / 2;
			return i;
		};

		for (Iterator i = begin; i != end; ++i){
			Iterator j = i;
			while (j != begin && *getParent(j, begin) < *j){
				Iterator parent = getParent(j, begin);
				std::swap(*j, *parent);
				j = parent;
			}
		}
	}

	template <class Iterator> void heapSort(Iterator begin, Iterator end){
		ex::heapify(begin, end);

		size_t size = std::distance(begin, end);
		if (size <= 0)
			return;

		//Iterator i = begin;
		Iterator last = std::prev(end);

		auto getLeft = [](Iterator &i, Iterator &b, Iterator &e) -> Iterator{
			auto dist = std::distance(b, i);
			auto size = std::distance(b, e);
			return (dist * 2 + 1) < size ? i + dist + 1 : e;
		};
		auto getRight = [](Iterator &i, Iterator &b, Iterator &e) -> Iterator{
			auto dist = std::distance(b, i);
			auto size = std::distance(b, e);
			return (dist * 2 + 2) < size ? i + dist + 2 : e;
		};
		auto hasLeft = [&](Iterator &i, Iterator &b, Iterator &e) -> bool{
			return getLeft(i,b,e) < e;
		};
		auto hasRight = [&](Iterator &i, Iterator &b, Iterator &e) -> bool{
			return getRight(i,b,e) < e;
		};		
		auto hasHigherChildren = [&](Iterator &i, Iterator &b, Iterator &e) -> bool{
			return (hasLeft(i, b, e) && *getLeft(i, b, e) > *i) || (hasRight(i, b, e) && *getRight(i, b, e) > *i);
		};

		for (int i = 0; i < size-1; ++i){
			Iterator it = begin;
			std::swap(*it, *last);			
			while (hasHigherChildren(it, begin, last)){
				
				if (!hasRight(it, begin, last) || *getLeft(it, begin, last) >= *getRight(it, begin, last)){
					std::iter_swap(it, getLeft(it, begin, last));
					it = getLeft(it, begin, last);
				}
				else{
					std::iter_swap(it, getRight(it, begin, last));
					it = getRight(it, begin, last);
				}
			}
			--last;
		}

	}
	
};

#endif