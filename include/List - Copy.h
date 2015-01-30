#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <initializer_list>
#include <ostream>
#include <functional>

/*Why use reverse_iterator instead of decrementing a normal forward_iterator??
Good answer here
http://stackoverflow.com/questions/188942/how-do-you-iterate-backwards-through-an-stl-list
This is done to simplify the implementation, in order for the range in reverse to have the same semantics as a range forward [begin, end) and [rbegin, rend)*/

/*How to make a range-based for loop iterates backwards in a container, that is, from rbegin() to rend()
In other words, how to make the following iterates backwards instead of forward
for (auto& i: c)

answer: http://stackoverflow.com/questions/8542591/c11-reverse-range-based-for-loop

*/

namespace ex{

	/*Forward declaration*/
	template <class T> class List;

	template <class T> class ListNode{

	private:

		friend class ex::List<T>;

		bool isTailNode = false;
		bool isHeadNode = false;
		bool isPastTailNode = false;
		bool isPrevHeadNode = false;
		
	public:

		ListNode(const T &element) : element(element) {
			int a = 5;
		
		}
		~ListNode(){}

		bool isHead(){ return isHeadNode; }
		bool isTail(){ return isTailNode; }
		bool isPastTail(){ return isPastTailNode; }
		bool isPrevHead(){ return isPrevHeadNode; }

		ListNode<T> *next;
		ListNode<T> *prev;
		T element; /*Will be automatically freed when this class' instance is deleted*/
	};

	template <class T> class ListIterator
		: public std::iterator<std::bidirectional_iterator_tag, T>
	{
	
	private:
		ListNode<T> *node;
	public:

		ListIterator(const ListIterator<T>& iterator) : node(iterator.node) {}
		ListIterator(ListNode<T> *node) : node(node){}

		/*Pre-increment operators, as in ++myInstance and --myInstance*/
		ListIterator<T>& operator++() { node = node->next; return *this; }
		ListIterator<T>& operator--() { 
			node = node->prev; return *this; 
		}

		/*Post-increment operators, as in myInstance++ and myInstance--
		The 'int' parameter is just a dummy variable the compiler uses interally
		to distinguish both operators overloads.*/
		ListIterator<T> operator++(int) { ListIterator<T> tempIter(*this); ++(*this); return tmp; }
		ListIterator<T> operator--(int) { ListIterator<T> tempIter(*this); --(*this); return tmp; }

		bool operator==(const ListIterator<T>& rhs) { return node == rhs.node; }
		bool operator!=(const ListIterator<T>& rhs) { return node != rhs.node; }

		friend bool operator==(const ListIterator<T>& lhs, const ListIterator<T>& rhs){ return lhs.node == rhs.node; }
		friend bool operator!=(const ListIterator<T>& lhs, const ListIterator<T>& rhs){ return lhs.node != rhs.node; }

		T& operator*() { return  node->element; }
		T* operator->(){ return &node->element; }
	};

