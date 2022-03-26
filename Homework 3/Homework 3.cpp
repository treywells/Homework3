#include "pch.h"

int main(int argc, char** argv)
{

    DWORD numOfCmdArgs = 2;

    // Check if right number of command line args 
    if (argc != numOfCmdArgs) {
        printf("Incorrect amount of commandline arguments, %d are needed\n", numOfCmdArgs);
        exit(-1);
    }

    char* keywordFileName = argv[1];

    KeywordsCollection keywords(keywordFileName);

   
}

