#include <iostream>
#include "../common/common.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../nfa2dfa/nfa2dfa.h"


//void printDfa(CDfa dfaTab)
//{
//	int tabSize = dfaTab.Size();
//	std::vector<std::vector<std::vector<STATEID>>> matrix;
//	std::vector<std::vector<STATEID>> matRow;
//	std::vector<STATEID> matRowRow;
//	for(int i = 0; i < tabSize; ++i)
//	{
//		matrix.push_back(matRow);
//		for(int j = 0; j < tabSize; ++j)
//		{
//			matrix[i].push_back(matRowRow);
//		}
//	}
//
//		for(STATEID s = 0; s < dfaTab.Size(); ++s)
//	{
//		for(int i = 0 ; i < dfaTab.GetColNum(); ++i)
//		{
//			if(dfaTab[s][i] != STATEID(-1))
//			{
//				STATEID suf = dfaTab[s][i];
//				matrix[s][suf].push_back(i);
//			}
//		}
//
//	}
//
//
//	for(std::vector<std::vector<std::vector<STATEID>>>::iterator iter = matrix.begin(); iter != matrix.end(); ++iter)
//	{
//		std::cout << iter - matrix.begin() << "  :  ";
//		for(std::vector<std::vector<STATEID>>::iterator curIter = iter->begin(); curIter != iter->end(); ++curIter)
//		{
//			if(curIter->size() > 0)
//			{
//				std::cout << curIter - iter->begin() << " (";
//				for(std::vector<STATEID>::iterator curcurIter = curIter->begin(); curcurIter != curIter->end(); ++curcurIter)
//				{
//					std::cout << *curcurIter << ",";
//				}
//				std::cout << ")  ";
//			}
//		}
//		std::cout << std::endl;
//
//	}
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
	CNfa nfa;
	CDfa dfa;
	size_t size = dfa.Size();
	nfa.Resize(5);
	nfa[0][EMPTY].PushBack(1);
	nfa[0][EMPTY].PushBack(3);
	nfa[1]['a'].PushBack(1);
	nfa[1]['b'].PushBack(2);
	nfa[1][EMPTY].PushBack(5);
	nfa[2]['b'].PushBack(2);
	nfa[2][EMPTY].PushBack(5);
	nfa[3]['a'].PushBack(4);
	nfa[3]['b'].PushBack(4);
	nfa[4]['a'].PushBack(4);
	nfa[4][EMPTY].PushBack(5);

	printNfa(nfa);
	NfaToDfa(nfa,dfa);
	printDfa(dfa);
	system("pause");
}