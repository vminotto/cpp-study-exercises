#include "Directory.h"

ex::Directory::~Directory(){
	/*When a directory is deleted, we also delete all of its contents. Since we store
	pointers to such content, we must manually call the destructor of each.*/
	for (auto el : entries){
		delete el;
	}
	/*The 'entries' container itself will be deleted when this class goes out of scope.*/
}

void ex::Directory::removeEntry(ex::Entry *entry){
	entries.erase(entry);
}
void ex::Directory::addEntry(ex::Entry *entry){
	/*we use a set to avoid duplicate insertions.
	In a real file-system we would browse the user for what to do
	with a duplicate insertion. Soem optiosn would: not insert,
	insert with an automatically generated different name, or replace
	original entry.*/
	entries.insert(entry);
	entry->setParent(this);
}

/*Methods to return the list of entries (files and directories) inside this directored
sorted by some particular criteria. Lambda functiosn could be used to accomplish such
behavior.*/
std::shared_ptr<std::list<ex::Entry*>> ex::Directory::getSortedBy(std::function<bool(ex::Entry *l, ex::Entry *r)> criterium){
	std::shared_ptr<std::list<ex::Entry*>> sortedList(new std::list<ex::Entry*>(entries.begin(), entries.end()));
	sortedList->sort(criterium);
	return sortedList;
}