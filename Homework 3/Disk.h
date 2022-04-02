#pragma once

class Disk {
public:
	HANDLE file;
	HANDLE eventQuit;
	PC* pcEmpty;
	PC* pcFull;
	char* buf;
	DWORD slots;
	DWORD shadowSize;
	DWORD slotSize;
	DWORD b;
	DWORD l;
	char* previousShadow;
	DWORD amountRead;

	Disk(DWORD slots, DWORD longestWord, DWORD b, char* fileName);
	void Run();
	~Disk();
};
