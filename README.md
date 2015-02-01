# cpp-study-exercises
All the source code in this repository was developed for purposes of studying and exercising my C++ skills. It covers topics such as: arrays, strings, scalability, math, probability, recursion, dynamic programming, object orientation, and data structures in general (stack, queue, tree, graph, list, hash table etc.). Exercises were taken from the book "Cracking the Coding Interview (5th Edition)". Please feel free to use this a reference or copy any code to your convenience. Keep in mind
this project is not maintained often.

This was developed using Visual Studio 2013, and each folder contains a project referring to a chapter of the book. Loading the .sln should be enough to organize the project tree. If you are not a VS user, each main file should run without problems. The number suffix on each main file represents the chapter of the book it is related to. For ex.: 'main7.cpp' will run the exercises for the seventh chapter 'Math and Probability'.

Below is a small reference of what you will find in this code, and that I believe might be useful as a base for contrasting to your own code.

* BigInt class. 
  * Implemented using base 10 numerical system, for sake of readability and testing. 
  * Easely adaptable to higher bases.
  * Addition, subtraction, multiplication and division implemented.

* BinaryTree class. 
  * Depth First pre, post and in order traversals. 
  * Breadth first left-to-right and right-to-left traversals. 
  * Pretty print function.
  * for_each function (using any of the above traversals).
  * Function for finding path between two nodes.

* Random Generator class.
 * Class that encapsulates all of STL's unintuitive structures, and offers a simple interface for generating random numbers.
 * Produces a single random value or a sequence of N random ones.
 * Overriden for most numeric formats (double, char, short, long long), and also strings.
 * Able to reset the seed of the RandomGenerator object.
 * Able to set a min and max range for the random numbers.

* Directed and Undirected Graph classes.
 * Implemented using adjecency list.
 * BFS and DFS (pre and post) functions.
 * Find all paths between two nodes.
 * Find smallest path between two nodes.
 
* List class (doubly linked)
 * Insert, find, erase, push and pop (back and front).
 * Bidirectional iterator class (with begin(), end(), rbegin() and rend() functions), making the List useable with all STL functions.
 * Swap nodes function,
 * Invert list function.

* Hash Table class.
 * std::vector as main underlying structure, and std::list for extranal chaining (to avoid colisions).
 * Self adjustable load factor.
 * Bidirectional iterator classes implemented, making the container useable against most STL functions.
 * std::hash is used for hashing the key values.

* Stack, queue, and heap classes.
 * All element access, insertion and deletion function implemented (push, pop, and front or back).

* Sorting functions.
 * Heap, buble, quick, merge, and radix.

Some other stuff, such as bit manipulation, recursion, dynamic progrmaming and memory handling.
