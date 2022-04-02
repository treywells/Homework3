// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define QUIT 0

// add headers that you want to pre-compile here
// Imported headers
#include <Windows.h>
#include <iostream>
#include <Winternl.h>
#include <psapi.h>
#include <stdlib.h>

// User Defined Classes
#include "cpu.h"
#include "Keyword.h"
#include "KeywordsCollection.h"
#include "PC.h"
#include "Disk.h"
#include "SearchBuffer.h"
#include "Search.h"

#include "ThreadStarters.h"

#endif //PCH_H
