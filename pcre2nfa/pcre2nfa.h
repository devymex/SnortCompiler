#pragma once

#include <vector>
#include "../SnortCommon/snortcommon.h"

#ifndef PCRETONFA_H_
#define PCRETONFA __declspec(dllimport)
#else
#define PCRETONFA __declspec(dllexport)
#endif

//把单个pcre转化为NFA
PCRETONFA bool PcreToNFA(const char *pPcre, CNfa &nfa);
