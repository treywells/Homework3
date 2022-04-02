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
	DWORD amountRead;

	Disk(DWORD slots, DWORD longestWord, char* fileName);
	void Run();
	~Disk();
};
