#ifndef REC_DP_FUNCS_H
#define REC_DP_FUNCS_H

#include <cstdint>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <stack>
#include <set>
#include <sstream>
#include <cstdint>
#include <map>
#include <functional>
#include <vector>

struct int2{
	int2() :x(0), y(0){}
	int2(int x, int y) :x(x), y(y){}
	int x, y;
	friend bool operator ==(int2 l, int2 r){ return l.x == r.x && l.y == r.y; }
};

namespace std
{
	template <> struct hash<int2>
	{
		/*This class must be empty, therefore the below commented declaration is not valid*/
		//const std::hash<int> intHasher;
		size_t operator()(const int2 &n){
			return ((std::hash<int>()(n.x) << 1) ^ std::hash<int>()(n.y)) >> 1;
		}
	};
};
using namespace std;

namespace ex{

	
	void getPossibleParenthesation(std::string str, int numPairs, int leftRemaining, int rightRemaining,
		std::list <std::string> &parenthesisCombination){
		
		if (rightRemaining == 0 && leftRemaining == 0){
			parenthesisCombination.push_back(str);
			return;
		}
		if (leftRemaining > 0){
			getPossibleParenthesation(str + "(", numPairs, leftRemaining - 1, rightRemaining, parenthesisCombination);
		}
		if (rightRemaining > leftRemaining){
			getPossibleParenthesation(str + ")", numPairs, leftRemaining, rightRemaining - 1, parenthesisCombination);
		}
	}
	std::list <std::string> getPossibleParenthesation(int numPairs){
		std::list <std::string> allValidParCombs;
		ex::getPossibleParenthesation("", numPairs, numPairs, numPairs, allValidParCombs);
		return allValidParCombs;
	}

	std::list<std::string> findAllStringPermutations(std::string &str){
		
		std::list<std::string> permutations;
		permutations.push_back(std::string("") + str.front());

		for (int i = 1; i < str.size(); ++i){
			char c = str[i];
			int numPerms = permutations.size();
			for (int j = 0; j < numPerms; ++j){
				std::string permStr = permutations.front();
				permutations.pop_front();
				for (int pos = 0; pos < permStr.size() + 1; ++pos){ /*iterate +1 to be able to insert after last character*/
					std::string tmp = permStr;
					tmp.insert(pos, 1, c);
					if (std::find(permutations.begin(), permutations.end(), tmp) == permutations.end()){
						permutations.push_back(tmp);
					}
				}
			}			
		}
		return permutations;
	}

	void findAllSubsets(std::set<char> mainSet, std::list<std::set<char>> &subsets){
		
		if (std::find(subsets.begin(), subsets.end(), mainSet) != subsets.end())
			return;
		
		subsets.push_back(mainSet);

		for (char c : mainSet){
			std::set<char> subset(mainSet.begin(), mainSet.end());
			subset.erase(c);
			findAllSubsets(subset, subsets);
		}
	}

	/*Pots of gold game: Two players A & B. There are pots of gold arranged in a line, each containing 
	some gold coins (the players can see how many coins are there in each gold pot - perfect information). 
	They get alternating turns in which the player can pick a pot from one of the ends of the line. The 
	winner is the player which has a higher number of coins at the end. The objective is to "maximize" 
	the number of coins collected by A, assuming B also plays optimally. A starts the game. 

	The idea is to find an optimal strategy that makes A win knowing that B is playing optimally as well. 
	How would you do that?*/
	std::uint64_t my_coin_sum_dp(std::vector<int> &coins, int s, int e, std::map<std::pair<int, int>, std::uint64_t, std::function<bool(std::pair<int, int>, std::pair<int, int>)>> &prevResults){

		if (s > e)
			return 0;
		if (s == e)
			return coins[s];

		std::pair<int, int> p(s, e );

		if (prevResults.find(p) == prevResults.end()){

			/*We use min here because this refers to our next play. Since the oponent plays with optmality, he picks
			the maximum solution, leaving us with the minium of the two options (in each case). This does not mean we
			do not try to play optimally as well. The max in the return statemetn indicate we try to maximise what is
			given left to us.*/
			std::uint64_t begPick = coins[s] + std::min(my_coin_sum_dp(coins, s + 1, e - 1, prevResults), my_coin_sum_dp(coins, s + 2, e, prevResults));
			std::uint64_t endPick = coins[e] + std::min(my_coin_sum_dp(coins, s + 1, e - 1, prevResults), my_coin_sum_dp(coins, s, e - 2, prevResults));

			prevResults[p] = std::max(begPick, endPick);
		}
		return prevResults[p];
	}

