#include "../common/common.h"
#include <Windows.h>

#ifndef CRECHANFA_H_
#define CRECHANFA
#else
#define CRECHANFA
#endif

CRECHANFA size_t InterpretRule(const CSnortRule &rule, CNfaTree &outTree);

CRECHANFA void SerializeNfa(CNfaChain &nfaChain, CNfa &seriaNfa);
