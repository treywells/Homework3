#include "pch.h"

PC::PC(HANDLE eventQuit, DWORD slots, DWORD sizeOfObject) : eventQuit(eventQuit), slots(slots), sizeOfObject(sizeOfObject) {

	if ((mutex = CreateMutex(NULL,false, NULL)) == NULL) {
		printf("Creation the Mutex failed\n");
		exit(-1);
	}

	if ((semaFullSlots = CreateSemaphore(NULL, 0, slots, NULL)) == NULL) {
		printf("Creation the semaFullSlots failed\n");
		exit(-1);
	}

	if ((semaEmptySlots = CreateSemaphore(NULL, slots, slots, NULL)) == NULL) {
		printf("Creation the semaFullSlots failed\n");
		exit(-1);
	}

	allocatedSpace = slots * sizeOfObject;
	buf = new char[allocatedSpace];
	tail = 0;
	head = 0;
	size = 0;

}

void PC::Produce(char* source) {

	memcpy(buf + tail, source, sizeOfObject);
	tail = (tail + sizeOfObject) % allocatedSpace;
	size++;

}

void PC::Consume(char* destination) {

	memcpy(destination, buf + head, sizeOfObject);
	head = (head + sizeOfObject) % allocatedSpace;
	size++;

}
