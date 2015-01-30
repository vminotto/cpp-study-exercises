#ifndef FILE_H
#define FILE_H

#include "Entry.h"

namespace ex{

	template <class T> class File : Entry{

	public:
		File(T* data, std::string name, bool hiden = false, bool readOnly = false)
			: Entry(name, hiden, readOnly)
		{
			this->data = data;
		}
		~File(){
			delete data;
		}

	protected:
		T* data;
		size_t size; // in bytes
	};

};

#endif