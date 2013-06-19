/*
**	@file		pcre2nfa.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		transform pcre to nfa
**
*/

#include <hwprj\common.h>
#include <hwprj\nfa.h>
#include <hwprj\signatures.h>

void ExtractSequence(BYTEARY &code, std::vector<BYTEARY> &seqAry);

void PcreToNFA(BYTEARY &code, bool bFromBeg, CNfa &nfa);
