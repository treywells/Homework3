#include "pch.h"

KeywordsCollection::KeywordsCollection() {

    size = 0;
    capacity = 1;
    words = new char*[capacity];
    longestKeyword = 0;
}

void KeywordsCollection::resize() {
    DWORD newCapacity = capacity * 2;
    char** newCollection = new char*[newCapacity];

    memcpy(newCollection, words, size * sizeof(char*));

    delete[] words;
    words = newCollection;
    capacity = newCapacity;
}

DWORD KeywordsCollection::populateKeywords(char* fileName) {

    const DWORD BUF_SIZE = 1024;
    char buf[BUF_SIZE];

    FILE* f = fopen(fileName, "rb");
    while (!feof(f)) {
        // read one line at a time
        if (fgets(buf, BUF_SIZE, f) == NULL)
            break; // EOF or error

        if (size + 1 > capacity) resize();

        DWORD length = strlen(buf) - 2;

        words[size] = new char[length + 1];

        memcpy(words[size], buf, length);
        words[size][length] = NULL;

        if (length > longestKeyword) longestKeyword = length;

        size++;
       
    }
    fclose(f);

    hits = new DWORD[size];
    memset(hits, 0x00, size * sizeof(DWORD));

    return longestKeyword;

}

KeywordsCollection::~KeywordsCollection() {
    delete[] hits;
    for (int i = 0; i < size; i++) {
        delete[] words[i];
    }
    delete[] words;
}
