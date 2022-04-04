#include "pch.h"

void addCommas(DWORD num, char* buffer) {

    memset(buffer, 0x00, 14);
    DWORD digits = 1;
    DWORD tempNum = num;
    while (tempNum >= 10) {
        tempNum /= 10;
        digits++;
    }
    DWORD commas = (digits - 1) / 3;
    DWORD totalCharacters = digits + commas;
    DWORD iterations = 0;
    for (int i = totalCharacters - 1; i > -1; i--) {
        iterations++;
        buffer[i] = '0' + (num % 10);
        num /= 10;
        if ((iterations % 3) == 0 && i != 0) {
            i--;
            buffer[i] = ',';
        }
    }

    buffer[totalCharacters] = '\0';
}

int main(int argc, char** argv)
{

    CPU cpu;
    DWORD threads = cpu.cpus;
    //DWORD threads = 1;

    DWORD numOfCmdArgs = 3;

    // Check if right number of command line args 
    if (argc != numOfCmdArgs) {
        printf("Incorrect amount of commandline arguments, %d are needed\n", numOfCmdArgs);
        exit(-1);
    }

    char* keywordFileName = argv[1];
    char* wikiFileName = argv[2];

    KeywordsCollection keywords;
    DWORD l = keywords.populateKeywords(keywordFileName);

    DWORD nSlots = threads * 2;
    DWORD b = 1 * (1 << 20);

    Disk disk(nSlots, l, b, wikiFileName);

    HANDLE eventQuit;
    if ((eventQuit = CreateEvent(NULL, true, false, NULL)) == NULL) {
        printf("Creation of eventQuit failed\n");
        exit(-1);
    }
    PC pcEmpty(eventQuit, nSlots, sizeof(DWORD));
    PC pcFull(eventQuit, nSlots, sizeof(SearchBuffer));

    disk.eventQuit = eventQuit;
    disk.pcFull = &pcFull;
    disk.pcEmpty = &pcEmpty;

    Search search(b, l);
    search.pcEmpty = &pcEmpty;
    search.pcFull = &pcFull;
    search.keywords = &keywords;

    HANDLE diskThread;
    HANDLE* searchThreads = new HANDLE[threads];

    if ((diskThread = CreateThread(NULL, 0, DiskThreadStarter, &disk, 0, NULL)) == NULL) {
        printf("Creation of disk thread failed\n");
        exit(-1);
    }

    if (!SetThreadPriority(diskThread, 2)) {
        printf("Failed to set disk thread priority\n");
        exit(-1);
    }

    for (int i = 0; i < threads; i++) {
        if ((searchThreads[i] = CreateThread(NULL, 0, SearchThreadStarter, &search, 0, NULL)) == NULL) {
            printf("Creation of search thread failed\n");
            exit(-1);
        }

        if (!SetThreadAffinityMask(searchThreads[i], 1 << (i % threads))) {
            printf("Failed to set thread affinity mask\n");
            exit(-1);
        }

        if (!SetThreadPriority(searchThreads[i], -15)) {
            printf("Failed to set search thread priority\n");
            exit(-1);
        }
    }


    WaitForSingleObject(diskThread, INFINITE);
    
    for (int i = 0; i < threads; i++) {
        WaitForSingleObject(searchThreads[i],INFINITE);
    }

    char commas[14];

    for (int i = 0; i < keywords.size; i++) {
        addCommas(keywords.hits[i], commas);
        printf("[%d] %s = %s\n", i, keywords.words[i], commas);
    }

    delete[] searchThreads;

}

