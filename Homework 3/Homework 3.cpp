#include "pch.h"

int main(int argc, char** argv)
{

    CPU cpu;
    DWORD threads = cpu.cpus;

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
    DWORD b = 1 << 20;

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

        SetThreadAffinityMask(searchThreads[i], i);

        if (!SetThreadPriority(searchThreads[i], -15)) {
            printf("Failed to set search thread priority\n");
            exit(-1);
        }
    }


    WaitForSingleObject(diskThread, INFINITE);
    
    for (int i = 0; i < threads; i++) {
        WaitForSingleObject(searchThreads[i],INFINITE);
    }

    for (int i = 0; i < keywords.size; i++) {
        printf("'%s' was found %d times\n", keywords.words[i].word, keywords.words[i].hits);
    }

}

