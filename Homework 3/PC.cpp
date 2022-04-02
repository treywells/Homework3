#include "pch.h"

PC::PC(HANDLE eventQuit, DWORD slots, DWORD sizeOfObject) : eventQuit(eventQuit), slots(slots), sizeOfObject(sizeOfObject) {

	InitializeCriticalSection(&cs);

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

	empty[0] = eventQuit;
	empty[1] = semaEmptySlots;

	full[0] = eventQuit;
	full[1] = semaFullSlots;

}

DWORD PC::Produce(char* source) {

	if (WaitForMultipleObjects(2, empty, false, INFINITE) == WAIT_OBJECT_0) {
		return QUIT;
	}

	EnterCriticalSection(&cs);

	memcpy(buf + tail, source, sizeOfObject);
	tail = (tail + sizeOfObject) % allocatedSpace;
	size++;

	LeaveCriticalSection(&cs);

	if (!ReleaseSemaphore(semaFullSlots, 1, NULL)) {
		printf("Releasing the semaphore in Produce, failed\n");
		exit(-1);
	}

	return 1;

}

DWORD PC::Consume(char* destination) {

	if (WaitForMultipleObjects(2, full, false, INFINITE) == WAIT_OBJECT_0) {
		return QUIT;
	}

	EnterCriticalSection(&cs);

	memcpy(destination, buf + head, sizeOfObject);
	head = (head + sizeOfObject) % allocatedSpace;
	size--;

	LeaveCriticalSection(&cs);

	if (!ReleaseSemaphore(semaEmptySlots, 1, NULL)) {
		printf("Releasing the semaphore in Consume, failed\n");
		exit(-1);
	}

	return 1;


}
