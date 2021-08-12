#include <stddef.h>
#include <string.h>
#include "CORE_String.h"

#define DEFAULT_BUFFER_SIZE (256)

CoreString::CoreString(const int customSize) {
	int size;

	
	size = customSize > 0 ? customSize : DEFAULT_BUFFER_SIZE;

	this->buffer = new char[size];
	this->size = 0;
	this->max_size = size;
}

CoreString::~CoreString() {
	delete[] this->buffer;
}

void CoreString::Concat(const char *value) {
	const char *iter = value;
	while (1) {
		if (this->size == this->max_size) {
			this->max_size *= 2;
			delete[] this->buffer;
			this->buffer = new char[this->max_size];
		}
		
		this->buffer[this->size] = *iter;
		
		if (*iter == '\0') {
			break;
		}

		this->size++;
		iter++;
	}
}

char* CoreString::GetBuffer() {
	return this->buffer;
}

int CoreString::GetSize() {
	return this->size;
}
