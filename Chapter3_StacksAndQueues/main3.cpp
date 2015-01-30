#include <iostream>
#include "Stack.h"
#include <array>
#include "MultiArrayStack.h"
#include "SetOfStacks.h"
#include "StackFunctions.h"
#include "DoubleStackQueue.h"
#include "AnimalShelter.h"

using namespace std;



int main(){

	/*Testing 3.7 Must debug for testing.*/
	ex::AnimalShelter aniShelter;
	aniShelter.enqueue(ex::Animal(ex::Animal::AnimalType::Dog));
	aniShelter.enqueue(ex::Animal(ex::Animal::AnimalType::Cat));
	aniShelter.enqueue(ex::Animal(ex::Animal::AnimalType::Dog));
	aniShelter.enqueue(ex::Animal(ex::Animal::AnimalType::Dog));
	aniShelter.enqueue(ex::Animal(ex::Animal::AnimalType::Cat));
	aniShelter.dequeueAny();
	aniShelter.dequeueCat();
	aniShelter.dequeueDog();
	aniShelter.dequeueAny();
	aniShelter.dequeueAny();


	/*Testing 3.6*/
	std::stack<int> stack6({ 1, 2, 4, 7, 6, 0,0,0,0,0,0,0,4, 2, 89, 4, 7, 4, 7, 0, 6, 3, 2, 4, 6 });
	std::stack<int> stack6a = ex::sortStack2(stack6);
	ex::printStack(stack6);
	cout << endl << endl;
	ex::printStack(stack6a);
	cout << endl << endl;
	ex::sortStack(stack6);
	ex::printStack(stack6);
	

	/*Testing 3.5*/
	ex::DoubleStackQueue<int> que;
	int queueSize = 10;
	for (int i = 0; i < queueSize; i++)
		que.enqueue(i);

	que.dequeue();
	que.dequeue();

	while (!que.empty()){
		int el = que.front();
		cout << el << " ";
		que.dequeue();
	}
	cout << endl;

	/*Testing 3.4*/
	std::stack<int> t3({10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }), t2, t1;
	ex::printHanoiTower(t3, t2, t1);
	ex::solveHanoiTowerRec(t3, t2, t1, t3.size());
	cout << endl << endl;
	ex::printHanoiTower(t3, t2, t1);

	/*Testing 3.3*/
	auto initList = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
	ex::SetOfStacks<int> sos(5, initList);
	int size3 = sos.size();
	sos.popAt(1); sos.popAt(2);
	sos.pop(); sos.pop();
	size3 = sos.size();
	for (int i = 0; i < size3; i++){
		sos.pop();
	}
	size3 = sos.size();
	//sos.pop(); //Uncommenting this line will cause an error, as expected.

	/*Testing 3.2*/
	ex::StackWithMin<int> swm{ 1, 2, 4, 7, 8, 5, 3, 2, 6, 8, 0, 6, 3, 4, 6, 8,9, 9, 8, 8, 6, 2, 5, 3, 7, 6, 8, 5, 87, 0, 7 };
	int size = swm.getSize();
	cout << "Elements - Mins" << endl;
	for (int i = 0; i < size; i++){
		cout << swm.top() << "-" << swm.getMins()->top() << endl;
		swm.pop();
	}

	/*Testing 3.1*/
	ex::MultiStacks<> stacks;

	std::cin.get();
}