#include "Employee.h"
#include "InheritanceTests.h"
#include "Jukebox.h"
#include <iostream>
#include <string>
#include "HashTests.h"
#include "HashTable.h"
#include <sstream>
#include <algorithm> 
#include <vector>
#include "File.h"
#include "Directory.h"
#include "Entry.h"

using namespace std;

using uint = unsigned int;

int main(){
/*
	A1 *b1 = new B1();
	b1->print();
	b1->printVirtual();
	delete b1;
	
	A1 *c1 = new C1();
	c1->print();
	c1->printVirtual();
	delete c1;

	B1 *cc1 = new C1();
	cc1->print();
	cc1->printVirtual();
	delete cc1;

	B1 *b1 = new B1();
	b1->print();
	b1->printVirtual();
	delete b1;

	A2 *a2 = new B2();
	a2->print();
	a2->printVirtual();
	delete a2;

	B2 *b2 = new B2();
	b2->print();
	b2->printVirtual();
	delete b2;*/


	std::vector<std::pair<int, string>> sdsd = { { 1, "a" }, { 2, "b" }, { 3, "c" }, { 4, "d" } };
	auto itr = sdsd.end();

	ex::HashTable<int, std::string> t(5, 3.0, 3.0);

	for (int i = 0; i < 7; i++){
		std::stringstream ss;
		ss.width(5);
		ss << std::right << i;
		t[i] = ss.str();
	}

	/*This will insert all elements, because even though the hash fails, the equal will correct the colisions caused
	by the bad hash*/
	ex::testBadHashGoodEqual();
	/*This won't insert all elements, because the equal function is band, and thus won't compensate for the colisions
	caused by the bad hashing*/
	ex::testBadHashBadEqual();  
	/*This will insert all elements, because the hash does not generate colisions, which is enough to guarantee no
	repeated elements are going to be inserted. In other words, the bad equal function doest not even get called.*/
	ex::testGoodHashBadEqual();

	/*Example of teste for 8.3*/
	ex::JukeboxBase &asdf = ex::JukeboxBase::getInstance();

	/*Example of test for 8.2 (if all methods were implemented)*/
	ex::CallManager &manager = ex::CallManager::getInstance();
	try{
		manager.dispatchCall(new ex::PhoneCall, { ex::Employee::Type::Respondent, ex::Employee::Type::Manager, ex::Employee::Type::Director });
	}
	catch (const std::string &error){
		cout << "Errror caught in main8.cpp:" << endl;
		cout << error;
	}

	cin.get();



	return 0;
}