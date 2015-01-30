#include <iostream>
#include "ModerateQuestions.h"
#include <initializer_list>

using namespace std;

int main(){
	
	/*Testing 17.1*/
	int a = -10;
	int b = 155;
	ex::inplaceSwap(a, b);

	/*Testint 17.2*/
	ex::TicTacToeBoard board;
	board.set(0, 0, -1);
	board.set(0, 1, -1);
	board.set(0, 2, -1);
	char hasWinner = board.hasWinner();
	board.reset();

	board.set(0, 1, 1);
	board.set(1, 1, 1);
	board.set(2, 1, 1);
	hasWinner = board.hasWinner();
	board.reset();

	board.set(0, 0, -1);
	board.set(1, 1, -1);
	board.set(2, 2, -1);
	hasWinner = board.hasWinner();
	board.reset();

	/*Testing 17.3*/
	int numTrailingZeros = ex::getNumTrailingZerosInFactorial(85);
		
	/*Testing 17.4*/
	int maxV = ex::getMax(10, -212);
	int minV = ex::getMin(10, -212);

	/*Testing 17.5*/
	int numPseudoHits;
	int numHits = ex::getHits(std::array<int, 4>({ 1, 1, 2, 3 }), 
							  std::array<int, 4>({ 3, 1, 2, 1 }), numPseudoHits);


	/*Testing 17.6*/
	auto range = ex::getRangeToSort({2, 1, 4, 7, 10, 11, 7, 12, 6, 7, 16, 1, 19 });

	/*Testing 17.7*/
	std::string numberName = ex::getNumberName(123456);

	/*Testing 17.8*/
	auto range2 = ex::getMaxSumRange({ 2, -8, 3, -2, 4, -10, 1, 2, 3, -1 });

	/*Testing 17.9*/
	std::ifstream book("book.txt");
	ex::OcurrenceCounter wordCounter(book);
	int count = wordCounter.getCount("energy");
	count = wordCounter.getCount("ANY");
	count = wordCounter.getCount("speed");
	count = wordCounter.getCount("system");
	
	/*Testing 17.10*/
	ex::XmlElement child("person", "Some Message",
		std::vector<ex::XmlAttribute>({ { "firstName", "Gayle" } }));
	
	ex::XmlElement root("family", 
		std::vector<ex::XmlElement*>({ &child }),
		std::vector<ex::XmlAttribute>({ { "lastName", "McDowell" }, { "state", "CA" } }));

	ex::encode(root, std::cout);
	cout << endl << endl;

	/*Testing 17.11*/
	for (int i = 0; i < 30; ++i){
		cout << ex::rand7() << "  ";
	}
	cout << endl;

	/*Testing 17.12*/
	int sum = 5;
	std::vector<int> intVec = { 1, 2, 3, 0, 5, 7, 4, 3, 1, 9, 10 };
	auto indexes = ex::getPairsThatFormSum(intVec, sum);
	cout << "Pairs that sum to: " << sum << endl;
	for (auto pair : indexes){
		cout << intVec[pair.first] << " " << intVec[pair.second] << endl;
	}
	
	/*Testing 17.13*/
	std::vector<int> testBST1 = { 34, 56, 8, 9, -34, -2, -3, -9, -50, 200, 10};
	ex::BinaryTree<int> *tree1 = ex::createBST(testBST1.begin(), testBST1.end());
	cout << endl << "Original structure (as BST):" << endl;
	ex::printPretty(tree1->getRoot(), 0, 0, std::cout);
	cout << endl << "Desired result:" << endl;
	tree1->print(ex::BinaryTree<int>::TraversalType::DepthFirst_InOrder);
	ex::TreeNode<int> *rt = tree1->getRoot();
	ex::treeToList(rt);
	cout << endl << "Obtained result" << endl;
	while (rt){
		cout << "[" << rt->element << "] ";
		rt = rt->right;
	}

	/*Testing 17.14*/
	std::ifstream file("EngDic.txt");
	std::list<std::string> dictionary = ex::readFileAsStringList(file);
	//std::string original = "My dog is a nice animal, but Harry has a cat that is cooler!";
	std::string original = "but Harry has a cat that is cooler!";
	std::string unformatted = ex::getUnformatedString(original);
	std::string formated = ex::getUncocatenatedString(unformatted, dictionary);

	return 0;
}