	std::uint64_t my_coin_sum_rec(std::vector<int> &coins, int s, int e ){

		if (s > e)
			return 0;
		if (s == e)
			return coins[s];

		/*We use min here because this refers to our next play. Since the oponent plays with optmality, he picks
		the maximum solution, leaving us with the minium of the two options (in each case). This does not mean we
		do not try to play optimally as well. The max in the return statemetn indicate we try to maximise what is
		given left to us.*/
		std::uint64_t begPick = coins[s] + std::min(my_coin_sum_rec(coins, s + 1, e - 1), my_coin_sum_rec(coins, s + 2, e));
		std::uint64_t endPick = coins[e] + std::min(my_coin_sum_rec(coins, s + 1, e - 1), my_coin_sum_rec(coins, s, e - 2));

		return std::max(begPick, endPick);
	}
	

	unsigned int iter_factorial(unsigned int n)
	{
		unsigned int ret = 1;
		for (unsigned int i = 1; i <= n; ++i)
			ret *= i;
		return ret;
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
		delete [] buffer;
		buffer = nullptr;
	}

	bool hasCicle(std::vector<string> &strs){

		int len = strs.size();

		//std::unordered_map<char, int> count;
		std::vector<int> count(len, 0);

		for (int i = 0; i < len; ++i){
			std::string str = strs[i];
			for (int j = 0; j < len; ++j){
				if (j != i){
					if (str.back() == strs[j].front()){
						count[i]++;
					}
				}
			}
		}

		for (int i : count){
			if (i == 0){
				return false;
			}
		}

		return true;
	}

	
	/*9.9 Write an algorithm to prim all ways of arranging eight queens on an 8x8 chess
	board so that none of them share the same row, column or diagonal. In this case,
	"diagonal" means all diagonals, not just the two that bisect the board.*/
	
	bool isValidQueenPos(const int2 &desiredPos, std::list<int2> & posList){
		for (int2 &existingPos : posList){
			if (desiredPos.x == existingPos.x) // same row
				return false;
			else if (desiredPos.y == existingPos.y) // same column
				return false;
			else if ((desiredPos.x - existingPos.x) == (desiredPos.y - existingPos.y)) // same diagonal
				return false;
		}
		return true;
	}

	uint32_t placeQueensChessBoard(std::list<int2> &prevPos, int row, int2 dim){
		
		/*if (row >= dim.y)
			return 1;*/

		uint32_t ways = 1;
		for (int x = 0; x < dim.x; ++x){
			
			std::list<int2> tempList(prevPos.begin(), prevPos.end());
			int2 p(x, row + 1);
			
			if (isValidQueenPos(p, tempList))
			{
				tempList.push_back(p);
				ways += ex::placeQueensChessBoard(tempList, row + 1, dim);
			}
		}
		return ways;
	}
	uint32_t placeQueensChessBoard(int2 dim){
		if (dim.x != dim.y) return 0;
		if (dim.x <= 3 || dim.y <= 3) return 0;

		std::list<int2> prevPos;
		return placeQueensChessBoard(prevPos, -1, dim);
	}


	/*9.8 Given an infinite number of quarters (25 cents), dimes (10 cents), nickels (5 cents)
	and pennies (1 cent), write code to calculate the number of ways of representing n
	cents.*/
	int makeChange(int n, int denom){
		int next_denom = 0;
		switch (denom) {
		case 25:
			next_denom = 10;
			break;
		case 10:
			next_denom = 5;
			break;
		case 5:
			next_denom = 1;
			break;
		case 1:
			return 1;
		}
		int ways = 0;
		for (int i = 0; i * denom <= n; i++) {
			ways += makeChange(n - i * denom, next_denom);
		}
		return ways;
	}

