#ifndef MULTI_ARRAY_STACK_H
#define MULTI_ARRAY_STACK_H

#include <array>

/*New syntax to substitute typedef*/
using uint = unsigned int;

namespace ex{

	template <class T = int, uint numStacks = 3, uint stackSize = 100>
	class MultiStacks{
		
	public:

		MultiStacks() = default;

		template <uint stackNum> void push(T element){			
			assertStackIndex<stackNum>();
			if (stacksPos[stackNum] >= (signed)stackSize - 1)
				throw "The stack is already full. Cannot push more elements to it.\n";

			stacks[stackNum][++stacksPos[stackNum]] = element;
		}
		template <uint stackNum> void pop(){
			assertStackIndex<stackNum>();
			if (stacksPos[stackNum] == -1)
				return;
			else
				stacks[stackNum][stacksPos[stackNum]--] = 0;
		}
		template <uint stackNum> T&   top(){
			assertStackIndex<stackNum>();
			if (stacksPos[stackNum] == -1) throw "Cannot access las element because the stack is empty.\n";
			else return stacks[stackNum][stacksPos[stackNum]];
		}
		template <uint stackNum> uint size(){
			assertStackIndex<stackNum>();
			return stacksPos[stackNum] + 1;
		}

		void push(int stackNum, T element){
			assertStackIndex(stackNum);
			if (stacksPos[stackNum] >= (signed)stackSize - 1)
				throw "The stack is already full. Cannot push more elements to it.\n";

			stacks[stackNum][++stacksPos[stackNum]] = element;
		}
		void pop(uint stackNum){
			assertStackIndex(stackNum);
			if (stacksPos[stackNum] == -1)
				return;
			else
				stacks[stackNum][stacksPos[stackNum]--] = 0;
		}
		T&   top(uint stackNum){
			assertStackIndex(stackNum);
			if (stacksPos[stackNum] == -1) throw "Cannot access last element because the stack is empty.\n";
			else return stacks[stackNum][stacksPos[stackNum]];
		}
		
		
		uint size (uint stackNum){
			assertStackIndex(stackNum);
			return stacksPos[stackNum] + 1;
		}


	private:

		template <uint stackNum> void assertStackIndex(){
			static_assert(stackNum < numStacks, "The 'stackNum' template argument is above its maximum supported value.");
			static_assert(stackNum >= 0, "The 'stackNum' template argument is below its minimum supported value.");
		}
		
		void assertStackIndex(uint stackNum){
			if (stackNum > numStacks) throw "The 'stackNum' argument is above its maximum supported value.\n";
			if (stackNum < 0) throw "The 'stackNum' argument is below its minimum supported value.\n";
		}

		/*Just typedef'ing nested std::arrays as a Matrix*/
		template <class T, size_t rows, size_t cols> using Matrix = std::array<std::array<T, cols>, rows>;
		/*Although this is declared as a matrix, it is allocated as an contiguous array,
		give the size of each dimension is known at compile time.*/
		Matrix<T, numStacks, stackSize> stacks;
		
		std::array<int, numStacks> stacksPos = std::array<int, numStacks>({ -1, -1, -1 });
		//std::array<uint, numStacks> actualSizes = std::array<uint, numStacks>({ 0, 0, 0 });
	};
	
};



#endif