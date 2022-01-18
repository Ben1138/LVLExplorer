// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once


#ifdef _WIN32

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#endif //ifdef _WIN32

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#ifdef _WIN32

#include <tchar.h>

#endif //_WIN32