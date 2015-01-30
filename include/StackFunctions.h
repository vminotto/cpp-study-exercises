#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H

#include "Stack.h"
#include <stack>
#include <algorithm>
#include <iostream>

namespace ex{

	using std::cout;
	using std::endl;

	template <class T> void printStack(std::stack<T> s){
		while (!s.empty()){
			cout << s.top() << endl;
			s.pop();
		}
	}

	/*3.6 Write a program to sort a stack in ascending order (with biggest items on top).
	You may use at most one additional stack to hold items, but you may not copy
	the elements into any other data structure (such as an array). The stack supports
	the following operations: push, pop, peek, and isEmpty.*/
	template <class T> void sortStack(std::stack<T> &s1){
		
		std::stack<T> s2;
		
		int stackSize = s1.size();
		int numElemsOrdered = 0;
		

		while (numElemsOrdered < stackSize){
			int numMinsWithSameValue = 0;
			T min = std::numeric_limits<T>::max();
			for (int i = 0; i < stackSize - numElemsOrdered; i++){
				T el = s1.top();
				if (el < min){
					numMinsWithSameValue = 1;
					min = el;
				}
				else if (el == min)
					numMinsWithSameValue++;
				
				s1.pop();
				s2.push(el);
			}
			
			for (int i = 0; i < numMinsWithSameValue; i++)
				s1.push(min);
			
			numElemsOrdered += numMinsWithSameValue;
			
			/*Transfer back, except the mins, which are already in the correct order*/
			ex::transfer(s2, s1, min);
		}
	}
	
	template <class T> std::stack<T> sortStack2(std::stack<T> s) {
		std::stack<T> r;
		while (!s.empty()) {
			T tmp = s.top(); // Step 1
			s.pop();
			while (!r.empty() && r.top() > tmp) { // Step 2
				T el = r.top();
				s.push(el);
				r.pop();
			}
			r.push(tmp); // Step 3
		}
		return r;
	}	

	/*Transfer all elements from 'from' to 'to' except for the 'butElement', whose copies are discarded.*/
	template <class T> void transfer(std::stack<T> &from, std::stack<T> &to, T butElement){
		while (!from.empty()){
			T el = from.top();
			/*When returning the elements, we do not return the mins*/
			if (el != butElement)
				to.push(el);
			from.pop();
		}
	}

	void printHanoiTower(std::stack<int> t3, std::stack<int> t2, std::stack<int> t1){
		int s1 = t1.size();
		int s2 = t2.size();
		int s3 = t3.size();
		int sMax = std::max(s1, std::max(s2, s3));

		/*t3  t2  t1
	 	  |   |   |
	 	  |   |   |   */

		for (int i = sMax+1; i > 0; i--){
			if (i <= s3) { 
				cout << t3.top();
				t3.pop();
			}
			else 
				cout << "|"; cout << "  ";
			if (i <= s2) { 
				cout << t2.top();
				t2.pop();
			}
			else 
				cout << "|"; cout << "  ";
			if (i <= s1) { 
				cout << t1.top(); 
				t1.pop();
			}
			else 
				cout << "|"; cout << "  ";

			cout << endl;
		}
	}

	void trf(std::stack<int> &from, std::stack<int> &to){
		int el = from.top();
		from.pop();
		to.push(el);
	}

	void solveHanoiTowerRec(std::stack<int> &src, std::stack<int> &tmp, std::stack<int> &dst, int disk){
		if (disk == 1)
			ex::trf(src, dst);
		else{
			solveHanoiTowerRec(src, dst, tmp, disk - 1);
			trf(src, dst);
			solveHanoiTowerRec(tmp, src, dst, disk - 1);
		}
	}

	void solveHanoiTower(std::stack<int> &t3a, std::stack<int> &t2, std::stack<int> &t1a){
		
		std::stack<int> &t3 = t3a;
		std::stack<int> &t1 = t1a;

		int size = t3a.size();
		for (int i = 0; i < size; i++){
			ex::trf(t3, t2);
			ex::trf(t3, t1);
			ex::trf(t2, t1);
			ex::trf(t3, t2);
			ex::trf(t1, t3);
			ex::trf(t1, t2);
			ex::trf(t3, t2);
			ex::trf(t3, t1);
			ex::trf(t2, t3);
			ex::trf(t2, t1);
			ex::trf(t1, t3);
			ex::trf(t2, t1);
			ex::trf(t3, t2);
			ex::trf(t3, t1);
			ex::trf(t2, t1);
			ex::trf(t3, t2);
			
			t1 = t3a;
			t3 = t1a;
		}


	}

	





};

#endif
