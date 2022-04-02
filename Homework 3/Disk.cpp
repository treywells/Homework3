#include "pch.h"

Disk::Disk(DWORD slots, DWORD longestWord, char* fileName) : slots(slots), l(longestWord) {

	if ((file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
		printf("Opening %s failed\n", fileName);
		exit(-1);
	}

	amountRead = 0;
    previousShadow = new char[l];
    memset(previousShadow, 0x20, l);

    DWORD sectorSize;

    GetDiskFreeSpace(NULL, NULL, &sectorSize, NULL, NULL);
    // align max string length to sector size, which gives us the first shadow buffer length;
    // the second shadow buffer only holds a NULL and can be limited to just sectorSize
    shadowSize = (longestWord / sectorSize + 1) * sectorSize;
    int nSlots = slots; // how many slots to maintain
    int padding = shadowSize + sectorSize; // both shadow buffers
    b = 1 << 20; // 1MB in each slot
    slotSize = b + padding; // full slot with padding
    // VirtualAlloc guarantees page-aligned addresses, while the heap does not
    buf = (char*)VirtualAlloc(NULL, (UINT64)nSlots * slotSize,
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

}

void Disk::Run() {

    DWORD bytesRead;
    DWORD shadowOffset = shadowSize - l;

    for (DWORD i = 0; i < slots; i++) {
        pcEmpty->Produce((char*)&i);
    }

    do {

        int slotID;

        if (pcEmpty->Consume((char*)&slotID) == QUIT) return;

        if (!ReadFile(file, buf + slotID * slotSize + shadowSize, b, &bytesRead, NULL)) {
            printf("Error reading wiki file\n");
            exit(-1);
        }

        // Set the shadow buffers
        memcpy(buf + slotID * slotSize + shadowOffset, previousShadow, l);
        buf[slotID * slotSize + shadowSize + bytesRead] = NULL;
        memcpy(previousShadow, buf + slotID * slotSize + shadowSize + bytesRead - l, l);

        SearchBuffer searchBuf;
        searchBuf.ptr = buf + slotID * slotSize + shadowSize;
        searchBuf.size = bytesRead;
        searchBuf.slotID = slotID;

        if (pcFull->Produce((char*)&searchBuf) == QUIT) return;

    } while (bytesRead > 0);

}

Disk::~Disk() {
	delete[] buf;
}


