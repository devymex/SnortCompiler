#pragma once

#include <vector>
#include "../SnortCommon/snortcommon.h"

#ifndef PCRETONFA_H_
#define PCRETONFA __declspec(dllimport)
#else
#define PCRETONFA __declspec(dllexport)
#endif

#define SC_SUCCESS 0
#define SC_ERROR -1
#define SC_EXCEED -2

//�ѵ���pcreת��ΪNFA
PCRETONFA size_t PcreToNFA(const char *pPcre, CNfa &nfa);
