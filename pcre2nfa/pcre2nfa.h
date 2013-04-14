#pragma once

#include <vector>
#include "../common/common.h"

#ifndef PCRETONFA_H_
#define PCRETONFA __declspec(dllimport)
#else
#define PCRETONFA __declspec(dllexport)
#endif

#define SC_SUCCESS 0
#define SC_ERROR -1
#define SC_EXCEED -2
#define SC_EMPTY -3

//把单个pcre转化为NFA
PCRETONFA size_t PcreToNFA(const char *pPcre, CNfa &nfa);
