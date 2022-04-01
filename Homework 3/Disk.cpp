#include "pch.h"

Disk::Disk(DWORD threads, DWORD longestWord, char* fileName) {

	if ((file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
		printf("Opening %s failed\n", fileName);
		exit(-1);
	}

	amountRead = 0;

    DWORD sectorSize;

    GetDiskFreeSpace(NULL, NULL, &sectorSize, NULL, NULL);
    // align max string length to sector size, which gives us the first shadow buffer length;
    // the second shadow buffer only holds a NULL and can be limited to just sectorSize
    int shadowSize = (longestWord / sectorSize + 1) * sectorSize;
    int nSlots = threads; // how many slots to maintain
    int padding = shadowSize + sectorSize; // both shadow buffers
    int b = 1 << 20; // 1MB in each slot
    int slotSize = b + padding; // full slot with padding
    // VirtualAlloc guarantees page-aligned addresses, while the heap does not
    buf = (char*)VirtualAlloc(NULL, (UINT64)nSlots * slotSize,
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

}

Disk::~Disk() {
	delete[] buf;
}


