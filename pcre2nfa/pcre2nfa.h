#pragma once

#include <vector>
#include "../common/common.h"

#ifndef PCRETONFA_H_
#define PCRETONFA __declspec(dllimport)
#else
#define PCRETONFA __declspec(dllexport)
#endif

#define SC_SUCCESS size_t(0)
#define SC_ERROR size_t(-1)
#define SC_EXCEED size_t(-2)

//把单个pcre转化为NFA
PCRETONFA size_t PcreToNFA(const char *pPcre, CNfa &nfa, CRegChain &regchain);
PCRETONFA void GetSignature(std::vector<unsigned char> &code, std::vector<std::vector<unsigned char>> &strs);
