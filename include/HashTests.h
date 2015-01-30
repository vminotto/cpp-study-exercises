#ifndef HASH_TESTS_H
#define HASH_TESTS_H

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace ex{

	using uint = unsigned int;

	const uint maxHashValues = 5;

	/*Hash function that purposely generates collisions*/
	size_t weakHash(uint a){
		size_t temp = a % maxHashValues;
		return temp;
	}

	bool weakEqual(uint a, uint b){
		uint l = a % maxHashValues;
		uint r = b % maxHashValues;
		return l == r;
	}


	/*Note we test with an std::unordered_set because it uses hash for mapping,
	while an std::set does not (it uses just the 'less' comprator). This is also 
	the reason why std::unoredered_set is unsorted. It uses the hash key for indexing
	which has no guarantee of outputting sorted values according to their inputs.*/

	/*Uses a bad hash function that generates collisions and a good equal function*/
	void testBadHashGoodEqual(){
		
		std::unordered_set<uint, std::function<size_t(uint)>> seta(0, ex::weakHash);
		
		using it = std::unordered_set<uint, std::function<size_t(uint)>>::iterator;
		
		for (uint i = 0; i < 116; ++i){
			std::pair<it, bool> res = seta.insert(i);
		}
	}

	/*Uses a bad equal function and a bad hash function*/
	void testBadHashBadEqual(){
		std::unordered_set<uint, std::function<size_t(uint)>, std::function<bool(uint, uint)>> seta(0, weakHash, weakEqual);
	
		using it = std::unordered_set<uint, std::function<size_t(uint)>>::iterator;

		for (uint i = 0; i < 116; ++i){
			std::pair<it, bool> res = seta.insert(i);
		}
	}

	/*Uses a good hash function and a bad equal function*/
	void testGoodHashBadEqual(){
		std::unordered_set<uint, std::hash<uint>, std::function<bool(uint, uint)>> seta(0, std::hash<uint>(), weakEqual);

		using it = std::unordered_set<uint, std::function<size_t(uint)>>::iterator;

		for (uint i = 0; i < 116; ++i){
			std::pair<it, bool> res = seta.insert(i);
		}
	}

};



#endif