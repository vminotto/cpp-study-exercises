#ifndef ANIMAL_SHELTER_H
#define ANIMAL_SHELTER_H

#include <list>
#include <cstdint>
#include <algorithm>

/*3.7 An animal shelter holds only dogs and cats, and operates on a strictly "first in,
first out" basis. People must adopt either the "oldest" (based on arrival time) of
all animals at the shelter, or they can select whether they would prefer a dog or
a cat (and will receive the oldest animal of that type). They cannot select which
specificanimal they would like. Create the data structures to maintain this system
and implement operations such as enqueue, dequeueAny, dequeueDog and
dequeueCat.You may use the built-in LinkedList data structure.*/

/*Holding an ID of the animals (representing the number of animals before them), is a way
to achieve O(1) in any dequeue operation.*/

namespace ex{
	
	/*We could have classes Dog and Cat derived from animal, but this would
	be an overkill for this example.*/
	struct Animal{
		enum class AnimalType : std::uint8_t{
			Cat,
			Dog
		};

		Animal(AnimalType type) :type(type){}

		std::uint64_t numBeforeMe = 0;

		AnimalType type;
		/*This could be a possible field, e.g. We will keep it commented though.*/
		//std::string animalName;
	};

	class AnimalShelter{
		
	public:

		void enqueue(Animal a){
			a.numBeforeMe = numAnimalsAdded++;
			switch (a.type){
				case Animal::AnimalType::Cat: catList.push_back(a); break;
				case Animal::AnimalType::Dog: dogList.push_back(a); break;
			}
		}
		Animal dequeueDog(){
			if (dogList.empty())
				throw "Error! This shelter has no more dogs!!!\n"; //Error message to be changed in real scenario
			else{
				Animal dog = dogList.front();
				dogList.pop_front();
				return dog;
			}
		}
		Animal dequeueCat(){
			if (catList.empty())
				throw "Error! This shelter has no more dogs!!!\n"; //Error message to be changed in real scenario
			else{
				Animal cat = catList.front();
				catList.pop_front();
				return cat;
			}
		}
		Animal dequeueAny(){
			if (catList.empty() && dogList.empty())
				throw "Shelter is completely empty. No cats. No dogs!\n";
			else if (catList.empty())
				return dequeueDog();
			else if (dogList.empty())
				return dequeueCat();

			if (catList.front().numBeforeMe < dogList.front().numBeforeMe)
				return dequeueCat();
			else
				return dequeueDog();
		}

	private:

		std::uint64_t numAnimalsAdded = 0;

		std::list<Animal> catList;
		std::list<Animal> dogList;
	};

		
};

#endif