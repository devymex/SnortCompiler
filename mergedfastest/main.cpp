#include "stdafx.h"
#include "../common/common.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../nfa2dfa/nfa2dfa.h"
#include "../mergedfas/mergedfas.h"

void printDfa(CDfa dfaTab)
{
	for (size_t j = 0; j < dfaTab.Size(); ++j)
		{
			std::cout << j << ": ";
			for (size_t k = 0; k < CHARSETSIZE - 4; ++k)
			{
				size_t temp = dfaTab.GetGroup(k);
				if(dfaTab[j][temp] != (BYTE (-1)))
				{
					std::cout << "(" << k << "," << dfaTab[j][temp] << ")";
				}

			}
			std::cout << std::endl;
		}
}

void printNfa(CNfa &nfa)
{
	for (size_t j = 0; j < nfa.Size(); ++j)
		{
			std::cout << j << ": ";
			for (size_t k = 0; k < CHARSETSIZE; ++k)
			{
				for (size_t l = 0; l < nfa[j][k].Size(); ++l)
				{
				
					std::cout << "(" << k << "," << nfa[j][k][l] << ")";
				}
			}
			std::cout << std::endl;
		}
}

void main()
{
	const char *pcre1 = "/^ab.*c/";
	const char *pcre2 = "/^de.*f/";

	std::vector<CDfa> dfaVec;
	CDfa dfa1, dfa2,lastdfa;

	dfa1.SetId(1);
	dfa2.SetId(2);

	CNfa nfa1, nfa2;
	PcreToNFA(pcre1, nfa1);
	//size_t nfasize = nfa1.Size();
	NfaToDfa(nfa1, dfa1);
		
	PcreToNFA(pcre2, nfa2);
	NfaToDfa(nfa2, dfa2);

	dfaVec.push_back(dfa1);
	dfaVec.push_back(dfa2);
	//NfaToDfa(nfa3, dfa3);
	OrMerge(dfaVec, lastdfa);
	printDfa(lastdfa);

	std::cout << lastdfa.Size() << std::endl;

	//size = dfa3.Size();
	size_t size = dfa1.Size();

	size = lastdfa.Size();

	CDfa mindfa;
	DfaMin(lastdfa, mindfa);
	size = mindfa.Size();

	//size = dfa2.Size();
	//size = dfa3.Size();


	std::string str1 = "abcd";
	std::string str2 = "fghi";
	size_t match = MatchDfa(dfa1, str1.begin(), str1.end());
}