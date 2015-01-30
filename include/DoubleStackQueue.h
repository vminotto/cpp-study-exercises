#ifndef DOUBLE_STACK_QUEUE
#define DOUBLE_STACK_QUEUE

#include <stack>
#include <initializer_list> 

/*3.5 Implement a MyQueue class which implements a queue using two stacks*/


namespace ex{

	template <class T> class DoubleStackQueue{

	public:

		DoubleStackQueue() = default;
		DoubleStackQueue(std::initializer_list<T> initList){
			s1 = std::stack<T>(initList);
		}
		~DoubleStackQueue() = default;

		void enqueue(T element){
			if (!s2.empty() && s1.empty())
				transfer(s2, s1);
			s1.push(element);
		}
		void dequeue(){
			if (s1.empty() && s2.empty())
				throw "No element to be dequeued.\n";
			else if (s1.empty() && !s2.empty())
				s2.pop();
			else if (!s1.empty() && s2.empty()){
				transfer(s1, s2);
				s2.pop();
			}
		}
		T&   front(){
			if (s1.empty() && s2.empty())
				throw "No element to be returned.\n";
			else if (s1.empty() && !s2.empty())
				return s2.top();
			else if (!s1.empty() && s2.empty()){
				transfer(s1, s2);
				return s2.top();
			}
		}
		T&   back(){
			if (s1.empty() && s2.empty())
				throw "No element to be returned.\n";
			else if (s1.empty() && !s2.empty())
				transfer(s2, s1);
			
			return s1.top();
		}
		size_t size(){ return s1.size() + s2.size(); }
		bool empty(){ return s1.empty() & s2.empty(); }
	private:

		void transfer(std::stack<T> &from, std::stack<T> &to){
			int size = from.size();
			for (int i = 0; i < size; i++){
				to.push(from.top());
				from.pop();
			}
		}
		void transferToReserve(){
			while(s1.size() > 0){
				s2.push(s1.top());
				s1.pop();
			}
		}

		/*s1 is the actual buffer. s2 is the reserve used for temporary transfers*/
		std::stack<T> s1, s2;
	};
};



#endif