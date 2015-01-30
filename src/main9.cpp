#include <iostream>
#include <numeric>
#include "DPFuncs.h"
#include "BinaryTree.h"
#include "TreeFunctions.h"

using namespace std;

using uint = uint32_t;
using tType = ex::BinaryTree<char>::TraversalType;



int main(){


	std::vector<std::string> cicStrs = {"ab", "ab", "ba"};
	bool hasCycle = ex::hasCicle(cicStrs);
	
	/*ab ba ab*/

	int numParenthesisPairs = 4;
	std::list<std::string> allValidParsCombs = ex::getPossibleParenthesation(numParenthesisPairs);
	cout << "All valid parenthesation for " << numParenthesisPairs << " pairs." << endl;
	for (std::string &str : allValidParsCombs){
		cout << str << endl;
	}
	cout << endl;
	
	std::string someStr = "acaa";
	auto strPermutations = ex::findAllStringPermutations(someStr);
	
	std::set<char> mainSet = { 'A', 'B', 'C', 'D' };
	std::list<std::set<char>> subSets;
	ex::findAllSubsets(mainSet, subSets);


	int i = 0;
	for (std::set<char> &subset : subSets){
		cout << "Set num " << i << ":  ";
		for (char c : subset){
			cout << c << " ";
		}
		cout << endl;
		++i;
	}

	
	//                                0   1   2  3  4  5  6   7   8   9
	std::vector<int> magicVector = { -5, -3, -1, 3, 7, 8, 9, 10, 11, 12 };
	int magicIndex = ex::findMagicIndex_NonRepeated(magicVector, 0, magicVector.size() - 1);
	magicVector[3] = 4;
	magicIndex = ex::findMagicIndex_NonRepeated(magicVector, 0, magicVector.size() - 1);

	//               0   1   2  3  4  5  6  7   8   9
	magicVector = {  0,  2,  5, 5, 7, 7, 7, 8, 11, 12 };
	magicIndex = ex::findMagicIndex_Repeated(magicVector, 0, magicVector.size() - 1);
	magicVector[0] = -1;
	magicVector[7] = 7;
	magicIndex = ex::findMagicIndex_Repeated(magicVector, 0, magicVector.size() - 1);
	magicVector[7] = 10;
	magicIndex = ex::findMagicIndex_Repeated(magicVector, 0, magicVector.size() - 1);


	/*Priority queue is max by default. That is, it uses 'std::less<T>' as a comparator. Elements
	can only be popped from the back of the queue (which seems more like a stack, but ok...). For
	this reason, to obtain a maxHeap, the 'comparator' should order elements in such a way that the
	lowest one is in the front of the container, and the max one is in the back.*/
	std::priority_queue<int> maxHeap;
	
	/*In contrast to the above comment, to obtain a minHeap, the maximum element must be in the front,
	recalling the 'stl' implementation always remove from the back of the the container. In other words,
	the minimum one must be at the end.*/
	std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
	
	std::vector<int> someValues = { 1, 3, 7, 7, 2, 5, 8, -12, 6, -5, -1, -47, -12, 645, 1, 7, 9, 2, 7, 5 };
	for (int i : someValues){
		maxHeap.push(i);
		minHeap.push(i);
	}
	cout << "Max heap order :" << endl;
	while (!maxHeap.empty()){
		cout << maxHeap.top() << " ";
		maxHeap.pop();
	}

	cout << endl << "Min heap order :" << endl;
	while (!minHeap.empty()){
		cout << minHeap.top() << " ";
		minHeap.pop();
	}

	/*ex::BinaryTree<char> treePre({ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u' },
	tType::DepthFirst_PostOrder);
	printPretty(treePre.getRoot(), 1, 0, std::cout);*/
	std::vector<int> coins = { 1, 3, 1,1,3,6,8,9,3,3,5,2,45,1,46,4,1,4,7,234,8,3,5,8,2,4,7,4};

	std::function<bool(std::pair<int, int>, std::pair<int, int>)> pairComp = 
		[](std::pair<int, int> &a, std::pair<int, int> &b) -> bool {
		if (a.first < b.first)
			return true;
		else if (a.first > b.first)
			return false;
		else{
			if (a.second < b.second)
				return true;
			else //f(x,x) must be false
				return false;
		}
	};

	std::map<std::pair<int, int>, std::uint64_t, decltype(pairComp)> prevResults(pairComp);

	std::uint64_t myBestSum = ex::my_coin_sum_dp(coins, 0, coins.size() - 1, prevResults);
	std::uint64_t opBestSum = std::accumulate(coins.begin(), coins.end(), 0) - myBestSum;
	std::uint64_t sumCheck = myBestSum + opBestSum - std::accumulate(coins.begin(), coins.end(), 0);

	/*Testing 9.9*/
	uint32_t numWays9 = ex::placeQueensChessBoard(int2(5, 5));

	/*Testing 9.8*/
	int totalValueInCents = 125;
	int highestCoin = 25;
	int numWays8 = ex::makeChange(totalValueInCents, highestCoin);

	/*Testing 9.7*/
	

	/*Testing 9.6*/
	auto parens = ex::getParensCombination(5);
	for (auto &str : parens){
		cout << str << endl;
	}

	/*Testing 9.5*/
	std::string str5("abc");
	auto subStrs = ex::getAllPermutations(str5);
	for (auto &str : subStrs){
		cout << str << endl;
	}

	/*Testing 9.4*/
	std::set<int> set = { 1, 2, 3, 4 };
	auto subsets = ex::getSubsets(set);
	ex::printAllSubSets(subsets);

	/*Testing 9.3*/
 	              /* 0  1  2  3  4  5  6   7   8   9  10  11  12  13  14*/
	int vals1[] = { -1, 0, 2, 4, 6, 7, 8, 10	, 11, 13, 14, 15, 16, 17, 18 };
	int magicIndex1 = ex::findMagicIndex_DistinctElements(vals1, 0, 14);
	              /* 0  1  2  3  4  5  6   7   8    9  10  11  12  13  14*/
	int vals2[] = { -2, 0, 1, 4, 6, 7, 8,  9,  9,  14, 14, 14, 14, 14, 14 };
	int magicIndex2 = ex::findMagicIndex(vals2, 0, 14);

	/*Testing 9.2*/
	uint x0 = 0, y0 = 0, xTarget = 1, yTarget = 1;
	cout << "Num way to get to (" << xTarget << ", " << yTarget << ") starting at (" << x0 << ", " << y0 << "): " << ex::pathXY(x0, y0, xTarget, yTarget) << endl;
	x0 = 0, y0 = 0, xTarget = 12, yTarget = 12;
	cout << "Num way to get to (" << xTarget << ", " << yTarget << ") starting at (" << x0 << ", " << y0 << "): " << ex::pathXY(x0, y0, xTarget, yTarget) << endl;
	cout << "Num way to get to (" << xTarget << ", " << yTarget << ") starting at (" << x0 << ", " << y0 << "): " << ex::pathXY_dp(x0, y0, xTarget, yTarget) << endl;

	xTarget = 5, yTarget = 5;
	int2 p0(x0, y0), pEnd(xTarget, yTarget);
	std::unordered_map<int2, bool> availability, cache;
	for (int x = 0; x <= xTarget; ++x){
		for (int y = 0; y <= yTarget; ++y){
			availability[{x, y}] = true;
		}
	}

	availability[{2, 0}] = false;
	availability[{2, 1}] = false;
	availability[{3, 2}] = false;

	std::list<int2> path;
	ex::pathXY(p0, pEnd, availability, cache, path);
	ex::drawPath(pEnd, path, availability);

	/*Testing 9.1*/
	int numStairs = 25;
	cout << "Num way to climb " << numStairs << " stairs (without DP): " << ex::stairs(numStairs) << endl;
	cout << "Num way to climb " << numStairs << " stairs (with    DP): " << ex::stairs_dp(numStairs) << endl;

	cin.get();
	return 0;
}