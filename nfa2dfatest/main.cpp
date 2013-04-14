#include "../pcre2nfa/pcre2nfa.h"
#include <iostream>
#include "../nfa2dfa/nfa2dfa.h"
#include "../SnortCommon/snortcommon.h"

void main()
{
	const char* a = "/BM+.+b{2,}/";
	CNfa nfa;
	CDfa dfa;
	PcreToNFA(a, nfa);
	size_t nfasize = nfa.size();
	NfaToDfa(nfa, dfa);
	size_t dfasize = dfa.size();
	system("pause");
}