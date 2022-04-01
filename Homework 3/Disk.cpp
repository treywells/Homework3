#include "pch.h"

Disk::Disk(HANDLE eventQuit, PC* pcEmpty, PC* pcFull, DWORD bufSize, char* fileName) : eventQuit(eventQuit), pcEmpty(pcEmpty), pcFull(pcFull), bufSize(bufSize) {

	buf = new char[bufSize];

	if ((file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
		printf("Opening %s failed\n", fileName);
		exit(-1);
	}

	amountRead = 0;

}

Disk::~Disk() {
	delete[] buf;
}


