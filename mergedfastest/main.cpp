#include "stdafx.h"
#include "../common/common.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../nfa2dfa/nfa2dfa.h"
#include "../mergedfas/mergedfas.h"

template <class _t>
void printDfa(_t dfaTab)
{
	int tabSize = dfaTab.Size();
	std::vector<std::vector<std::vector<size_t>>> matrix;
	std::vector<std::vector<size_t>> matRow;
	std::vector<size_t> matRowRow;
	for(int i = 0; i < tabSize; ++i)
	{
		matrix.push_back(matRow);
		for(int j = 0; j < tabSize; ++j)
		{
			matrix[i].push_back(matRowRow);
		}
	}

		for(size_t s = 0; s < dfaTab.Size(); ++s)
	{
		for(int i = 0 ; i < CHARSETSIZE - 4; ++i)
		{
			if(dfaTab[s][i] != size_t(-1))
			{
				size_t suf = dfaTab[s][i];
				matrix[s][suf].push_back(i);
			}
		}

	}


	for(std::vector<std::vector<std::vector<size_t>>>::iterator iter = matrix.begin(); iter != matrix.end(); ++iter)
	{
		std::cout << iter - matrix.begin() << "  :  ";
		for(std::vector<std::vector<size_t>>::iterator curIter = iter->begin(); curIter != iter->end(); ++curIter)
		{
			if(curIter->size() > 0)
			{
				std::cout << curIter - iter->begin() << " (";
				for(std::vector<size_t>::iterator curcurIter = curIter->begin(); curcurIter != curIter->end(); ++curcurIter)
				{
					std::cout << *curcurIter << ",";
				}
				std::cout << ")  ";
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
	const char *pcre1 = "/^abcd/";
	const char *pcre2 = "/^fghi/";

	const char *pcre3 = "/^fghiijk/";

	std::vector<CDfa> dfaVec;
	CDfa dfa1, dfa2,dfa3, lastdfa;
	CNfa nfa1, nfa2, nfa3;
	PcreToNFA(pcre1, nfa1);
	//PcreToNFA(pcre2, nfa2);
	printNfa(nfa1);
	printNfa(nfa2);
	NfaToDfa(nfa1, dfa1);
	//NfaToDfa(nfa2, dfa2);
	//NfaToDfa(nfa3, dfa3);

	//size = dfa3.Size();
	size_t size 
		= dfa1.Size();
	//size = dfa2.Size();
	//size = dfa3.Size();
	dfaVec.push_back(dfa1);
	dfaVec.push_back(dfa2);
	dfaVec.push_back(dfa3);
	OrMerge(dfaVec, lastdfa);
	size = lastdfa.Size();
	std::string str1 = "abcd";
	size_t match = MatchDfa(dfa1, str1.begin(), str1.end());
	match = MatchDfa(lastdfa, str1.begin(), str1.end());

	size = lastdfa.Size();

}