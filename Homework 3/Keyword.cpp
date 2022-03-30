#include "pch.h"

Keyword::Keyword() {
	word = nullptr;
	length = 0;
	hits = 0;
}

Keyword::Keyword(char* keyword) {

	length = strlen(keyword) - 2;
	hits = 0;
	word = new char[length + 1];

	memcpy(word, keyword, length);
	word[length] = NULL;

}

Keyword& Keyword::operator=(const Keyword& other) {

	if (this != &other) {
		length = other.length;

		word = new char[length + 1];

		memcpy(word, other.word, length + 1);
	}

	return *this;
	
}

Keyword::~Keyword() {
	//printf("Destructor called\n");
	//delete[] word;
}
