#include "pch.h"

Search::Search(DWORD b, DWORD l) : b(b), l(l), activeThreads(0) {
	InitializeCriticalSection(&cs);
}

void Search::Run() {

	char* match;

	while (1) {
		
		SearchBuffer buf;
		if (pcFull->Consume((char*)&buf) == QUIT) return;
		InterlockedIncrement(&activeThreads);
		DWORD slotID = buf.slotID;

		// Perform the search
		for (int i = 0; i < keywords->size; i++) {
			match = strstr(buf.ptr, keywords->words[i].word);
			while (match) {
				keywords->words[i].hits++;

				if (strlen(match) <= l) break;

				match = strstr(match + keywords->words[i].length, keywords->words[i].word);
			}
		}

		printf("Done searching block\n");

		if (pcEmpty->Produce((char*)&slotID) == QUIT) return;

		EnterCriticalSection(&cs);
		if (--activeThreads == 0) {
			printf("active threads is 0\n");
			if (pcFull->diskDone) {
				printf("disk is done, pcFull size: %d\n",pcFull->size);
				if (pcFull->size == 0) {
					SetEvent(pcFull->eventQuit);
					SetEvent(pcEmpty->eventQuit);
					printf("Setting event\n");
					LeaveCriticalSection(&cs);
					return;
				}
			}
		}
		LeaveCriticalSection(&cs);


	}

}
