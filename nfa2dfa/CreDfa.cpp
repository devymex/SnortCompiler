#include "stdafx.h"
#include "CreDfa.h"
#include "nfa2dfa.h"


/* Following codes is a testing for unordered_map */
/* DONT REMOVE */
//std::vector<size_t> v1, v2, v3;
//v1.push_back(0);
//v2.push_back(1);
////v2.push_back(2);

//int n = ssh.size();
//ssh.insert(std::make_pair(v1, 20));
//n = ssh.size();
//ssh.insert(std::make_pair(v2, 30));
//n = ssh.size();

//STATESETHASH::iterator xx1 = ssh.find(v1);
//n = xx1->second;
//STATESETHASH::iterator xx2 = ssh.find(v2);
//n = xx2->second;

bool ColumnEqual(std::vector<CStateSet*> &c1, std::vector<CStateSet*>&c2)
{
	if (c1.size() != c2.size())
	{
		return false;
	}
	for (size_t i = 0; i < c1.size(); ++i)
	{
		if (c1[i]->Size() != c2[i]->Size())
		{
			return false;
		}
		if (!(*c1[i] == *c2[i]))
		{
			return false;
		}
	}
	return true;
}

void AvaiEdges(CNfa &oneNfaTab, BYTE *group)
{
	std::vector<std::vector<BYTE>> charGroups;
	std::vector<CStateSet*> column[CHARSETSIZE];
	for(size_t charNum = 0; charNum < CHARSETSIZE; ++charNum)
	{
		column[charNum].reserve(20000);
		for(size_t i = 0; i < oneNfaTab.Size(); ++i)
		{
			CStateSet &elem = oneNfaTab[i][charNum];
			elem.Sort();
			column[charNum].push_back(&elem);
			for (size_t j = 0; j < elem.Size(); ++j)
			{
				if(elem[j] > oneNfaTab.Size())
				{
					std::cout << "overflow" << std::endl;
					return;
				}
			}
		}
	}

	charGroups.clear();
	std::vector<size_t> fullSet;

	//std::cout << "columns complete" << std::endl;

	for (size_t i = 0; i < CHARSETSIZE; ++i)
	{
		fullSet.push_back(i);
	}

	for (; !fullSet.empty();)
	{
		charGroups.push_back(std::vector<BYTE>());
		std::vector<BYTE> &curGroup = charGroups.back();
		curGroup.push_back(fullSet.front());
		fullSet.erase(fullSet.begin());
		for (std::vector<size_t>::iterator i = fullSet.begin(); i != fullSet.end() && !fullSet.empty();)
		{
			if (ColumnEqual(column[curGroup.front()], column[*i]))
			{
				curGroup.push_back(*i);
				i = fullSet.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
	//std::cout << "grouping complete" << std::endl;

	for(size_t i = 0; i < charGroups.size(); ++i)
	{
		for(size_t j = 0; j < charGroups[i].size(); ++j)
		{
			group[charGroups[i][j]] = i;
		}
	}
}

void NextNfaSet(const CNfa &oneNfaTab, const std::vector<size_t> &curNfaVec, size_t edge, std::vector<size_t> &nextENfaVec, char &finFlag)
{
	if (edge >= CHARSETSIZE)
	{
		std::cout << "Fatal Error!" << std::endl;
		return;
	}
	std::vector<size_t> nextNfaVec;

	for(std::vector<size_t>::const_iterator vecIter = curNfaVec.begin(); vecIter != curNfaVec.end(); ++vecIter)
	{
		if(*vecIter == oneNfaTab.Size())
		{
			continue;
		}
		const CStateSet &nextEdges = oneNfaTab[*vecIter][edge];
		for (size_t i = 0; i < nextEdges.Size(); ++i)
		{
			nextNfaVec.push_back(nextEdges[i]);
		}
	}
	std::sort(nextNfaVec.begin(), nextNfaVec.end());
	nextNfaVec.erase(std::unique(nextNfaVec.begin(), nextNfaVec.end()), nextNfaVec.end());
	if(!nextNfaVec.empty())
	{
		EClosure(oneNfaTab, nextNfaVec, nextENfaVec, finFlag);
	}
}

void EClosure(const CNfa &oneNfaTab, const std::vector<size_t> &curNfaVec, std::vector<size_t> &eNfaVec, char &finFlag)
{
	std::vector<size_t> eStack;
	char caledStat[20000] = {0};
	const size_t cEmptyEdge = 256;

	if (eNfaVec.capacity() < 1000)
	{
		eNfaVec.reserve(1000);
	}

	for(std::vector<size_t>::const_iterator vecIter = curNfaVec.begin(); vecIter != curNfaVec.end(); ++vecIter)
	{
		eStack.push_back(*vecIter);
		eNfaVec.push_back(*vecIter);
		if(*vecIter == oneNfaTab.Size())
		{
			continue;
		}

		while(!eStack.empty())
		{
			size_t curSta;
			curSta = eStack.back();
			eStack.pop_back();
			if(caledStat[curSta] == 0)
			{
				caledStat[curSta] = 1;
				if(curSta < oneNfaTab.Size())
				{
					const CStateSet &eTempVec = oneNfaTab[curSta][cEmptyEdge];
					if(eTempVec.Size() != 0)
					{
						for (size_t i = 0; i < eTempVec.Size(); ++i)
						{
							eNfaVec.push_back(eTempVec[i]);
							eStack.push_back(eTempVec[i]);
						}
					}
				}
			}
		}
	}

	std::sort(eNfaVec.begin(), eNfaVec.end());
	eNfaVec.erase(std::unique(eNfaVec.begin(), eNfaVec.end()), eNfaVec.end());

	for(std::vector<size_t>::iterator iter = eNfaVec.begin(); iter != eNfaVec.end(); ++iter)
	{
		if(*iter == oneNfaTab.Size())
		{
			finFlag = 1;
			break;
		}
	}
}


void printDfa(CDfa dfaTab)
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