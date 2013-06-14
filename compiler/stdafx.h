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

#include <hwprj\common.h>
#include <hwprj\dfa.h>
#include <hwprj\dllstring.h>
#include <hwprj\ctimer.h>

#define COMPILERHDR_DS
#define REGRULEHDR_DS
#define SNORTRULEHDR_DS
#define DFAARYHDR_DS
#define SIDDFAHDR_DS
#define COMPRESHDR_DS
#define COMPRULEHDR_DS
#define PCREMATCHHDR_DS
#define SIGHDR_DS

typedef std::vector<byte>					BYTEARY;
typedef BYTEARY::iterator					BYTEARY_ITER;
typedef BYTEARY::const_iterator				BYTEARY_CITER;
typedef std::vector<CDfa>					CDFAVEC;
typedef std::vector<CDllString>				STRINGVEC;
typedef std::string							STRING;
typedef std::vector<class CRuleOption*>		OPTIONVEC;
typedef std::vector<class CRegChain>		CHAINVEC;
typedef std::vector<SIGNATURE>              SIGNATUREVEC;
typedef SIGNATUREVEC::iterator              SIGVEC_ITER;
