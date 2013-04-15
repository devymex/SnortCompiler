#include "stdafx.h"
#include "../common/common.h"
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

void main()
{
	CDfa dfa1, dfa2;
	CAndDfa dfa3;
	dfa1.Resize(2);
	dfa2.Resize(2);
	dfa1[0][97] =  0;
	dfa1[0][98] =  1;
	dfa1[1][98] =  1;

	dfa1[1].SetFlag(dfa1[1].GetFlag() | CDfaRow::TERMINAL);

	printDfa(dfa1);
	std::cout << ".............." << std::endl;

	dfa2[0][97] =  1;
	dfa2[0][98] =  1;
	dfa2[1][97] =  1;
	dfa2[1].SetFlag(dfa2[1].GetFlag() | CDfaRow::TERMINAL);

	printDfa(dfa2);
	std::cout << ".............." << std::endl;

	AndMerge(dfa1, dfa2, dfa3);
	printDfa(dfa3);
	std::cout << ".............." << std::endl;

	system("pause");
}