#pragma once

#include <vector>
#include "../SnortCommon/snortcommon.h"

#ifndef PCRETONFA_H_
#define PCRETONFA __declspec(dllimport)
#else
#define PCRETONFA __declspec(dllexport)
#endif

//�ѵ���pcreת��ΪNFA
PCRETONFA bool PcreToNFA(const char *pPcre, CNfa &nfa);
