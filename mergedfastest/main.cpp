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
	const char *pcre1 = "/.*AUTH\\s[^\\n]{2}/";

	std::vector<CDfa> dfaVec;
	CDfa dfa1;

	dfa1.SetId(1);
	CNfa nfa1;
	PcreToNFA(pcre1, nfa1);
	printNfa(nfa1);
	NfaToDfa(nfa1, dfa1);
	//NfaToDfa(nfa3, dfa3);

	//size = dfa3.Size();
	size_t size 
		= dfa1.Size();
	//size = dfa2.Size();
	//size = dfa3.Size();

	printDfa(dfa1);

	std::string str1 = "abcd";
	std::string str2 = "fghi";
	size_t match = MatchDfa(dfa1, str1.begin(), str1.end());
	//match = MatchDfa(lastdfa, str2.begin(), str2.end());


}