/*
**	@file		comprule.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Functions declaration of rules' processing
**
**	Includes compile rule etc.
**
*/

#pragma once

#include <hwprj\snortrule.h>
#include <hwprj\regrule.h>
#include <hwprj\nfa.h>
#include <hwprj\compiler.h>

extern double rule2pcretime;
extern double pcre2nfatime;
extern double nfa2dfatime;
extern double dfamintimetime;

void __stdcall CompileCallback(const PARSERESULT &parseRes, void *lpVoid);

ulong Rule2PcreList(const CSnortRule &rule, CRegRule &regrule);

ulong Chain2NFA(const CRegChain &regchain, CNfa &nfa, CSignatures &sigs);

void CompileRule(LPCSTR rule, RECIEVER recv, LPVOID lpUser);

ulong LoadFile(const char *fileName, std::vector<std::string> &rules);

void Rule2Dfas(const CSnortRule &rule, CCompileResults &result, COMPILEDINFO &ruleResult);

void AssignSig(CCompileResults &result, ulong BegIdx, ulong EndIdx);

ulong ProcessOption(std::string &ruleOptions, CSnortRule &snortRule);

void Rule2Dfas(const CRegRule &rule, CCompileResults &result);
