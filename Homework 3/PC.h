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
	HANDLE mutex;

public:
	PC(HANDLE eventQuit, DWORD slots, DWORD sizeOfObject);
	void Produce(char* source);
	void Consume(char* destination);
};