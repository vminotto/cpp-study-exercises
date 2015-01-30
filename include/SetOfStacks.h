#ifndef SET_OF_STACKS_H
#define SET_OF_STACKS_H

#include <stack>
#include <list>
#include <initializer_list> 


/*3.3 Imagine a (literal) stack of plates. If the stack gets too high, it might topple.
Therefore, in real life, we would likely start a new stack when the previous stack
exceeds some threshold. Implement a data structure SetOf Stacks that mimics
this. SetOf Stacks should be composed of several stacks and should create a
new stack once the previous one exceeds capacity. SetOf Stacks. push() and
SetOf Stacks. pop() should behave identically to a single stack (that is, pop()
should return the same values as it would if there were just a single stack).
FOLLOW UP
Implement a function popAt(int index) which performs a pop operation on
a specific sub-stack.*/

namespace ex{

	template <class T> class SetOfStacks{

	public:

		/*Construct a SetOfStacks object with maxSize = 100*/
		SetOfStacks() {
			stacks.push_back(std::stack<T>());
			it = stacks.begin();
			this->maxSize = 100;
		}
		/*Construct a SetOfStacks object with provided 'maxSize'*/
		SetOfStacks(size_t maxSize) : SetOfStacks(){ this->maxSize = maxSize; }
		/*Construct a SetOfStacks object with provided 'maxSize' and initializer list*/
		SetOfStacks(size_t maxSize, std::initializer_list<T> initList) : SetOfStacks(maxSize) {
			for (auto &element : initList)
				this->push(element);
		}

		void push(T element){
			manageStackBeforePush();
			it->push(element);
		}
		void pop(){
			manageStackBeforePop();
			it->pop();
		}
		T&   top(){
			return it->top();
		}
		
		/*Pop an element of stack 'i'*/
		void popAt(int i){
			if (i >= stacks.size()) throw "Error at popAt(). The index exceeds the number of stacks.\n";
			else if (i < 0)			throw "Error at popAt(). The index must be a positive integer.\n";

			auto tempIt = std::next(stacks.begin(), i);
			if (tempIt->size() == 0)
				throw "Error at popAt(). The stack is already empty.\n";
			else
				tempIt->pop();
		}

		size_t size(){
			size_t size = 0;
			for_each(stacks.begin(), stacks.end(), [&](const std::stack<T> &s){size += s.size(); });
			return size;
		}

	private:

		void manageStackBeforePush(){
			if (it->size() == maxSize){ /*No space to add an element, so we grab the next stack*/
				if (std::next(it, 1) == stacks.end()) /*Means a new stack must be created*/
					stacks.push_back(std::stack<T>());
				it++;
			}
		}
		void manageStackBeforePop(){
			if (it->size() == 0){ /*No element to pop, so we grab the previous stack*/

				if (it == stacks.begin()) /*No previous exist*/
					throw "Error poping the stack. It is already empty!\n";
				else
					it--;

			}
		}

		/*Pointer to the actual stack being used*/
		typename std::list<std::stack<T>>::iterator it;
		/*Index of the actual stack being used*/
		int i;

		size_t maxSize;
		std::list<std::stack<T>> stacks;

	};

};


#endif