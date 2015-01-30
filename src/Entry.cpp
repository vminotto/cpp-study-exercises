#include "Entry.h"
#include "Directory.h"

void ex::Entry::setParent(Directory *parent){
	this->parent = parent;
	this->entryPath = parent->entryPath + "/" + this->entryName;
	lastAccessDate = today();
}
void ex::Entry::moveTo(Directory *parent){
	this->parent->removeEntry(this);
	this->setParent(parent);
}