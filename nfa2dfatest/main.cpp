#include <iostream>
#include "../common/common.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../nfa2dfa/nfa2dfa.h"


//void printDfa(CDfa dfaTab)
//{
// int tabSize = dfaTab.Size();
// std::vector<std::vector<std::vector<STATEID>>> matrix;
// std::vector<std::vector<STATEID>> matRow;
// std::vector<STATEID> matRowRow;
// for(int i = 0; i < tabSize; ++i)
// {
// matrix.push_back(matRow);
// for(int j = 0; j < tabSize; ++j)
// {
// matrix[i].push_back(matRowRow);
// }
// }
//
// for(STATEID s = 0; s < dfaTab.Size(); ++s)
// {
// for(int i = 0 ; i < dfaTab.GetColNum(); ++i)
// {
// if(dfaTab[s][i] != STATEID(-1))
// {
// STATEID suf = dfaTab[s][i];
// matrix[s][suf].push_back(i);
// }
// }
//
// }
//
//
// for(std::vector<std::vector<std::vector<STATEID>>>::iterator iter = matrix.begin(); iter != matrix.end(); ++iter)
// {
// std::cout << iter - matrix.begin() << " : ";
// for(std::vector<std::vector<STATEID>>::iterator curIter = iter->begin(); curIter != iter->end(); ++curIter)
// {
// if(curIter->size() > 0)
// {
// std::cout << curIter - iter->begin() << " (";
// for(std::vector<STATEID>::iterator curcurIter = curIter->begin(); curcurIter != curIter->end(); ++curcurIter)
// {
// std::cout << *curcurIter << ",";
// }
// std::cout << ") ";
// }
// }
// std::cout << std::endl;
//
// }
//
//}

void printDfa(CDfa &dfa)
{
	for (STATEID j = 0; j < dfa.Size(); ++j)
	{
		std::cout << (int)j << ": ";
		for (STATEID k = 0; k < dfa.GetColNum(); ++k)
		{
			if(dfa[j][k] != STATEID(-1))
			{
				std::cout << "(" << (int)k << "," << (int)dfa[j][k]<< ")";
			}

		}
		std::cout << std::endl;
	}
}

//void printNfa(CNfa &nfa)
//{
//	for (size_t j = 0; j < nfa.Size(); ++j)
//	{
//		std::cout << j << ": ";
//		for (size_t k = 0; k < CHARSETSIZE; ++k)
//		{
//			for (size_t l = 0; l < nfa[j][k].Size(); ++l)
//			{
//				std::cout << "(" << k << "," << nfa[j][k][l] << ")";
//			}
//		}
//		std::cout << std::endl;
//	}
//}
//void main()
//{
//	CNfa nfa;
//	CDfa dfa;
//	size_t size = dfa.Size();
//	nfa.Resize(5);
//	nfa[0][EMPTY].PushBack(1);
//	nfa[0][EMPTY].PushBack(3);
//	nfa[1]['a'].PushBack(1);
//	nfa[1]['b'].PushBack(2);
//	nfa[1][EMPTY].PushBack(5);
//	nfa[2]['b'].PushBack(2);
//	nfa[2][EMPTY].PushBack(5);
//	nfa[3]['a'].PushBack(4);
//	nfa[3]['b'].PushBack(4);
//	nfa[4]['a'].PushBack(4);
//	nfa[4][EMPTY].PushBack(5);
//
//	printNfa(nfa);
//	NfaToDfa(nfa,dfa);
//	printDfa(dfa);
//	system("pause");
//}

void main()
{
	CDfa dfa;

	dfa.SetId(1);
	BYTE *pGroup = new BYTE[DFACOLSIZE];
	std::fill(pGroup, pGroup + DFACOLSIZE, 4);
	pGroup['a'] = 0;
	pGroup['b'] = 1;
	pGroup['c'] = 2;
	pGroup['d'] = 3;
	dfa.SetGroup(pGroup);
	
	dfa.Resize(7);
	for (STATEID i = 0; i < 7; ++i)
	{
		dfa[i].Fill(STATEID(-1));
	}

	dfa[0][0] = 2;
	dfa[0][1] = 1;
	dfa[1][0] = 3;
	dfa[1][1] = 1;
	dfa[2][1] = 5;
	dfa[2][2] = 2;
	dfa[2][3] = 4;
	dfa[3][1] = 6;
	dfa[3][2] = 2;
	dfa[3][3] = 4;
	dfa[4][0] = 3;
	dfa[5][1] = 5;
	dfa[6][1] = 5;
	
	dfa[0].SetFlag(1);
	dfa[1].SetFlag(2);
	dfa[2].SetFlag(2);
	dfa[3].SetFlag(2);
	dfa[4].SetFlag(2);
	dfa[5].SetFlag(4);
	dfa[6].SetFlag(4);

	CDfa::TERMSET oneterm;
	oneterm.dfaId = 1;
	oneterm.dfaSta = STATEID(5);
	dfa.PushTermSet(oneterm);
	oneterm.dfaSta = STATEID(6);
	dfa.PushTermSet(oneterm);

	printDfa(dfa);
	std::cout << std::endl;

	CDfa minDfa;
	DfaMin(dfa, minDfa);
}