	template <class T> class List {

	public:

		List(std::initializer_list<T> elements){
			if (elements.size() <= 0) return;

			for (auto &el : elements){
				this->push_back(el);
			}
		}

		/*Nice C++11 feature. Useless in this case, though. Would be same as List(){}*/
		List() = default;

		/*The constructor delegation here is also useless, but just showing a nice new C++11 feature.*/
		List(ex::List<T> &example) : List() {
			for (ListNode<T> *node = example.getHead(); node != 0; node = node->next){
				this->push_back(node->element);
			}

		}

		~List(){
			if (size == 1){
				delete head;
				size = 0;
			}
			else{
				for (ListNode<T> *node = head->next; node != 0; node = node->next){
					delete node->prev;
					size--;
				}
				delete tail;
				size--;
				if (size != 0){
					throw "Error at 'List::~List()': size of the list should be 0 after destruction, but is not.\n";
				}
			}
		}

		typedef std::reverse_iterator<ex::ListIterator<T>> RevIter;

		ListIterator<T>  begin(){ return ex::ListIterator<T>(this->head);}
		ListIterator<T>    end(){ return ex::ListIterator<T>(this->tail->next); }
		/*Here the std::reverse_iterator class provides an easy way of reversing an iterator*/
		RevIter rbegin(){ return RevIter(this->end()); }
		RevIter   rend(){ return RevIter(this->begin()); }

		T &first(){ return head; }
		T &last(){ return tail; }
		T &getAt(int pos){
			if (pos < 0 || pos >= size)
				throw "Error: List insertion is out of bounds\n.";

			return getNodeAt(pos)->element;
		}

		bool isEmpty(){ return head == 0 && tail == 0; }

		void push_back(const T &element){

			ListNode<T> *newTail = new ListNode<T>(element);

			if (size > 0){ /*Means this is not the first insert*/
				newTail->prev = tail;
				tail->next = newTail;
			}
			tail = newTail;
			size++;
			this->reArrangeTail();
			this->checkForSize();
		}
		void push_front(const T &element){

			ListNode<T> *newHead = new ListNode<T>(element);

			if (size > 0){ /*Means this is not the first insert*/
				newHead->next = head;
				head->prev = newHead;
			}
			head = newHead;
			size++;
			this->reArrangeHead();
			this->checkForSize();
		}
		void insert(const T &element, int pos){

			if (pos < 0 || pos >= size)
				throw "Error: List insertion is out of bounds\n.";
			else if (pos == 0)
				this->push_front(element);
			else{

				ListNode<T> *newEl = new ListNode<T>(element);
				ListNode<T> *temp = getNodeAt(pos);

				newEl->next = temp;
				newEl->prev = temp->prev;
				temp->prev->next = newEl;
				temp->prev = newEl;
				size++;
			}
		}

		void pop_back(){
			if (size == 0)
				return;

			else if (size == 1){
				delete tail;
				tail = 0;
				head = 0;
			}
			else{
				ListNode<T> *prevNode = tail->prev;
				prevNode->next = 0;
				delete tail;
				this->tail = prevNode;
			}
			size--;
			this->reArrangeTail();
			this->checkForSize();
		}
		void pop_front(){
			if (size == 0)
				return;

			else if (size == 1){
				delete tail;
				tail = 0;
				head = 0;
			}
			else{
				ListNode<T> *nextNode = tail->next;
				nextNode->prev = 0;
				delete head;
				this->head = nextNode;
			}
			size--;
			this->reArrangeHead();
			this->checkForSize();
		}
		void remove(int pos){
			if (pos < 0 || pos >= size)
				throw "Error: List removal is out of bounds\n.";
			else if (pos == 0)
				this->pop_front();
			else if (pos == size - 1)
				this->pop_back();
			else{

				ListNode<T> *temp = getNodeAt(pos);
				ListNode<T> *prevEl = temp->prev;
				ListNode<T> *nextEl = temp->next;

				prevEl->next = nextEl;
				nextEl->prev = preVEl;

				delete temp;
				size--;
			}
			this->checkForSize();
		}

		int getSize(){ return size; }

		ListNode<T> *getHead(){ return head; }
		ListNode<T> *getTail(){ return tail; }

		/*Just for helping debug (the template argument T must have a << implementatin for printing, obviously).*/
		void printElements(std::ostream &oStream = std::cout){
			//for (ex::ListNode<T> *node = head; node != 0; node = node->next)
			//	oStream << node->element << " ";
			//oStream << endl;
		}

		/*2.1 Write code to remove duplicates from an unsorted linked list.
		How would you solve this problem if a temporary buffer is not allowed?*/
		//void removeDuplicates(){
		//	List<T> temp
		//}

		/*Sort the list. Default is ascending order. If some other criteria should be
		used, a function must be provided.*/
		void sort(std::function<bool(T, T)> func = [](T a, T b) {return a < b; })
		{

		}

	private:

		/*C++11 allows member variables to be initialized directly in the class definition
		The initialization is done when the constructor is called, and it happens before any
		line in the constructors implementation is executed (same behavior as constructor list)*/
		unsigned long size = 0;
		ListNode<T> *head = 0;
		ListNode<T> *tail = 0;

		ListNode<T> *getNodeAt(int pos){

			/*Given this list is doubly linked, we may search backwards too*/
			int middle = std::ceil(size / 2.0 - 1);

			ListNode<T> *temp = 0;

			if (pos <= middle){
				temp = head;
				for (int i = 1; i <= pos; i++)
					temp = temp->next;
			}
			else{
				temp = tail;
				for (int i = size - 2; i >= pos; i--)
					temp = temp->prev;
			}
			return temp;
		}

		void checkForSize(){
			if (size == 1){
				if (!tail && !head)
					throw "Error at ex::List, size is 1 and no head nor tail exist.";
				else if (tail == 0) tail = head;
				else if (head == 0) head = tail;
				tail->isTailNode = true;
				tail->isHeadNode = true;
				reArrangeTail();
				reArrangeHead();
			}
		}

		void reArrangeTail(){			
			
			tail->isPastTailNode = false;
			tail->isTailNode = true;
			
			tail->next->isPastTailNode = true;
			tail->next->isTailNode = false;

			if (size > 1) 
				tail->prev->isTailNode = false;
		}
		void reArrangeHead(){
			head->isPrevHeadNode = false;
			head->isHeadNode = true;
			
			head->prev->isPrevHeadNode = true;
			head->prev->isHeadNode = false;

			if (size > 1)
				head->next->isHeadNode = false;
		}

	};

}

#endif