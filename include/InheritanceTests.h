#ifndef INHERITANCE_TESTS_H
#define INHERITANCE_TESTS_H

#include <iostream>
using namespace std;
	
/*in a c++ 'delete' operation of a class within an inheritance, destructor of the DECLARED type will
be called, which implies in the calling of the base classes' destructrs aftewards.

If the destructors are virtual (which happens when the base class' destructor has the 'virtual keyword), the
destructor called will be the one belonging to the POINTING type (the type used in the 'new'). The upwards
call will remain the same.
*/

struct A1{
	virtual ~A1(){
		int a = 5;
	}
	void print(){ cout << "This is A1" << endl; }
	virtual void printVirtual() { cout << "This is A1 (virtual)" << endl; }
	virtual void newPrint(){}
};

struct B1 : public A1{
	~B1(){
		int a = 5;
	}
	void print(){ cout << "This is B1" << endl; }
	/*This function is virtual even if the keyword 'virtua' is not used, since the base class function is set to virtual.*/
	void printVirtual() { cout << "This is B1 (virtual)" << endl; }

};

struct C1 : public B1{
	~C1(){
		int a = 5;
	}
	void print(){ cout << "This is C1" << endl; }
	void printVirtual() { cout << "This is C1 (virtual)" << endl; }
	void newPrint() { cout << "Teste" << endl; }
};

struct A2{
	virtual ~A2(){
		int a = 5;
	}
	void print(){ cout << "This is A2" << endl; }
	virtual void printVirtual(){ cout << "This is A2 (virtual)" << endl; }
};

struct B2 : public A2{
	~B2(){
		int a = 5;
	}
	virtual void print(){ cout << "This is B2" << endl; }
	virtual void printVirtual(){ cout << "This is B2 (virtual)" << endl; }
};



#endif