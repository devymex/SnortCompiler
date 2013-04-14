#include <iostream>
#include "../common/common.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../nfa2dfa/nfa2dfa.h"

void main()
{
	const char* a = "/BM+.+b{2,}/";
	CNfa nfa;
	CDfa dfa;
	PcreToNFA(a, nfa);
	size_t nfasize = nfa.Size();
	NfaToDfa(nfa, dfa);
	size_t dfasize = dfa.Size();
	system("pause");
}