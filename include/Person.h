#ifndef SOCIAL_NETWORK_PERSON_H
#define SOCIAL_NETWORK_PERSON_H

#include <list>
#include <string>
#include <cstdint>
#include <unordered_map>

namespace ex{

	using namespace std;

	/*10.2 How would you design the data structures for a very large social network like Facebook
	or Linked/n? Describe how you would design an algorithm to show the connection,
	or path, between two people (e.g., Me -> Bob -> Susan -> Jason -> You).

	Any connection may be found by traversing the tree of connections (friends) a given Person has. If the connection
	is found in the first level of the tree, it means they are direct friends. If it is found in a deeper level, this means
	the target Person is the friend of a friend of a friend... of the initial Person. For this reason, a breadth-first search
	is more efficient.
	*/

	/*This class is equivalent to a node of a graph. It would work in a case where all the information of the social
	network is stored locally in a single machine. In case of Facebook or Twitter, e.g., we know this would not be possible.*/
	class PersonLocal{

	public:
		PersonLocal() = default;
		~PersonLocal() = default;

		std::list<PersonLocal> friends;

		/*Auxiliary variable used for traversing this node.*/
		bool visited = false;

	private:
		/*Miscelaneous attributes*/
		std::string relationshipStatus;
		unsigned char age;
		std::string realName;
		std::string profileName;
		/*...*/
	};



	/*The following implementation shows a prototype of the above case, but in a distributed manner.*/
	class PersonDist{
	public:
		PersonDist(uint64_t id) : id(id){}
		~PersonDist() = default;

		uint64_t getId(){return id;}
		std::list<uint64_t> friends;

	private:

		uint64_t id;

		/*Miscelaneous attributes*/
		std::string relationshipStatus;
		unsigned char age;
		std::string realName;
		std::string profileName;
		/*...*/
	};

	class Machine{
	
	public:
		
		PersonDist* getPersonById(uint64_t userId){
			auto it = personById.find(userId);
			return it == personById.end() ? nullptr : it->second;
		}

	private:
		
		/*We could have simply used an std::list of PersonDist*  instead of a HashTable,
		but performing lookups in HashTables is more efficient*/
		std::unordered_map<uint64_t, PersonDist*> personById;
		std::string ip;

	};

	class Server{
		
	public:
	
		Machine* getMachineOfUser(uint64_t userId){
			auto it = machineByUserId.find(userId);
			return it == machineByUserId.end() ? nullptr : it->second;
		}
		
	private:
		/*Maps from Person::id to Machine*/
		std::unordered_map<uint64_t, Machine*> machineByUserId;
	};

	class ConnectionFinder{
	
	public:
		ConnectionFinder(Server *server) : server(server){}
		~ConnectionFinder() = default;

		std::list<PersonDist> *getConnection(uint64_t fromId, uint64_t toId){
			
			Machine *m = server->getMachineOfUser(fromId);
			/*... estabilish connection to the machine*/
			PersonDist *basePerson = m->getPersonById(fromId);

			if (basePerson->friends.size() <= 0)
				return nullptr;

			PersonDist *p;

			for (uint64_t i = 1; i < basePerson->friends.size(); i++){

			}
		
		}

	private:
		Server *server;

	};

};


#endif