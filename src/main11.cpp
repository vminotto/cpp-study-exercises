#include <iostream>
#include "SortingFunctions.h"
#include <list>
#include <deque>
#include "Heap.h"
#include "RandomGenerator.h"
#include <fstream>
#include "TreeFunctions.h"

using namespace std;

int main(){

	/*Testing 11.8*/
	std::vector<int> testBST = { 34, 56, 8, 9,-34, -2, -9, -50, 200, 50 };
	auto tree = ex::createBST(testBST.begin(), testBST.end());
	ex::printPretty(tree->getRoot(), 1, 0, std::cout);
	int smallerValues = ex::countSmallerValuesInBST(tree, 50);

	/*Testing 11.7*/
	std::vector<ex::CircusPerson> perVec = { { 160, 55 }, { 170, 50 }, { 190, 85 }, { 192, 100 }, { 180, 93 }, 
										   { 120, 30 }, { 130, 45 }, { 140, 52 }, { 180, 75 }, { 172, 69 }, 
										   { 150, 57 }, { 162, 71 }, { 195, 101}, {199,  89 }, { 189, 78 }, 
										   { 142, 55 }, { 143, 77 }, { 160, 59 }, { 155, 49 }, { 181, 81 }};
	
	std::vector<ex::CircusPerson> bestTower = ex::findBestTower(perVec);


	/*Testing 11.6*/
	int mat[6][5] = {	{ 0,  1,  2,  3,  4 },
						{ 1,  5, 10, 15, 16 }, 
						{ 2, 11, 20, 22, 41 }, 
						{ 3, 12, 40, 55, 56 }, 
						{ 4, 30, 55, 80, 91 }, 
						{ 5, 62, 63, 81, 99 }};
	std::pair<int,int> pos2D = ex::searchInMatrix(mat, 41, 0, 5, 0, 4);

	/*Testing 11.5*/
	std::vector<std::string> strVec = { "abc", "teste", "vicente", "bla", "cara", "cracha", "almofada", "zebra", "tutu", "estojo", "lavabo", "bola", "cacetete", "fita", "celular", "papel", "toblerone"};
	std::sort(strVec.begin(), strVec.end());
	auto it = strVec.begin();
	strVec.insert(strVec.begin() + 2, "");
	strVec.insert(strVec.begin() + 2, "");
	strVec.insert(strVec.begin() + 5, "");
	strVec.insert(strVec.begin() + 6, "");
	strVec.insert(strVec.begin() + 9, "");
	strVec.insert(strVec.begin() + 9, "");
	strVec.insert(strVec.begin() + 12, "");
	strVec.insert(strVec.begin() + 15, "");
	strVec.insert(strVec.begin() + 19, "");
	strVec.insert(strVec.begin() + 19, "");
	
	it = ex::findString(strVec.begin(), strVec.end(), "xaxa");

	/*Testing 11.4*/
	/*Generating strings file*/
	//ex::StringGenerator sd;
	//std::ofstream of("manyStrings.txt");
	//for (int i = 0; i < 300; i++)
	//	of << sd() << endl;
	//of.close();
	ex::sortLinesInFile(std::ifstream("manyStrings.txt"), 255, "manySortedStrings.txt");
	
	
	/*
	{7,8,0,1,2,3,4,5,6}
	         *
	{3,4,5,6,7,8,0,1,2}
	*/
	/*Testing 11.3*/

	std::vector<int> vecSorted = {0,1,2,3,5,6,8,90,92,122,125};
	auto ita = ex::findInSortedArray(vecSorted.begin(), vecSorted.end(), 10);

	std::vector<int> vecRotLeft  = { 7, 8, 0, 1, 2, 3, 4, 5, 6 };
	std::vector<int> vecRotRight = { 3, 4, 5, 6, 7, 8, 0, 1, 2 };
	auto itb = ex::findInRotatedArray(vecRotLeft.begin(), vecRotLeft.end(), 0);
	auto itc = ex::findInRotatedArray(vecRotLeft.begin(), vecRotLeft.end(), 4);
	auto itd = ex::findInRotatedArray(vecRotLeft.begin(), vecRotLeft.end(), 11);
	
	auto ite = ex::findInRotatedArray(vecRotRight.begin(), vecRotRight.end(), 0);
	auto itf = ex::findInRotatedArray(vecRotRight.begin(), vecRotRight.end(), 4);
	auto itg = ex::findInRotatedArray(vecRotRight.begin(), vecRotRight.end(), 55);

	/*Testing 11.2*/
	std::list<std::string> anagrams = { "abb", "acd", "asd", "das", "ads", "bab", "bba", "dsa", "ssd", "dsd", "cad" };
	ex::anagramSort(anagrams.begin(), anagrams.end());
	ex::printRange(anagrams.begin(), anagrams.end());

	/*Testing 11.1*/
	std::vector<int> bufferA = { -345, 345, 54, 12, 4, 7 };
	std::sort(bufferA.begin(), bufferA.end());
	
	int lenB = 12;
	for (int i = 0; i < lenB; ++i)
		bufferA.push_back(0);

	auto endA = bufferA.end() - lenB;

	std::vector<int> bufferB = { 346, 124, -7354, 32, 6, 8, 2, 6, 5, 234, 568, 34 };
	std::sort(bufferB.begin(), bufferB.end());
	ex::mergeBintoA(bufferA.begin(), endA, bufferB.begin(), bufferB.end());
	ex::printRange(bufferA.begin(), bufferA.end());

	/*Testing Heap-Sort*/
	std::vector<int> tess = { 1, 4, -1231,-346457,121,90,22,334,5566,68, 2, 51,23434,560,9,228,-78,352,1,-2342,212,7, 9, 2, 234, 5, 2 };
	ex::heapSort(tess.begin(), tess.end());
	cout << endl;
	ex::printRange(tess.begin(), tess.end());

	/*Testing ex::Heap strcuture*/
	std::initializer_list<int> common = { 1, 4, 68, 2, 57, 9, 2, 234, 5, 2 };
	ex::Heap<int> heap = common;


	/*Testing selection-sort*/
	std::list<std::int64_t> vecSelecSort = { 23, 67, 68, -0, 76, 54, 23, 67, 2, 6, 23, 6, 878, 4, 23, 65, 23, 65346, 24645, 124347, 2353542, 8, 9, -678, -456, -45, 3, -345, 345, -9898 };
	cout << endl << "Original vector:" << endl;
	ex::printRange(vecSelecSort.begin(), vecSelecSort.end());
	ex::selectionSort(vecSelecSort.begin(), vecSelecSort.end());
	cout << endl << "Sorted vector (Selection Sort)" << endl;
	ex::printRange(vecSelecSort.begin(), vecSelecSort.end());
	cout << endl;

	/*Testing bubble-sort*/
	std::list<float> listBubbleSort = { 1.12f, 4.346f, 46.34f, -123.12f, -34.44f, 123.55f, 123.f, 88.f, 23423.f, 57.f, -123.f, -123.f, -77.f, -22.f, -5643.f, 123.347f, -1343.135f };
	cout << endl << "Original list:" << endl;
	ex::printRange(listBubbleSort.begin(), listBubbleSort.end());
	ex::bubbleSort(listBubbleSort.begin(), listBubbleSort.end());
	cout << endl << "Sorted list (Bubble Sort)" << endl;
	ex::printRange(listBubbleSort.begin(), listBubbleSort.end());
	cout << endl;
	
	/*Testing radix-sort*/
	std::deque<long> dequeRadixSort = { 1, 4, 56, 67,-3444,234,23,34,685,68976,342,111,3456,569607,-23423,-32,4,-2,3-4,2-3,-23,-632, 222, 3, -6, 2, -6, 2, 3563, 23, 0, 0, 0, 7, 7, 7, 8, 78, 7, 4, 2, 5, 8, 0 };
	cout << endl << "Original deque:" << endl;
	ex::printRange(dequeRadixSort.begin(), dequeRadixSort.end());
	ex::radixSort(dequeRadixSort.begin(), dequeRadixSort.end());
	cout << "Sorted deque (Radix Sort)" << endl;
	ex::printRange(dequeRadixSort.begin(), dequeRadixSort.end());

	/*Testing merge-sort*/
	int vecMergeSort[] = { 2, 8, 5, 3, 6, 9, 7, 43, 2, 1, 4, 6, 54, 6, 2, 6, 9, 7, 56, 43, 67, 98, 1, 80, 54, 34, 6, 7, 32, 46, 5, 79, 4 };
	size_t size = sizeof(vecMergeSort) / 4;
	cout << endl << "Original Array:" << endl;
	std::copy(vecMergeSort, vecMergeSort + size, std::ostream_iterator<int>(std::cout, " "));
	ex::mergeSort(vecMergeSort, vecMergeSort + size);
	cout << endl << "Sorted array (Merge Sort)" << endl;
	std::copy(vecMergeSort, vecMergeSort + size, std::ostream_iterator<int>(std::cout, " "));
	cout << endl;
	
	cin.get();
	return 0;
}