#pragma once

class Keyword {
public:
//private:
	DWORD hits;
	DWORD length;
	char* word;

//public:
	Keyword();
	Keyword(char* keyword);
	Keyword& operator=(const Keyword& other);
	Keyword(const Keyword& other);
	~Keyword();
};