	/*9.7 Implement the "paint fill" function that one might see on many image editing
	programs. That is, given a screen (represented by a two-dimensional array of colors),
	a point, and a new color, fill in the surrounding area until the color changes from the
	original color.*/
	void paintAndFill(uint8_t **image, bool **painted, int2 p, int2 dimensions, uint8_t originalColor, uint8_t newColor){
		
		uint8_t &pixel = image[p.y][p.x];

		if (pixel != originalColor || painted[p.y][p.x] ||
			p.x >= dimensions.x || p.y >= dimensions.y ||
			p.x < 0 || p.y < 0)
		{
			return;
		}
		else
		{
			pixel = newColor;
			painted[p.x][p.y] = true;
			paintAndFill(image, painted, int2(p.x+1, p.y), dimensions, originalColor, newColor);
			paintAndFill(image, painted, int2(p.x-1, p.y), dimensions, originalColor, newColor);
			paintAndFill(image, painted, int2(p.x, p.y+1), dimensions, originalColor, newColor);
			paintAndFill(image, painted, int2(p.x, p.y-1), dimensions, originalColor, newColor);
		}
	}
	void paintAndFill(uint8_t **image, int2 p, int2 dimensions, uint8_t newColor){
		if (p.x >= dimensions.x || p.y >= dimensions.y || p.x < 0 || p.y < 0)
			return;
		uint8_t originalColor = image[p.y][p.x];
		bool **painted = ex::alloc2D<bool>(dimensions.y, dimensions.x);
		ex::paintAndFill(image, painted, int2(p.x, p.y), dimensions, originalColor, newColor);
		ex::delete2D(painted, dimensions.y);
	}

	/*9.6 Implement an algorithm to print all valid (i.e., properly opened and closed) combinations
	of n-pairs of parentheses.
	Example: n = 3
	()()()	()(())	(())()	((()))	(()())	*/
	void getAllParenthesisCombinations(string base, int numLefts, int numRights, int numPairs, std::set<std::string> &set){
		
		if (numRights == numPairs && numLefts == numPairs){
			set.insert(base);
			return;
		}

		string actual = base;
		for (int i = 0; i < numPairs-numLefts; i++){
			actual += "(";
			getAllParenthesisCombinations(actual, numLefts + i + 1, numRights, numPairs, set);
		}
		actual = base;
		for (int i = 0; i < numLefts-numRights; i++){
			actual += ")";
			getAllParenthesisCombinations(actual, numLefts, numRights + i + 1, numPairs, set);
		}
	}
	std::set<std::string> getParensCombination(int numPairs){
		std::set<std::string> set;
		getAllParenthesisCombinations("", 0, 0, numPairs, set);
		return set;
	}

	/*9.5 Write a method to compute all permutations of a string*/
	void getAllPermutations(std::string &prefix, std::string &str, std::set<std::string> &subStrs){
		
			
		if (str.size() == 0)
			return;
				
		subStrs.insert(prefix + str);
		
		for (size_t i = 0; i < str.size(); i++){
			std::string tempPrefix	= prefix + str.at(i);
			std::string tempStr		= std::string(str).erase(i, 1);
			getAllPermutations(tempPrefix, tempStr, subStrs);
		}		
	}

	std::set<std::string> getAllPermutations(std::string str){
		std::set<std::string> subStrs;
		if (str.size() > 0){
			std::string emptyStr;
			ex::getAllPermutations(emptyStr, str, subStrs);
		}
		return subStrs;
	}

	/*9.4 Write a method to return all subsets of a set.*/
	template <class T> void getSubsets(std::set<T> &set, std::set<std::set<T>> &subsets){
	
		if (set.size() == 0)
			return;

		auto insertResult = subsets.insert(set);
		/*Means 'set' was not inserted into 'subsets' because it already exists, therefore, we stop the branching*/
		if (!insertResult.second) 
			return;

		for (auto val : set){
			std::set<T> tempSet = set;
			tempSet.erase(val);
			getSubsets(tempSet, subsets);
		}		
	}

	template <class T> std::set<std::set<T>> getSubsets(std::set<T> &set)
	{
		std::set<std::set<T>> subsets;
		getSubsets(set, subsets);
		return subsets;
	}
	template <class T> void printAllSubSets(std::set<std::set<T>> &subsets){

		for (auto &set : subsets){
			cout << "(";
			int i = 0;
			for (auto &val : set){
				if (i < set.size()-1)  cout << val << ",";
				else				   cout << val;
				++i;
			}
			cout << ")" << endl;
		}
	}

	/*9.2 A magic index in an array A[l.. .n-l] is defined to be an index such that A[i] = 	i. 
	Given a sorted array of distinct integers, write a method to find a magic index, if
	one exists, in array A.
	FOLLOW UP
	What if the values are not distinct?*/

