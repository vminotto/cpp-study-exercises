#ifndef DIRECTORY_H
#define DIRECOTYR_H

#include "Entry.h"
#include <memory> 

namespace ex{

	class Directory : public Entry{

	public:
		Directory(std::string name, bool hiden = false, bool readOnly = false)
			: Entry(name, hiden, readOnly)
		{}
		~Directory();
		void removeEntry(Entry *entry);
		void addEntry(Entry *entry);

		/*Methods to return the list of entries (files and directories) inside this directored
		sorted by some particular criteria. Lambda functiosn could be used to accomplish such
		behavior.*/
		std::shared_ptr<std::list<Entry*>> getSortedBy(std::function<bool(Entry *l, Entry *r)> criterium);

		bool isEmpty(){ return entries.empty(); }
		size_t numberOfEntries(){ return entries.size(); }

	private:
		std::set<Entry*> entries;

	};

};

#endif