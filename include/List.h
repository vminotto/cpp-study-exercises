#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <initializer_list>
#include <ostream>
#include <functional>
#include <set>
#include <sstream>
#include <stack>

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

		bool isHeadNode = false;
		bool isTailNode = false;

	public:

		ListNode(const T &element) : element(element) {}
		ListNode() = default;
		~ListNode(){}

		bool isHead(){ return isHeadNode; }
		bool isTail(){ return isTailNode; }

		ListNode<T> *next = 0;
		ListNode<T> *prev = 0;
		T element; /*Will be automatically freed when this class' instance is deleted*/
	};

	template <class T> class ListIterator
		: public std::iterator<std::bidirectional_iterator_tag, T>
	{

	private:

		ListNode<T> *node;

		friend class ex::List<T>;

		//Similar to std::list<T>::iterator::_Mynode;
		ListNode<T> *getNode(){ return node; }

	public:

		ListIterator(const ListIterator<T>& iterator) : node(iterator.node) {}
		ListIterator(ListNode<T> *node) : node(node){}

		/*Pre-increment operators, as in ++myInstance and --myInstance*/
		ListIterator<T>& operator++() { node = node->next; return *this; }
		ListIterator<T>& operator--() { node = node->prev; return *this; }

		/*Post-increment operators, as in myInstance++ and myInstance--
		The 'int' parameter is just a dummy variable the compiler uses interally
		to distinguish both operators overloads.*/
		ListIterator<T> operator++(int) { ListIterator<T> tempIter(*this); ++(*this); return tempIter; }
		ListIterator<T> operator--(int) { ListIterator<T> tempIter(*this); --(*this); return tempIter; }

		/*Useful when we want to access the next/prev position without modifying the iterator's state
		   i = a.next() is equivalent to i = ++a; --a;
		   i = a.prev() is equivalent to i = --a; ++a;
		   Similar TO std::list<T>::iterator::_Getpnext*/
		ListIterator<T> next(){ return ListIterator<T>(node->next); }
		ListIterator<T> prev(){ return ListIterator<T>(node->prev); }

		bool operator==(const ListIterator<T>& rhs) { return node == rhs.node; }
		bool operator!=(const ListIterator<T>& rhs) { return node != rhs.node; }

		friend bool operator==(const ListIterator<T>& lhs, const ListIterator<T>& rhs){ return lhs.node == rhs.node; }
		friend bool operator!=(const ListIterator<T>& lhs, const ListIterator<T>& rhs){ return lhs.node != rhs.node; }

		T& operator*() { return  node->element; }
		T* operator->(){ return &node->element; }
	};

	template <class T> class List {

	public:

		List(std::initializer_list<T> elements) : List() {
			if (elements.size() <= 0) return;

			for (auto &el : elements){
				this->push_back(el);
			}
		}

		List(){
			head = new ListNode<T>();
			tail = new ListNode<T>();
			connectHeadToTail();
			head->isHeadNode = true;
			tail->isTailNode = true;
		}

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

		ListIterator<T>  begin(){ return ex::ListIterator<T>(head->next); }
		ListIterator<T>    end(){ return ex::ListIterator<T>(tail); }

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

		void push_back(const T &element){
			ListNode<T> * newNode = new ListNode<T>(element);
			tail->prev->next = newNode;
			newNode->prev = tail->prev;
			newNode->next = tail;
			tail->prev = newNode;
			size++;
		}
		void push_front(const T &element){
			ListNode<T> * newNode = new ListNode<T>(element);
			head->next->prev = newNode;
			newNode->next = head->next;
			newNode->prev = head;
			head->next = newNode;
			size++;
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
				delete tail->prev;
				connectHeadToTail();
			}
			else{
				ListNode<T> *prevNode = tail->prev;
				tail->prev = prevNode->prev;
				prevNode->prev->next = tail;
				delete prevNode;
			}
			size--;
		}
		void pop_front(){
			if (size == 0)
				return;

			else if (size == 1){
				delete head->next;
				connectHeadToTail();
			}
			else{
				ListNode<T> *nextNode = head->next;
				head->next = nextNode->next;
				nextNode->next->prev = head;
				delete nextNode;
			}
			size--;
		}
		/*2.3 Implement an algorithm to delete a node in the middle of a singly linked list, given only access to that node.*/
		void remove(int pos){
			if (pos < 0 || (unsigned)pos >= size)
				throw "Error: List removal is out of bounds\n.";
			else if (pos == 0)
				this->pop_front();
			else if (pos == size - 1)
				this->pop_back();
			else{
				ListNode<T> *temp = getNodeAt(pos);
				remove(temp);
			}
		}
		

		int getSize(){ return size; }
		bool isEmpty(){ return size == 0; }

		ListNode<T> *getHead(){ return head; }
		ListNode<T> *getTail(){ return tail; }

		/*Just for helping debug (the template argument T must have a << implementatin for printing, obviously).*/
		void printElements(char separator = ',', std::ostream &oStream = std::cout){
			
			/*Approach using for_each and a lambda function*/
			//std::for_each(begin(), end(), [&](T a) {oStream << a << " "; });
			//oStream << endl;

			/*Approach using range-based for (which is possible because we have implemented the
			iterator-based begin() and end() functions*/
			unsigned long i = 0;
			for (auto &element : *this){
				oStream << element << ( i < size-1 ? separator : ' ');
				i++;
			}
			oStream << endl;
		}

		/*2.1 Write code to remove duplicates from an unsorted linked list.
		How would you solve this problem if a temporary buffer is not allowed?*/
		void removeDuplicates(std::function<bool(T, T)> equals = [](T a, T b) {return a == b; })
		{
			/*If we knew the data type of this list beforehand, and if this data type could only assume a short
			range of values, we could create a diccionary using an array, as in bool dic[maxNumValues]; 
			This would be possible, e.g., for 'char', where only 256 values are assumed. For other values,
			an std::set is preferable.*/
			std::set<T> unique;
			for (auto i = begin(); i != end(); i++){
				/*If tested true, it means the element was not inserted, because it already exists in the std::set<T>
				Testing using the insert method is more efficient, because it causes less function calls.*/
				if (! unique.insert(*i).second){
					i--;
					this->remove(i.next().getNode());
				}
			}

		}
		void removeDuplicatesNoTempBuffer(std::function<bool(T, T)> equals = [](T a, T b) {return a == b; }){
			for (auto i = begin(); i != end(); i++){
				for (auto j = i.next(); j != end(); j++){
					if (equals(*i,*j)){
						j--;
						this->remove(j.next().getNode());
					}
				}
			}
		}
		/*2.2 Implement an algorithm to find the kth to last element of a singly linked list*/
		T findKthToLastSizeUnknwon(int k){
			ListNode<T> *foundNode;
			int auxInd = 0;
			findRecursive(k, head->next, auxInd, foundNode);
			return foundNode->element;
		}
		void findRecursive(int k, ListNode<T> *firstNode, int &index, ListNode<T> *&foundNode){	
			if (firstNode == tail)
				return;

			findRecursive(k, firstNode->next, index, foundNode);

			if (index == k) /*This works only because foundNode is altered when index == k, otherwise it is untouched*/
				foundNode = firstNode;
			
			index++;
		}

		/*Sort the list. Default is ascending order. If some other criteria should be
		used, a function must be provided.*/
		void sortNoExtraMem(std::function<bool(T, T)> func = [](T a, T b) {return a < b; })
		{
			//int c = 0;
			for (int i = 0; i < size; i++){
				for (auto j = begin(); j != --end(); j++){
					if (func(*j.next(), *j)){
						swap(j.getNode(), j.next().getNode());
						j--;
						//c++;
					}
				}
			}
			//cout << "Sorting done. Orignal list size: " << size << ". Num swaps taken: " << c << "." << endl;
		}

		/*2.4 Write code to partition a linked list around a value x, such that all nodes less than x come before 
		all nodes greater than or equal to x. This approach takes O(1) space, and O(N) time*/
		void partitionAroundValue(T value, std::function<bool(const T &element, const T &value)> greaterThen = 
			[](const T &element, const T &value){return element > value; })
		{
			/*This version of the method attempts to keep the element's orders. If this was not
			needed, simple push_back() and push_front() calls could be used.*/
			ListNode<T> *refLeft = 0, *refRight = 0, *firstRefLeft = 0, *firstRefRight = 0;
			ListNode<T> *tmpNode = 0;

			for (auto i = begin(); i != end(); i++){
				if (greaterThen(*i, value)){
					/*Move node to after 'value's node*/
					if (!refRight){ //First time we do not move, just record the node.
						refRight = i.getNode();
						firstRefRight = refRight;
					}
					else{
						i++; //Preserving the iterator (otherwise it would land on the transfered position);
						tmpNode = i.prev().getNode();
						transferAfter(refRight, tmpNode);
						refRight = tmpNode;
						i--; //Jumping backwards to original position;
					}
				}
				else
				{
					/*Move node to before 'value's node*/
					if (!refLeft){ //First time we do not move, just record the node.
						refLeft = i.getNode();
						firstRefLeft = refLeft;
					}
					else{
						i++; //Preserving the iterator (otherwise it would land on the transfered position);
						tmpNode = i.prev().getNode();
						transferAfter(refLeft, tmpNode);
						refLeft = tmpNode;
						i--; //Jumping backwards to original position;
					}
				}
			}

			/*                             fr      r    fl      l
			  In case list was built as  h-5-6-7-8-6-()-4-3-2-1-1-t
			
			                               fl      l    fr      r
			we change it to be like this h-4-3-2-1-1-()-5-6-7-8-6-t */
			if (firstRefLeft->prev != head && firstRefRight->prev == head){
				head->next = firstRefLeft;
				firstRefLeft->prev = head;
				refLeft->next = firstRefRight;
				firstRefRight->prev = refLeft;
				tail->prev = refRight;
				refRight->next = tail;
			}
		}

		/*2.6 Given a circular linked list, implement an algorithm which returns the node at
			the beginning of the loop.
			DEFINITION
			Circular linked list: A (corrupt) linked list in which a node's next pointer points
			to an earlier node, so as to make a loop in the linked list.
			EXAMPLE
			Input: A - > B - > C - > D - > E - > C [the same C as earlier]
			Output: C*/
		ListNode<T> *getCircularNode()
		{
			/*This approach is O(N) memory and O(N) complexity*/
			/*We assuem there will be only one circular node in this implementation*/
			if (size <= 1)
				return 0;

			std::set<ListNode<T> *> unique;
			for (auto i = begin(); i != end(); i++)
				if (!unique.insert(i.getNode()).second)
					return i.getNode();

			return 0;
		}
		ListNode<T> *getCircularNodeNoMem()
		{
			/*We assuem there will be only one circular node in this implementation*/
			/*This approach is O(1) memory and O(1) complexity*/
			if (size <= 1)
				return 0;


			ListNode<T> *fast, *slow;

			fast = head->next;
			slow = head->next;
			/*A B C D E F G H J I K
			                S     L
			                R     M
							Q P O N
			*/

			while (fast != tail && slow != tail){
				fast = fast->next->next;
				slow = slow->next;
				if (slow == fast)
					break;
			}

			fast = head->next;
			while (fast != slow){
				fast = fast->next;
				slow = slow->next;
			}
			return fast;
		}
		void causeCircularNodePurposely(){
			int n = size / 2;
			ListNode<T> *node = this->getNodeAt(n);
			tail->prev->next = node;
		}

		/*2.7 Implement a function to check if a linked list is a palindrome*/
		bool isPalindrome(){

			ListNode<T> *fast, *slow;

			fast = head->next;
			slow = head->next;

			/*Palindrome 0 1 2 3 2 1 0   or   0 1 2 3 1 3 2 1 0
						   3 inserted           4 inserted
						 */

			std::stack<T> firstHalf;

			while (fast != tail && fast != tail->prev){
				firstHalf.push(slow->element);
				fast = fast->next->next;
				slow = slow->next;
			}

			if (fast == tail->prev) /*if fast == tail->prev, the list is odd, if fast == tail, list is even*/
				firstHalf.push(slow->element);

			while (slow != tail){
				if (slow->element != firstHalf.top())
					return false;

				firstHalf.pop();
				slow = slow->next;
			}

			return true;
		}

	private:

		/*C++11 allows member variables to be initialized directly in the class definition
		The initialization is done when the constructor is called, and it happens before any
		line in the constructors implementation is executed (same behavior as constructor list)*/
		unsigned long size = 0;
		/*In this implementation, 'head' is the before-the-beggining element, that is, the value when dereferencing rend()*/
		ex::ListNode<T> *head;
		/*In this implementation, 'tail' is the past-the-end element, that is, the value when dereferencing end()*/
		ex::ListNode<T> *tail;

		ListNode<T> *getNodeAt(int pos){

			/*Given this list is doubly linked, we may search backwards too*/
			int middle = (int)std::ceil(size / 2.0 - 1);

			ListNode<T> *temp = 0;

			if (pos <= middle){
				temp = head->next;
				for (int i = 1; i <= pos; i++)
					temp = temp->next;
			}
			else{
				temp = tail->prev;
				for (int i = size - 2; i >= pos; i--)
					temp = temp->prev;
			}
			return temp;
		}
		bool isConnected(ListNode<T> *node){
			bool isLinkedTo = node->next && node->prev;
			bool isReceivingLink = node->next->prev == node && node->prev->next == node;
			return isLinkedTo & isReceivingLink;
		}
		void connectHeadToTail(){
			head->next = tail;
			tail->prev = head;
		}
		void remove(ListNode<T> *node){
			if (node->isHead() || node->isTail())
				throw "Error at 'List<T>::remove()': cannot remove head or tail node.\n";
			else if (!isConnected(node))
				throw "Error at 'List<T>::remove()': node is not properly connect. Make sure it belongs to this List.\n";

			ListNode<T> *prevEl = node->prev;
			ListNode<T> *nextEl = node->next;

			prevEl->next = nextEl;
			nextEl->prev = prevEl;

			delete node;
			size--;

		}

		/*Swaps the adjacents nodes 'a' and 'b', where a comes before b*/
		/*x-a-b-y
		  x-b-a-y*/
		void swapAdjacents(ListNode<T> *a, ListNode<T> *b){

			if (a->next != b || b->prev != a)
				throw "Error at 'ex::List::swapAdjacents': elements are not adjacents.";
			else if (a->isTail() || b->isHead())
				throw "Error at 'ex::List::swapAdjacents': cannot swap head and tail.";
			else{
				a->prev->next = b;
				b->next->prev = a;
				b->prev = a->prev;
				a->next = b->next;
				a->prev = b;
				b->next = a;
			}
		}

		/*Swaps two nodes 'a' and 'b', where a comes before b*/
		/*x-a-t-b-y
		  x-b-t-a-y*/
		void swap(ListNode<T> *a, ListNode<T> *b){

			if (a->isTail() || a->isHead() || b->isTail() || b->isHead())
				throw "Error at 'ex::List::swapAdjacents': cannot swap head and tail.";
			else if (a->next == b && b->prev == a)
				swapAdjacents(a, b);
			else{
				a->prev->next = b;
				b->next->prev = a;
				
				b->prev->next = a;
				a->next->prev = b;
				
				std::swap(a->prev, b->prev);
				std::swap(a->next, b->next);
			}
		}
		/*Same swap as above, but also swapping the iterators*/
		//void swap(ListIterator<T> &a, ListIterator<T> &b){
		//	this->swap(a.getNode(), b.getNode());
		//	std::swap(*a.getNode(), *b.getNode());
		//}

		ListNode<T> *removeAndGet(ListNode<T> *node){
			
			if (node->isHead() || node->isTail())
				throw "Error at 'List<T>::removeNoDelete()': cannot delete head or tail nodes.\n";

			node->next->prev = node->prev;
			node->prev->next = node->next;
			node->next = 0;
			node->prev = 0;

			return node;
		}
		void insertAfter(ListNode<T> *base, ListNode<T> *&node){
			
			if (base->isTail())
				throw "Error at 'List<T>::insertAfter()': cannot insert a node after the List's tail.\n";
			else if (node->isHead() || node->isTail())
				throw "Error at 'List<T>::insertAfter()': cannot move the head or tail nodes.\n";
			
			/*
			from:   BASE-B
			  to:   BASE-NODE-B
			*/
			base->next->prev = node;
			node->next = base->next;
			node->prev = base;
			base->next = node;
		}
		void insertBefore(ListNode<T> *base, ListNode<T> *&node){

			if (base->isHead())
				throw "Error at 'List<T>::insertBefore()': cannot insert a node before the List's head.\n";
			else if (node->isHead() || node->isTail())
				throw "Error at 'List<T>::insertBefore()': cannot move the head or tail nodes.\n";

			/*
			from:      A-BASE
			to:   A-NODE-BASE
			*/
			base->prev->next = node;
			node->prev = base->prev;
			node->next = base;
			base->prev = node;
		}
		void transferAfter(ListNode<T> *base, ListNode<T> *&node){
			
			if (node->prev == base && base->next == node) //No need to transfer
				return;

			node = removeAndGet(node);
			insertAfter(base, node);
		}
};

}

#endif