	/* -5 -3 -1  3  7  7  7 10 11 12
	    0  1  2  3  4  5  6  7  8  9*/
	int findMagicIndex_Repeated(std::vector<int> &vec, int start, int end){

		/*Means the magic index was not found, so we return -1, which is an invalid index*/
		if (start > end){
			return -1;
		}
		int mid = (start + end) / 2;
		int midValue = vec[mid];
		int magicIndex = -1;

		if (midValue > mid){			
			/*Search part of right. We decide to go right first, because it is 
			the smaller portion to look at, compared to the left side.*/
			if (midValue <= end)
				magicIndex = findMagicIndex_Repeated(vec, midValue, end);
			
			/*Search entire left, in case magic index was not found.*/
			if (magicIndex == -1)
				magicIndex = findMagicIndex_Repeated(vec, start, mid - 1);
		}
		else if (midValue < mid){
			/*Search part of left. We decide to go left first, because it is
			the smaller portion to look at, compared to the right side.*/
			if (midValue >= start)
				magicIndex = findMagicIndex_Repeated(vec, start, midValue);

			/*Search entire right, in case magic index was not found.*/
			if (magicIndex == -1)
				magicIndex = findMagicIndex_Repeated(vec, mid+1, end);
		}
		else{
			magicIndex = mid;
		}
		return magicIndex;
	}


 	/* -5 -3 -1  3  7  8  9 10 11 12
		0  1  2  3  4  5  6  7  8  9*/
	int findMagicIndex_NonRepeated(std::vector<int> &vec, int start, int end){
		
		/*Means the magic index was not found, so we return -1, which is an invalid index*/
		if (start > end){
			return -1;
		}

		int mid = (start + end) / 2;

		if (vec[mid] > mid){//go left
			return findMagicIndex_NonRepeated(vec, start, mid - 1);
		}
		else if (vec[mid] < mid){//go right
			return findMagicIndex_NonRepeated(vec, mid+1, end);
		}
		else 
			return mid;
	}

	/*This version works for sorted arrays with no repeated elements, that is,
	all distinct elements*/
	int findMagicIndex_DistinctElements(int *vec, int start, int end){
		
		int mid = (start + end) / 2;

		if (start > end || end < start)
			return -1;

		if (mid == vec[mid])
			return mid;
		else if (vec[mid] < mid)
			return findMagicIndex_DistinctElements(vec, mid + 1, end);
		else if (vec[mid] > mid)
			return findMagicIndex_DistinctElements(vec, 0, mid - 1);
	}
	/*Any array of integers that is sorted*/
	int findMagicIndex(int *vec, int start, int end, std::stack<std::pair<int, int>> &cache){

		int mid = (start + end) / 2;

		if (start > end || end < start)
			return -1;

		int result;
		
		if (mid == vec[mid]){
			result = mid;
		}
		else if (vec[mid] < mid){
			cache.push({ 0, mid - 1 });
			result = findMagicIndex(vec, mid + 1, end, cache);			
		}
		else if (vec[mid] > mid){
			cache.push({ mid + 1, end });
			result = findMagicIndex(vec, 0, mid - 1, cache);			
		}
		if (result == -1 && !cache.empty()){
			int l = cache.top().first;
			int r = cache.top().second;
			cache.pop();
			result = findMagicIndex(vec, l, r, cache);
		}
		return result;
	}

	int findMagicIndex(int *vec, int start, int end){
		if (start > end || end < start)
			return -1;

		int midInd = (start + end) / 2;
		int midVal = vec[midInd];
		if (midInd == midVal)
			return midInd;

		int result;

		/*Search to the left of midIndex*/
		int leftRightBound = std::min(midVal, midInd - 1);
		result = findMagicIndex(vec, 0, leftRightBound);
		if (result != -1)
			return result;
		
		/*Search to the right of midIndex*/
		int rightLeftBound = std::max(midVal, midInd + 1);
		result = findMagicIndex(vec, rightLeftBound, end);
		if (result != -1)
			return result;

		return result;
	}

