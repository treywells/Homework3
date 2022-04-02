#pragma once

class PC {
	char* buf;
	DWORD head;
	DWORD tail;
	DWORD sizeOfObject;
	DWORD size;
	DWORD slots;
	DWORD allocatedSpace;
	HANDLE eventQuit;
	HANDLE semaFullSlots;
	HANDLE semaEmptySlots;
	CRITICAL_SECTION cs;
	HANDLE empty[2];
	HANDLE full[2];

public:
	PC(HANDLE eventQuit, DWORD slots, DWORD sizeOfObject);
	DWORD Produce(char* source);
	DWORD Consume(char* destination);
};