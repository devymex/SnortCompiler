#include "../common/common.h"
#include "../pcre2nfa/pcre2nfa.h"
#include <Windows.h>

#ifndef CRECHANFA_H_
#define CRECHANFA
#else
#define CRECHANFA
#endif


typedef void (CALLBACK *RECIEVER)(const CSnortRule &rule, LPVOID lpParam);

CRECHANFA size_t ParseRule(LPCTSTR fileName, RECIEVER recv, LPVOID lpUser);

CRECHANFA size_t InterpretRule(const CSnortRule &rule, CNfaTree &outTree);

CRECHANFA void SerializeNfa(CNfaChain &nfaChain, CNfa &seriaNfa);
