
#include <iostream>
#include "Person.h"
#include "MemoryLimitFunctions.h"
#include "RandomGenerator.h"

using namespace std;

int main(){

	/*Testing 10.5*/
	/*Testing 4.2*/
	ex::DiGraph<int> digraph;
	std::vector<ex::GraphNode<int>*> nodes;
	nodes.push_back(new ex::GraphNode<int>());//dummy;
	for (int i = 1; i <= 9; i++)
		nodes.push_back(new ex::GraphNode<int>(i));

	/*Hihgly internally looped graph*/
	digraph.addConnection(nodes[1], nodes[2]);
	digraph.addConnection(nodes[1], nodes[3]);
	digraph.addConnection(nodes[3], nodes[1]);
	digraph.addConnection(nodes[3], nodes[2]);
	digraph.addConnection(nodes[3], nodes[4]);
	digraph.addConnection(nodes[2], nodes[8]);
	digraph.addConnection(nodes[8], nodes[2]);
	digraph.addConnection(nodes[8], nodes[4]);
	digraph.addConnection(nodes[4], nodes[5]);
	digraph.addConnection(nodes[5], nodes[6]);
	digraph.addConnection(nodes[6], nodes[4]);
	digraph.addConnection(nodes[6], nodes[7]);
	digraph.addConnection(nodes[7], nodes[8]);
	auto pages = ex::performWebCrawl(nodes[1], 0);

	//std::ofstream file;
	string str = "C:/Users/Vicente/Dropbox/Material Entrevista/Exercises/Chapter10_ScalabilityAndMemoryLimits/1million_ints.txt";
	//file.open(str);
	//ex::UIntegerGenerator rd(0, 4000);
	//for (uint32_t i = 0; i < 500000; i++){
	//	file << i << " ";
	//}
	//for (uint32_t i = max-500000; i <= max; i++){
	//	file << i << " ";
	//}
	//file.close();
	
	/*Testing 10.3*/
	//const uint32_t memoryLimit = 1024*10*8; //10kB in bits
	//uint32_t uniqueUint = ex::getAbsentPositiveInt<memoryLimit>(str);
	//cout << "An absent (non existent) integer in a 1 million-ints file (using 10kb maximum memory) is: " << uniqueUint << endl;

	/*Testing 10.4*/
	std::vector<int> vec({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 6, 3, 8, 9, 1, 3, 23, 46, 68, 70, 12, 12, 55, 6, 7, 8, 9, 55 });
	ex::printDuplicates(vec);
	cout << endl << endl;
	ex::printDuplicates<100>(vec);

	cin.get();
	return 0;
}