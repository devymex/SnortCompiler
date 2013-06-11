#pragma once

#define WINVER 0x0600
#define _WIN32_WINNT 0x0600 
#define _WIN32_WINDOWS 0x0410
#define _WIN32_IE 0x0700

#include <tchar.h>
#include <windows.h>

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>

#define COMPILERNEW_H_

#define REGRULEHDR_DS
#define SNORTRULEHDR_DS
#define DFAARYHDR_DS
#define SIDDFAHDR_DS
#define COMPRESHDR_DS
#define COMPRULEHDR_DS
#define PCREMATCHHDR_DS
#define SIGHDR_DS

typedef std::vector<byte>			BYTEARY;
typedef BYTEARY::iterator			BYTEARY_ITER;
typedef BYTEARY::const_iterator		BYTEARY_CITER;