	/*Imagine a robot sitting on the upper left comer of an X by Y grid. The robot can only
	move in two directions: right and down. How many possible paths are there for the
	robot to go from (0, 0) to (X, Y) ?
	FOLLOW UP
	Imagine certain spots are "off limits," such that the robot cannot step on them.
	Design an algorithm to find a path for the robot from the top left to the bottom right.

	We will assume X grows towards the right and Y grows towards the bottom*/
	uint64_t pathXY(uint32_t x0, uint32_t y0, uint32_t xTarget, uint32_t yTarget){
		
		if (x0 > xTarget || y0 > yTarget)
			return 0;
		else if (x0 == xTarget && y0 == yTarget)
			return 1;

		return pathXY(x0 + 1, y0, xTarget, yTarget) + pathXY(x0, y0+1, xTarget, yTarget);
	}
	uint64_t pathXY_dp(uint32_t x0, uint32_t y0, uint32_t xTarget, uint32_t yTarget, uint64_t **buffer){

		if (x0 > xTarget || y0 > yTarget)
			return 0;
		else if (x0 == xTarget && y0 == yTarget)
			return 1;

		if (!buffer[x0][y0])
			buffer[x0][y0] = pathXY_dp(x0 + 1, y0, xTarget, yTarget, buffer) + pathXY_dp(x0, y0 + 1, xTarget, yTarget, buffer);
		return buffer[x0][y0];
	}
	uint64_t pathXY_dp(uint32_t x0, uint32_t y0, uint32_t xTarget, uint32_t yTarget){
		uint64_t **buffer = alloc2D<uint64_t>(xTarget+1, yTarget+1);
		uint64_t result = pathXY_dp(x0, y0, xTarget, yTarget, buffer);
		
		return result;
	}
	bool pathXY(int2 p0, int2 pEnd, std::unordered_map<int2, bool> &availability, std::unordered_map<int2, bool> &cache, std::list<int2> &path)
	{

		if (cache.find(p0) != cache.end()){
			return cache[p0];
		}

		if (p0.x > pEnd.x || p0.y > pEnd.y)
			return false;
		else if (p0.x == pEnd.x && p0.y == pEnd.y)
			return true;

		bool success = false;

		int2 right(p0.x + 1, p0.y);
		if (!success && availability[right])
			success = pathXY(right, pEnd, availability, cache, path);
		
		int2 down(p0.x, p0.y +1);
		if (!success && availability[down])
			success = pathXY(int2(p0.x, p0.y + 1), pEnd, availability, cache, path);

		if (success){
			path.push_front(p0);	
		}
		cache[p0] = success;

		return success;
	}

	void drawPath(int2 pEnd, std::list<int2> &path, std::unordered_map<int2, bool> &availability){
		for (int y = 0; y <= pEnd.y; ++y){
			for (int x = 0; x <= pEnd.x; ++x){
				if (std::find(path.begin(), path.end(), int2(x, y)) != path.end())
					cout << "o ";
				else
					cout << "x ";
			}
			cout << endl;
		}
	}


	/* 
					  x-2
					  /
				   x-1
				  /   \
				 /     x-3
fibonacci(x) = 	x      
				 \     x-3
				  \   /
				   x-2
					  \
					   x-4
	this is O(2^N) time and O(1) space
	*/
	uint64_t fibonacci(uint64_t k){
		if (k == 0)return 0;
		if (k == 1)return 1;
		return fibonacci(k - 1) + fibonacci(k - 2);
	}

	/*
	                       x-2
	                      /
	                   x-1
	                  /   \
	                 /     x-3
ffibonacci(x) = 	x
	                 \     *
	                  \   /
	                    *
	                      \
	                       *
	this is O(N) time and O(N) space
	*/
	uint64_t ffibonacci(uint64_t k, uint64_t *buffer){
		
		if (k == 0)return 0;
		if (k == 1)return 1;
		if (buffer[k] != 0) return buffer[k];
		buffer[k] = ffibonacci(k - 1, buffer) + ffibonacci(k - 2, buffer);
		return buffer[k];
	}
	uint64_t ffibonacci(uint64_t k){
		uint64_t *buffer = new uint64_t[k+1]();
		uint64_t result = ffibonacci(k, buffer);
		delete[] buffer;
		return result;
	}


	/*9.1 A child is running up a staircase with n steps, and can hop either 1 step, 2 steps,
	or 3 steps at a time. Implement a method to count how many possible ways the
	child can run up the stairs.*/
	int64_t stairs(int32_t n){
		
		if (n <  0) return 0;
		if (n == 0) return 1;
		return stairs(n - 1) + stairs(n - 2) + stairs(n - 3);
	}
	/*Same function as above, but with dynamic programming*/
	int64_t stairs_dp(int32_t n, int64_t *buffer){
		if (n < 0) return 0;
		if (n <= 1) return 1;
		if (!buffer[n])
			buffer[n] = stairs_dp(n - 1, buffer) + stairs_dp(n - 2, buffer) + stairs_dp(n - 3, buffer);
		return buffer[n];
	}
	/*Proxy function to call the above mone*/
	int64_t stairs_dp(int32_t n){
		/*The () allocates all values to 0 (simillar to C calloc())*/
		int64_t *buffer = new int64_t[n + 1]();
		int64_t result = stairs_dp(n, buffer);
		delete[] buffer;
		return result;
	}

};

#endif