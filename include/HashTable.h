#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <xhash>
#include <functional> 
#include <list>

namespace ex{
	
	/*Forward declaration/*/
	template <class KeyType, class DataType> class HashTable;

	template <class KeyType, class DataType> class HashTableIterator
		: public std::iterator<std::bidirectional_iterator_tag, DataType>
	{

	private:

		/*Old style type alias*/
		//typedef typename std::vector<std::list<std::pair<KeyType, DataType>>>::iterator ItVec;
		//typedef typename std::list<std::pair<KeyType, DataType>> ::iterator ItList;

		/*C++11 syntax for type alias (equivalent to the ' old' syntax above)*/		
		using ItVec  = typename std::vector<std::list<std::pair<KeyType, DataType>>>::iterator;
		using ItList = typename std::list<std::pair<KeyType, DataType>>::iterator;
		using Vector = std::vector<std::list<std::pair<KeyType, DataType>>>;

		Vector *vec = nullptr; //Pointer to the underlying data of the HashTable, in this case, an std::vector
		ItVec itVec;   //Position in the vector (bucket index)
		ItList itList; //Position in the external chaining list (anti collision list)

		friend class ex::HashTable<KeyType, DataType>;

		void advance(bool calledRecurseively = false){
			/*The first test checks if list is empty (begin == end).
			The second test checks if we are on the last element of the list.*/
			if (itList == itVec->end() || std::next(itList, 1) == itVec->end()){
				if (std::next(itVec, 1) != vec->end()){
					++itVec;
					itList = itVec->begin();
					if (itVec->size() > 1 || itVec->empty())
						this->advance(true);
				}
				else{
					itList = itVec->end();
				}
			}
			else if (!calledRecurseively)
				++itList;
		}
		void regress(){
			if (itVec->empty() || itList == itVec->begin()){
				
				--itVec;
				if (!itVec->empty())
					itList = --itVec->end();
				else
					regress();
			}
			else
				--itList;
		}

	public:

		HashTableIterator(const HashTableIterator<KeyType, DataType> &iterator) 
			: HashTableIterator(iterator.itVec, iterator.itList, iterator.vec) {}
		
		HashTableIterator(ItVec itVec, ItList itList, Vector *vec)
			: itVec(itVec), itList(itList), vec(vec){}
		
		HashTableIterator() = default;

		/*Pre-increment operators, as in ++myInstance and --myInstance*/
		HashTableIterator& operator++() { advance(); return *this; }
		HashTableIterator& operator--() { regress(); return *this; }

		/*Post-increment operators, as in myInstance++ and myInstance--
		The 'int' parameter is just a dummy variable the compiler uses interally
		to distinguish both operators overloads.*/
		HashTableIterator operator++(int) { HashTableIterator tempIter(*this); ++(*this); return tempIter; }
		HashTableIterator operator--(int) { HashTableIterator tempIter(*this); --(*this); return tempIter; }

		friend bool operator==(const HashTableIterator& lhs, const HashTableIterator& rhs){
			if (lhs.itVec == rhs.itVec){
				if (lhs.itList == rhs.itList){ /*If we compared this directly, we could get an incompatible iterator exception, given
										   iterators from two different list buckets could be compared*/
					return true;
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}
		}
		friend bool operator!=(const HashTableIterator& lhs, const HashTableIterator& rhs){
			if (lhs.itVec != rhs.itVec){
				return true;
			}
			else{
				if (lhs.itList != rhs.itList){
					return true;
				}
				else{
					return false;
				}
			}
		}

		//bool operator==(HashTableIterator& rhs) {
		//	return ex::operator== (*this, rhs);  //Calls the above friend operators
		//}
		//bool operator!=(HashTableIterator& rhs) {
		//	return ex::operator!= (*this, rhs);  //Calls the above friend operators
		//}

		DataType& operator*() { return   itList->second; }
		DataType* operator->(){ return &(itList->second); }

		KeyType key(){ return itList->first; }
	};


	/*The underlying container is an std::vector. We could make such container generic using a
	second template parameter for this class using an abstract interface as member (from which
	the container would have to inherit).
	
	Furthermore, for simplicity, we will force the "hash" and "equal" functions to be of type
	std::function<size_t(const T&)> and std::function<bool(const T&, const T&)> respectively.
	In case they are not provided, we default them to two lambdas. This is a very good option,
	given that std::function may be constructed as a lambda, function pointer or functor class.*/
	template <class KeyType, class DataType> class HashTable{

	public:
		HashTable(size_t initialNumBuckets,
			float increaseFactor,
			float maxLoadFactor,
			std::function<bool(const KeyType&, const KeyType&)> equal = [](const KeyType &a, const KeyType &b){return a == b; },
			std::function<size_t(const KeyType&)> hash = std::hash<KeyType>())
			: equal(equal), hash(hash){
			this->maxLoadFactor = std::max(maxLoadFactor,   2.0f);
			this->increaseFactor = std::max(increaseFactor, 1.0f);
			this->resize(initialNumBuckets);
			
		}

		HashTable(std::function<bool(const KeyType&, const KeyType&)> equal = [](const KeyType &a, const KeyType &b){return a == b; },
			std::function<size_t(const KeyType&)> hash = std::hash<KeyType>())
			: HashTable(32, 3.0f, 3.0f, equal, hash){}

		~HashTable(){}

		using HIter = ex::HashTableIterator<KeyType, DataType>;

		/*Modifiers*/
		std::pair<HIter, bool> insert(KeyType key, DataType data);
		HIter erase(KeyType key);
		HIter erase(HIter iter);
		//bool erase(KeyType key);
		void resize(size_t newSize);
		void clear(){
			for (auto &l : data)
				l.clear();
		}
		
		/*Element access or insertion*/
		DataType& operator[](KeyType &key);

		using RevIter = std::reverse_iterator<ex::HashTableIterator<KeyType , DataType>>;

		/*Iterators*/
		HIter begin(){
			std::list<std::pair<KeyType, DataType>> &list = data.front();
			HIter it(data.begin(), list.begin(), &data);
			if (list.empty())
				++it; //This will advance to the first valid element, that is, the actua 'begin' of the HashTable
			return it;
		}
		HIter end(){
			std::list<std::pair<KeyType, DataType>> &list = data.back();
			HIter it(--data.end(), list.end(), &data);
			if (list.empty()){
				--it; //This will regress to the last valid element, that is, the actual 'end-1' of the HashTable
				return ++it; //We increment to return the actual end of the has table, that is, the post-the-last element
			}
			else
				return it;
		}

		/*Here the std::reverse_iterator class provides an easy way of reversing an iterator*/
		RevIter rbegin(){ return RevIter(this->end()); }
		RevIter   rend(){ return RevIter(this->begin()); }

		size_t size(){
			size_t s = 0;
			for (auto &l : data)
				s += l.size();
			return s;
		}
		size_t numBuckets(){ 
			return data.size(); 
		}

	private:
		
		/*Data with external chaining (using std::list) to avoid colisions.
		Node: the buckets are not sorted, but the elements in each list are.*/
		std::vector<std::list<std::pair<KeyType, DataType>>> data;
		
		//size_t numBuckets   //this is equal to data.capacity()
		//size_t size;        //this is equal to data.size()

		/*The ratio at which the number of buckets will increase when a resize needs to be done.
		That is, newSize = size()*increaseFactor*maxLoadFactor */
		float increaseFactor;

		/*The load factor is the ratio between the number of elements in the container and the number of buckets, that is,
		size() / numBuckets(). If the actual load factor of the HashTable surpass this limit, a resize is forced.*/
		float maxLoadFactor;
		
		std::function<bool(const KeyType&, const KeyType&)> equal;
		std::function<size_t(const KeyType&)> hash;

		void reArrangeData();
		void checkForResize();
		size_t getIndex(KeyType key){ return hash(key) % numBuckets(); }
		std::pair<typename HIter::ItList, bool> insert(std::list<std::pair<KeyType, DataType>> &list, KeyType key, DataType data);
	};

};


template <class KeyType, class DataType> std::pair<ex::HashTableIterator<KeyType, DataType>, bool>
	ex::HashTable<KeyType, DataType>::insert(KeyType key, DataType data)
{
	this->checkForResize();

	size_t index = this->getIndex(key);

	/*Just insert at the list*/
	auto res = insert(this->data.at(index), key, data);
	ex::HashTableIterator<KeyType, DataType> it(this->data.begin() + index, res.first, &this->data);
	return std::pair<decltype(it), bool>(it, res.second);
	
}

template <class KeyType, class DataType> std::pair<typename ex::HashTableIterator<KeyType, DataType>::ItList, bool>
	ex::HashTable<KeyType, DataType>::insert(std::list<std::pair<KeyType, DataType>> &list, KeyType key, DataType data)
	{

		if (list.empty()){
			list.push_back(std::pair<KeyType, DataType>(key, data));
			auto it = list.begin();
			return std::pair<decltype(it), bool>(it, true);
		}
		else{
			auto pos = std::lower_bound(list.begin(), list.end(), key,
				[](const std::pair<KeyType, DataType> &a, const KeyType &b){
				return a.first < b;
			});

			if (pos != list.end() && equal((*pos).first, key)) /*Duplicate key found, do not insert and return false*/
				return std::pair<decltype(pos), bool>(pos, false);
			else{ /*Insert at correct pos, thus keeping list ordered*/
				pos = list.insert(pos, std::pair<KeyType, DataType>(key, data));
				return std::pair<decltype(pos), bool>(pos, true);
			}
		}
}

template <class KeyType, class DataType>
void ex::HashTable<KeyType, DataType>::checkForResize(){
	float loadFactor = size() / (float)numBuckets();
	if (loadFactor > maxLoadFactor)
		this->resize(maxLoadFactor * increaseFactor * numBuckets());
}

template <class KeyType, class DataType>
void ex::HashTable<KeyType, DataType>::resize(size_t newSize)
{
	if (newSize > data.size()){
		data.resize(newSize);
		this->reArrangeData();
	}
}

template <class KeyType, class DataType>
void ex::HashTable<KeyType, DataType>::reArrangeData(){
	
	std::list<std::pair<KeyType, DataType>> allElements;
	
	/*Transfer all elements of this HashTable to a temporary list*/
	for (auto &_list : data){
		allElements.splice(allElements.begin(), _list);
	}

	/*Re-insert all elements into their correct buckets*/
	for (auto &_pair : allElements){
		this->insert(_pair.first, _pair.second);
	}
}

template <class KeyType, class DataType>
DataType& ex::HashTable<KeyType, DataType>::operator[](KeyType &key){
	auto res = this->insert(key, DataType());
	return *res.first;
}

template <class KeyType, class DataType> ex::HashTableIterator<KeyType, DataType> 
	ex::HashTable<KeyType, DataType>::erase(KeyType key)
{
	size_t index = this->getIndex(key);
	if (index < data.size()){
		auto &l = data.at(index);
		for (auto it = l.begin(); it != l.end(); ++it){
			if (it->first == key){
				ex::HashTableIterator<KeyType, DataType> nextPos(data.begin() + index, it, &data);
				++nextPos;
				l.erase(it);
				return nextPos;
			}
		}
	}
	return this->end();
}
template <class KeyType, class DataType> ex::HashTableIterator<KeyType, DataType>
	ex::HashTable<KeyType, DataType>::erase(HIter iter)
{
	return erase(iter.key());
}

#endif