#pragma once

class KeywordsCollection {
private: 
	Keyword* words;
	DWORD size;
	DWORD capacity;

	void resize();
public: 
	KeywordsCollection();
	void populateKeywords(char* fileName);
	void addKeyword(Keyword keyword);
};
