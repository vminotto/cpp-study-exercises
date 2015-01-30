#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>
#include <list>
#include <functional> 
#include <set>

namespace ex{

	inline long today(){ return 0; /*returns current date as a long*/ }

	class Directory;

	class Entry{
	
	public:

		Entry(std::string name, bool hiden, bool readOnly){
			this->entryName = name;
			this->parent = parent;
			this->hiden = hiden;
			this->readOnly = readOnly;
			creationDate = lastChangeDate = lastAccessDate = today();
		}
		~Entry(){}
				
		//Using 'parent->name + this->name' we may have the path of this entry
		std::string entryName;
		std::string entryPath;

		long creationDate;
		long lastChangeDate;
		long lastAccessDate;

		bool readOnly;
		bool hiden;

		Directory *getParent(){	return parent;}
		void setParent(Directory *parent);
		void moveTo(Directory *parent);

	protected:

		Directory *parent;

	};

};

#endif