#pragma once

#include <hwprj\snortrule.h>
#include <hwprj\regrule.h>
#include <hwprj\nfa.h>
#include <hwprj\compiler.h>

extern double rule2pcretime;
extern double pcre2nfatime;
extern double nfa2dfatime;
extern double dfamintimetime;

ulong Rule2PcreList(const CSnortRule &rule, CRegRule &regrule);

ulong CRegChainToNFA(CRegChain &regchain, CNfa &nfa);

void CompileRule(LPCSTR rule, RECIEVER recv, LPVOID lpUser);

ulong LoadFile(const char *fileName, std::vector<std::string> &rules);

void Rule2Dfas(const CSnortRule &rule, CCompileResults &result, COMPILEDRULE &ruleResult);

void AssignSig(CCompileResults &result, ulong BegIdx, ulong EndIdx);