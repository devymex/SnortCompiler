#include <iostream>
#include "../common/common.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../nfa2dfa/nfa2dfa.h"

void main()
{
	//const char* a = "/abcd.*ABCD{1024}/";
	//CNfa nfa;
	//CDfa dfa;
	//PcreToNFA(a, nfa);
	//size_t nfasize = nfa.Size();
	//NfaToDfa(nfa, dfa);
	//size_t dfasize = dfa.Size();
	CDfa dfa;
	dfa.Resize(5);
	dfa[0]['a'] = 1;
	dfa[0]['b'] = 2;
	dfa[1]['a'] = 1;
	dfa[1]['b'] = 4;
	dfa[2]['b'] = 2;
	dfa[3]['a'] = 4;
	dfa[3]['b'] = 1;
	dfa[4].SetFlag(4);

	for (size_t i = 0; i < 5; ++i)
	{
		for (size_t j = 97; j < 99; ++j)
		{
			std::cout << dfa[i][j] << " "; 
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	CDfa mindfa;
	DfaMin(dfa, mindfa);

	system("pause");
}