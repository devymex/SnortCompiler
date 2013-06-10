#pragma once

#include <hwprj\snortrule.h>
#include <hwprj\regrule.h>
#include <hwprj\nfa.h>

#ifndef RULE2NFAHDR_DS
#define RULE2NFAHDR __declspec(dllimport)
#else
#define RULE2NFAHDR __declspec(dllexport)
#endif

typedef void (CALLBACK *RECIEVER)(const CSnortRule &rule, LPVOID lpParam);

RULE2NFAHDR ULONG CompileFile(LPCTSTR fileName, RECIEVER recv, LPVOID lpUser);

RULE2NFAHDR void CompileRule(LPCSTR rule, RECIEVER recv, LPVOID lpUser);

RULE2NFAHDR ULONG Rule2PcreList(const CSnortRule &rule, CRegRule &regrule);

RULE2NFAHDR ULONG CRegChainToNFA(CRegChain &regchain, CNfa &nfa);

