#include <fstream>
#include <string>
#include <list>
#include <cstdint> 
#include <algorithm> 
#include <numeric>
#include <iostream>



namespace ex{


	/*13.10 Write a function in C called my2DAIIoc which allocates a two-dimensional array.
	Minimize the number of calls to malloc and make sure that the memory is accessible
	by the notation arr[i][j].*/
	void **malloc2D(size_t fd, size_t sd, size_t numBytesPerElement){
		void **ptr;
		ptr = (void**)malloc(fd*sizeof(void*));
		for (size_t i = 0; i < fd; ++i)
			ptr[i] = malloc(sd*numBytesPerElement);

		return ptr;
	}
	void **malloc2DContiguous(size_t fd, size_t sd, size_t numBytesPerElement){
		int headerSize = fd * sizeof(void*);
		int dataSize = sd*numBytesPerElement;

		void **ptr = (void**) malloc(headerSize + dataSize);

		for (size_t i = 0; i < fd; ++i){
			ptr[i] = (char*)(headerSize + dataSize*i);
		}
		return ptr;
	}


	/*13.9 Write an aligned malloc and free function that supports allocating memory such
	that the memory address returned is divisible by a specific power of two*/
	size_t nextPow2(size_t n){
		n--;
		n |= n >> 1; 
		n |= n >> 2;   // and then or the results.
		n |= n >> 4;
		n |= n >> 8;
		n |= n >> 16;
		n |= n >> 32;
		n++;
		return n;
	}

	void *alignedMalloc(size_t requiredBytes, size_t alignment){
		int offset = alignment - 1 + sizeof(void*);

		void *p1 = malloc(requiredBytes + offset);
		
		if (p1 == nullptr)
			return nullptr;

		/*p2 will start at a memory location exactly divisible by 'alignment'*/
		void **p2 = (void**) (( (size_t)(p1)+offset) & ~(alignment - 1));
		/*Storing p1 right before first memory position (to latter be able to free it)*/
		p2[-1] = p1;
		return p2;
	}
	void alignedFree(void *ptr){
		void *p = ((void**)ptr)[-1];
		free(p);
	}

	//void* aligned_malloc(size_t required_byteSj size_t alignment) {
	//	void* pi; // original block
	//	void** p2; // aligned block

	//	int offset = alignment - 1 + sizeof(void*);
	//	if ((pi = (void*)malloc(required_bytes + offset)) == NULL) {
	//		return NULL;

	//	}
	//	p2 = (void**)(((size_t)(pl)+offset) & -(alignment - 1));
	//	P2[-l] = pi;
	//	return p2
	//}

	/*13.8 Write a smart pointer class. A smart pointer is a data type, usually implemented with
	templates, that simulates a pointer while also providing automatic garbage collection.
	It automatically counts the number of references to a SmartPointer<T*>
	object and frees the object of type T when the reference count hits zero.*/
	template <class T> class SmartPtr{
		
	public:

		friend class SmartPtr<T>;

		SmartPtr(T *ptr){
			if (!numInstances)
				numInstances = new std::uint32_t(); //The () operator inits the value to 0
			this->ptr = ptr;
			++(*numInstances);
		}
		SmartPtr(SmartPtr &ptr){
			this->numInstances = ptr.numInstances;
			++(*this->numInstances);
			this->ptr = ptr.ptr;
		}

		~SmartPtr(){
			--(*numInstances);
			if (*numInstances == 0){
				delete ptr;
				delete numInstances;
			}
		}

		T *operator->(){
			return ptr;
		}

	private:
		T *ptr = nullptr;
		std::uint32_t *numInstances = nullptr;
	};


	void printLastKLines(std::ifstream &file, int k)
	{
		
		if (!file) return;

		std::list<std::uint32_t> lineLens;

		/*Count number of characters in each line (so we may skeep them later fastly)*/
		while(!file.eof()){
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			lineLens.push_back(file.gcount() + 1); //We add 1 so we count the linebreaks
		}
		
		/*Count number of character we have to skip*/
		size_t totalCharsToIgnore = 0;
		auto it = lineLens.begin();
		for (int i = 0; i < lineLens.size() - k; ++i){
			totalCharsToIgnore += *it++;
		}

		/*Clear error flags caused by first while.*/
		file.clear(); 
		/*Skeep the characters so we reach the first line to be printed*/
		file.seekg(totalCharsToIgnore, file.beg);
		
		/*Prints the last k lines*/
		std::string str;
		for (int i = 0; i < k; ++i){
			std::getline(file, str);
			std::cout << str << std::endl;
		}
	}
	
	/*13.7 Write a method that takes a pointer to a Node structure as a parameter and
	returns a complete copy of the passed in data structure. The Node data structure
	contains two pointers to other Nodes.*/

};