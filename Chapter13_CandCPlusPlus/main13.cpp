#include <iostream>
#include <vector>
#include "RandomGenerator.h"
#include "CPPFunctions.h"

using namespace std;

int main(){

	/*Checking mem alignment stuff.*/
	for (int i = 70; i < 80; ++i){
		for (int j = 0; j < 7; ++j){
			int a = i + j;
			cout << a << "("<<a%8<<")" << " ";
		}
		cout << endl;
	}

	bool asdf = 0 == nullptr;

	/*Testing 13.1*/
	ifstream file("strings.txt");
	ex::printLastKLines(file, 10);

	/*13.2 Compare and contrast a hash table and an STL map. How is a hash table implemented?
	If the number of inputs is small, which data structure options can be
	used instead of a hash table?
	
	As in a hash table, the std::map also maps from a key to a value. One difference is that 
	the std::map orders the elements inside the container, that is, it does not use a tree
	internally for storing the <key, element> pairs. The std::map is preferable in the case
	the data is preferably stored sorted. Furthermore, it uses the comparator < to detect
	collisions, instead of == (as in the hash table). Therefore, in a std::map, A is equivalent to B
	if, only if, !(A < B) && !(A > B). In a hash table, a hash function is used to index the elements,
	and in case of collision, the == operator is used for desambiguation. In a std::map, no hash function
	is needed. For an STL equivalent to the hash_table, there is a class std::unordered_map.
	
	Furthermore, the hash_table (std::unordered_map) has an average lookup time (element finding) constant.
	This owes to the hash function generating the index of the array directly (recalling the underlying structure
	of a hash_table is a random-accessible container). It may be more than constant due to possible need of disambi-
	guation in cases a colision happens. Also, generating the hash key is not instant.
	
	Searching in a std::map is costlier because the underlying structure (tree) must be travesered as in a binary
	search tree until the element is found. This is done using the above mentioned comparators.*/

	/*13.3 How do virtual functions work in C++?
	
	Virtual functions are used to achieve polymorphism. If a function in a base class is declared virtual,
	all its versions in subclasses will also be virtual, even if those subversions don't use the virtual keyword.
	When a virtual function is called, it will always execute the version of the object type that has been allocated.
	If the function was not virtual, the version called would be the one belonging to the object type that has been
	declared. In other words:

	struct A{
		virtual void foo(){}
	};
	class B : public A {};

	A *obj = new B();
	obj->foo() //This calls B::foo();

	struct C{
	virtual void foo(){}
	};
	class D : public C {};

	C *obj = new D();
	obj->foo() //This calls C::foo();
	*/

	/*
	13.4 What is the difference between deep copy and shallow copy? Explain how you
	would use each.

	Deep copy means making a duplicate of all content of an object, including all memory it is pointing to.
	Shallow copy means duplicating only the header of the object, but not the memory (actual data of interest)
	it is pointing to.

	Deep copies are necessary when we need to modify the contents of the buffer without altering the original
	one. It costly to make a copy, but we assure the original content is not damaged.

	Shallow copy is good when we only need a reference of the original buffer to READ it, isntead of altering it.
	It would be similar to passing a pointer to the original data structure, except that a shallow copy usually
	uses some sorte of reference counting, so that when all references to that data structure leave their scopes,
	the original buffer is automatically dealocated.*/

	/*13.5 What is the significance of the keyword "volatile" in C?		
	'volatile' is used to refer to a declaration (variable or structure) that may be altered by
	some process the compiler is unaware of (as in a concurrent thread). This way, the compiler
	won't try to optmize that variable, avoiding unexpected behavior when that unknown process try
	to use the variable.
	*/

	/*13.6 Why does a destructor in base class need to be declared virtual?
	
	It has to be virtual so that if a object declared as a base class, but allocated as a derived one, is
	deleted, the derived constructor is also called. In other words, if a base constructor is virtual (thus
	all the derived one are too), any delete on that type will actually cause the correct hierarchy of destructors
	to be called.*/

	/*Testing 13.8*/

	std::vector<int> vec;
	ex::SmartPtr<std::vector<int>> sPtr = new std::vector<int>(10);
	{
		auto sPtr2 = sPtr;
	}
	
	/*Testing 13.9*/
	//n = ex::nextPow2(n);


	return 0;
}