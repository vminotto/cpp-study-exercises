#ifndef STACK_H
#define STACK_H

#include <initializer_list>

namespace ex{

	/*This stack class is implemented as having a list as base structure
	We could have a second template argument to chose which base it would be,
	and thus, easen the implementation, but we will just implement from scratch
	as an excercise.*/

	template <class T> struct StackNode{
		StackNode() = default;
		StackNode(T element) : element(element){}
		StackNode *prev = 0;
		T element;
	};

	template <class T> class Stack{

	public:
		Stack() = default;
		Stack(std::initializer_list<T> elements){
			for (auto &el : elements)
				push(el);
		}

		int getSize(){ return size; }

		void push(T element){
			StackNode<T> *node = new StackNode<T>(element);
			
			if (size > 0){
				node->prev = last;
			}
			last = node;
			size++;
		}
		T &top(){ return last->element; }
		void pop(){
			if (size == 0)
				throw "Error at 'Stack<T>::pop()': no elements to be popped.\n";
			else if (size == 1){
				delete last;
				last = 0;
				size--;
			}
			else{
				StackNode<T> *node = last;
				last = last->prev;
				delete node;
				size--;
			}
		}


	protected:

		/*Unlike a list, which should provide iterators, a stack does not need to.
		Therefore, we do not need to have a before-the-first and past-the-end elements.
		This allows us to test if the stack has one element left is 'last->prev == 0'*/

		StackNode<T> *last = 0;
		unsigned long size = 0;


	};


	/*3.2 How would you design a stack which, in addition to push and pop, also has a
	function min which returns the minimum element? Push, pop and min should
	all operate in O(1) time.*/
	template <class T> class StackWithMin : public Stack<T>{
	
	public:
		StackWithMin(std::initializer_list<T> elements){
			for (auto &el : elements)
				push(el);
		}
		StackWithMin() : Stack() {}

		void push(T element){
			if(size == 0)
				mins.push(element);
			else
				mins.push(std::min(element, Stack::top()));
			Stack::push(element);
		}
		void pop(){
			if (mins.getSize() == 0)
				throw "Error at 'StackWithMin<T>::pop()': no elements to be popped.\n";
			Stack::pop();
			mins.pop();
		}
	
		 Stack<T> *getMins() { return &mins; }
	protected:
	
		Stack<T> mins;

	};

	
}


#endif