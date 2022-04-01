#include "pch.h"

KeywordsCollection::KeywordsCollection() {

    size = 0;
    capacity = 1;
    words = new Keyword[capacity];
}

void KeywordsCollection::resize() {
    //printf("Resizing\n");
    DWORD newCapacity = capacity * 2;
    Keyword* newCollection = new Keyword[newCapacity];

    memcpy(newCollection, words, size * sizeof(char));

    delete[] words;
    words = newCollection;
    capacity = newCapacity;
}

void KeywordsCollection::populateKeywords(char* fileName) {

    const DWORD BUF_SIZE = 1024;
    char buf[BUF_SIZE];

    FILE* f = fopen(fileName, "rb");
    while (!feof(f)) {
        // read one line at a time
        if (fgets(buf, BUF_SIZE, f) == NULL)
            break; // EOF or error

        Keyword keyword(buf);
        addKeyword(keyword);
       
    }
    fclose(f);

}

void KeywordsCollection::addKeyword(Keyword keyword) {

    if (size == capacity) {
        resize();
    }

    words[size] = keyword;
    printf("%s\n", words[size].word);
    
    size++;

}
