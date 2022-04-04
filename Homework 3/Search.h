#pragma once

class Search {
public:
	PC* pcFull;
	PC* pcEmpty;
	KeywordsCollection* keywords;
	DWORD b, l;
	DWORD totalHits;
	DWORD activeThreads;
	CRITICAL_SECTION cs;

	Search(DWORD b, DWORD l);
	void Run();
};
