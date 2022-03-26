#include "pch.h"

KeywordsCollection::KeywordsCollection(char* keywordFileName) {

    const DWORD BUF_SIZE = 50;
    char buf[BUF_SIZE];

    FILE* f = fopen(keywordFileName, "rb");
    while (!feof(f)) {
        // read one line at a time
        if (fgets(buf, BUF_SIZE, f) == NULL)
            break; // EOF or error

        // remove \r and \n from keywords
        DWORD bytes = strlen(buf);
        buf[bytes - 2] = '\0';
        char* newBuf = new char[bytes - 1];
        memcpy(newBuf, buf, bytes - 1);

        printf("%d, %s\n", bytes - 1, newBuf);
    }
    fclose(f);

}
