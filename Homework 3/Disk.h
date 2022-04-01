#pragma once

class Disk {
	HANDLE file;
	HANDLE eventQuit;
	PC* pcEmpty;
	PC* pcFull;
	char* buf;
	DWORD amountRead;

public:
	Disk(DWORD threads, DWORD longestWord, char* fileName);
	~Disk();
};
