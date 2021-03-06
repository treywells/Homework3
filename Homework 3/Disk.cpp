#include "pch.h"

Disk::Disk(DWORD slots, DWORD longestWord, DWORD b, char* fileName) : slots(slots), l(longestWord), b(b) {

    // Open with no buffering
	if ((file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL)) == NULL) {
		printf("Opening %s failed\n", fileName);
		exit(-1);
	}

    // Open with caching enabled
    /*if ((file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
        printf("Opening %s failed\n", fileName);
        exit(-1);
    }*/

	amountRead = 0;
    previousShadow = new char[l];
    memset(previousShadow, 0x20, l);

    DWORD sectorSize;

    GetDiskFreeSpace(NULL, NULL, &sectorSize, NULL, NULL);
    // align max string length to sector size, which gives us the first shadow buffer length;
    // the second shadow buffer only holds a NULL and can be limited to just sectorSize
    shadowSize = (l / sectorSize + 1) * sectorSize;
    int nSlots = slots; // how many slots to maintain
    int padding = shadowSize + sectorSize; // both shadow buffers
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

        if (bytesRead == 0) break;

        // Set the shadow buffers
        memcpy(buf + slotID * slotSize + shadowOffset, previousShadow, l);
        buf[slotID * slotSize + shadowSize + bytesRead] = NULL;
        memcpy(previousShadow, buf + slotID * slotSize + shadowSize + bytesRead - l, l);

        SearchBuffer searchBuf;
        searchBuf.ptr = buf + slotID * slotSize + shadowOffset;
        searchBuf.size = bytesRead + l;
        searchBuf.slotID = slotID;

        if (pcFull->Produce((char*)&searchBuf) == QUIT) return;

    } while (1);

    pcFull->diskDone = true;
    pcEmpty->diskDone = true;

}

Disk::~Disk() {
    delete[] previousShadow;
    HeapFree(buf, 0, NULL);
}


