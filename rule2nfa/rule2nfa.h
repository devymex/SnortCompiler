#include "../common/common.h"
#include "../pcre2nfa/pcre2nfa.h"
#include <Windows.h>

#ifndef CRECHANFA_H_
#define CRECHANFA
#else
#define CRECHANFA
#endif


typedef void (CALLBACK *RECIEVER)(const CSnortRule &rule, LPVOID lpParam);

CRECHANFA size_t CompileRuleSet(LPCTSTR fileName, RECIEVER recv, LPVOID lpUser);

CRECHANFA void CompileRule(LPCSTR rule, RECIEVER recv, LPVOID lpUser);

//CRECHANFA size_t InterpretRule(const CSnortRule &rule, CNfaTree &outTree);

CRECHANFA size_t Rule2PcreList(const CSnortRule &rule, CRegRule &regrule);

CRECHANFA size_t CRegChainToNFA(CRegChain &regchain, CNfa &nfa);

//CRECHANFA void SerializeNfa(CNfaChain &nfaChain, CNfa &seriaNfa);
