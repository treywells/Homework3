#pragma once

class Disk {
	HANDLE file;
	HANDLE eventQuit;
	PC* pcEmpty;
	PC* pcFull;
	char* buf;
	DWORD bufSize;
	DWORD amountRead;

public:
	Disk(HANDLE eventQuit, PC* pcEmpty, PC* pcFull, DWORD bufSize, char* fileName);
	~Disk();
};
