#include "pch.h"

KeywordsCollection::KeywordsCollection() {

    size = 0;
    capacity = 1;
    words = new Keyword[capacity];
    longestKeyword = 0;
}

void KeywordsCollection::resize() {
    DWORD newCapacity = capacity * 2;
    Keyword* newCollection = new Keyword[newCapacity];

    memcpy(newCollection, words, size * sizeof(Keyword));

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

        Keyword keyword(buf);

        if (keyword.length > longestKeyword) longestKeyword = keyword.length;

        addKeyword(keyword);
       
    }
    fclose(f);

    return longestKeyword;

}

void KeywordsCollection::addKeyword(Keyword keyword) {

    if (size == capacity) {
        resize();
    }

    words[size++] = keyword;
}
