#pragma once

class Keyword {
private:
	char* word;
	DWORD length;
	DWORD hits;

public:
	Keyword(char* name, DWORD bytes);
};
