#include "pch.h"

Search::Search(DWORD b, DWORD l) : b(b), l(l), activeThreads(0) {
	totalHits = 0;
	InitializeCriticalSection(&cs);
}

void Search::Run() {

	char* match;
	int i = 1;

	while (1) {
		
		SearchBuffer buf;
		if (pcFull->Consume((char*)&buf) == QUIT) return;
		InterlockedIncrement(&activeThreads);
		DWORD slotID = buf.slotID;

		// Perform the search
		for (int i = 0; i < keywords->size; i++) {
			match = strstr(buf.ptr, keywords->words[i]);
			while (match) {
				if (strlen(match) <= l) break;

				InterlockedIncrement(&(keywords->hits[i]));
				InterlockedIncrement(&totalHits);

				match = strstr(match + strlen(keywords->words[i]), keywords->words[i]);
			}
		}

		if (pcEmpty->Produce((char*)&slotID) == QUIT) return;

		EnterCriticalSection(&cs);
		if (--activeThreads == 0 && pcFull->diskDone && pcFull->size == 0) {
			SetEvent(pcFull->eventQuit);
			LeaveCriticalSection(&cs);
			return;
		}
		LeaveCriticalSection(&cs);
		i++;


	}

}
