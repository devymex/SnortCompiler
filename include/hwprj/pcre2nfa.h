#pragma once

#include <hwprj\signatures.h>
#include <hwprj\nfa.h>
#include <vector>

#ifndef PCRE2NFA_DS
#define PCRE2NFA __declspec(dllimport)
#else
#define PCRE2NFA __declspec(dllexport)
#endif

//把单个pcre转化为NFA
PCRE2NFA ULONG PcreToNFA(const char *pPcre, CNfa &nfa, CSignatures &sigs);
