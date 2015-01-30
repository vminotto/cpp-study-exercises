#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <initializer_list>

namespace ex{

	template <class T> class Heap{

	public:

		using index = typename std::vector<T>::size_type;

		Heap(std::initializer_list<T> initList){
			for (const T &v : initList){
				push(v);
			}
		}
		Heap() = default;
		~Heap() = default;

		void push(const T &value){

			/*Push to the back of the heap*/
			vec.push_back(value);

			index i = vec.size() - 1;

			/*Organize element to the proper position in the heap*/
			while (hasParent(i) && vec[getParent(i)] < vec[i]){
				std::swap(vec[getParent(i)], vec[i]);
				i = getParent(i);
			}
		}

		void pop(){

			if (vec.size() == 0)
				return;
			else if (vec.size() == 1)
				vec.pop_back();
			else{

				std::swap(vec.front(), vec.back());
				vec.pop_back();

				index i = 0;

				while (hasHigherChildren(i)){
					if (!hasRightChild(i) || vec[getLeft(i)] >= vec[getRight(i)]){
						std::swap(vec[i], vec[getLeft(i)]);
						i = getLeft(i);
					}
					else{
						std::swap(vec[i], vec[getRight(i)]);
						i = getRight(i);
					}
				}

			}



		}
		T &front(){ return vec.front(); }

		typename std::vector<T>::iterator begin(){ return vec.begin(); }
		typename std::vector<T>::iterator   end(){ return vec.end(); }

		index size(){
			return vec.size();
		}
		bool empty(){return vec.size() == 0;}

	private:	

		std::vector<T> vec;

		index getParent(index i)	{ return (i - 1) / 2; }
		index getLeft(index i)		{ return 2 * i + 1; }
		index getRight(index i)		{ return 2 * i + 2; }
		
		bool hasLeftChild(index i)	{ return (2 * i + 1) < vec.size(); }
		bool hasRightChild(index i) { return (2 * i + 2) < vec.size(); }
		bool hasParent(index i)		{ return i != 0; }

		bool hasChildren(index i)	{ return hasLeftChild(i) || hasRightChild(i); }
		bool hasHigherChildren(index i) {
			return (hasLeftChild(i) && vec[getLeft(i)] > vec[i]) || (hasRightChild(i) && vec[getRight(i)] > vec[i]);
		}

	};	
};

#endif