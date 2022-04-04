#pragma once

class KeywordsCollection {
public: 
	char** words;
	DWORD size;
	DWORD capacity;
	DWORD longestKeyword;

	void resize();
	KeywordsCollection();
	DWORD populateKeywords(char* fileName);
	void addKeyword(Keyword keyword);
};
