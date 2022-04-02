#pragma once

class Search {
public:
	PC* pcFull;
	PC* pcEmpty;
	KeywordsCollection* keywords;
	DWORD b, l;

	Search(DWORD b, DWORD l);
	void Run();
};
