#include "pch.h"

Search::Search(DWORD b, DWORD l) : b(b), l(l) {}

void Search::Run() {

	while (1) {
		
		SearchBuffer* buf;
		if (pcFull->Consume((char*)buf) == QUIT) return;
		DWORD slotID = buf->slotID;

		// Perform the search


		if (pcEmpty->Produce((char*)&slotID) == QUIT) return;

	}

}
