#include "stdafx.h"
#include "CreDfa.h"

unsigned int HashFunc(std::vector<size_t> &oneVec)
{
	unsigned int hashResult;
	hashResult = (oneVec.size() * oneVec.back() * oneVec[oneVec.size() / 2]) % HASHMODULO;

	return hashResult;
}

unsigned int GetHashVaule(std::vector<size_t> &oneVec, std::vector<size_t> hashTable[])
{
	unsigned int hashResult;
	hashResult = HashFunc(oneVec);
	unsigned int time = 0;

	while(hashTable[hashResult] != oneVec)
	{
		++time;

		if(time > HASHMODULO)
		{
			return HASHMODULO;
		}
		++hashResult;
	}
	return hashResult;
}

//
//bool DealNfaStas(std::vector<size_t> &oneVec, NFATODFAMAP &nfaToDfaMap, std::vector<size_t> hashTable[])
//{
//	unsigned int hashResult = HashFunc(oneVec);
//	unsigned int temp = hashResult;
//	unsigned int time = 0;
//
//	while(!hashTable[temp].empty())
//	{
//		++time;
//		if(time > HASHMODULO)
//			return false;
//		temp = (temp + 1) % HASHMODULO;
//	}
//	hashTable[temp] = oneVec;
//	nfaToDfaMap[temp] = nfaToDfaMap.size();
//
//	return true;
//}

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

void AvaiEdges(CNfa &oneNfaTab, std::vector<std::vector<size_t>> &charGroups)
{
	std::vector<CStateSet*> column[CHARSETSIZE - 3];
	for(size_t charNum = 0; charNum < CHARSETSIZE - 3; ++charNum)
	{
		column[charNum].reserve(20000);
		for(size_t i = 0; i < oneNfaTab.size(); ++i)
		{
			CStateSet &elem = oneNfaTab[i][charNum];
			elem.Sort();
			column[charNum].push_back(&elem);
			for (size_t j = 0; j < elem.Size(); ++j)
			{
				if(elem[j] > oneNfaTab.size())
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

	for (size_t i = 0; i < CHARSETSIZE - 3; ++i)
	{
		fullSet.push_back(i);
	}

	for (; !fullSet.empty();)
	{
		charGroups.push_back(std::vector<size_t>());
		std::vector<size_t> &curGroup = charGroups.back();
		curGroup.push_back(fullSet.front());
		fullSet.erase(fullSet.begin());
		for (std::vector<size_t>::iterator i = fullSet.begin(); i != fullSet.end() && !fullSet.empty();)
		{
			if (column[curGroup.front()] == column[*i])
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
}

void NfaToDfa(CNfa &oneNfaTab, CDfa &dfaTab)
{
	std::vector<std::vector<size_t>> charGroups;
	AvaiEdges(oneNfaTab, charGroups);

	//printNfa(oneNfaTab);
	//std::vector<size_t> hashTable[HASHMODULO];
	typedef std::unordered_map<std::vector<size_t>, size_t, STATESET_HASH> STATESETHASH;

	STATESETHASH ssh;
	ssh.rehash(STATESET_HASH::MAX_SIZE);

	std::stack<std::vector<size_t>> nfaStasStack;
	std::vector<size_t> startEVec;
	std::vector<size_t> startVec;

	char finFlag = 0;
	startVec.push_back(0);
	EClosure(oneNfaTab, startVec, startEVec, finFlag);


	nfaStasStack.push(startEVec);
	//ssh.insert(std::make_pair(startEVec, ssh.size()));
	ssh[startEVec] = ssh.size();

	size_t nCursize = dfaTab.size();
	dfaTab.resize(nCursize + 1);

	dfaTab.back().SetnFlag(dfaTab.back().GetnFlag() | dfaTab.back().START);

	if(finFlag == 1)
	{
		dfaTab.back().SetnFlag(dfaTab.back().GetnFlag() | dfaTab.back().TERMINAL);
		finFlag = 0;
	}
	std::vector<size_t> curNfaVec;

	while(nfaStasStack.size() > 0)
	{
		int curStaNum;
		curNfaVec = nfaStasStack.top();
		nfaStasStack.pop();

		for(std::vector<std::vector<size_t>>::iterator group = charGroups.begin();
			group != charGroups.end(); ++group)
		{
			if( dfaTab.size() > 30000)
			{
				return;
			}
			size_t nCurChar = group->front();

			STATESETHASH::iterator ir = ssh.find(curNfaVec);
			if (ir == ssh.end())
			{
				std::cout << "Fatal Error!" << std::endl;
				break;
			}
			curStaNum = ir->second;

			std::vector<size_t> nextNfaVec;
			NextNfaSet(oneNfaTab, curNfaVec, nCurChar, nextNfaVec, finFlag);

			if(!nextNfaVec.empty())
			{
				if(ssh.count(nextNfaVec) == 0)
				{
					size_t nextSta = ssh.size();
					ssh[nextNfaVec] = nextSta;

					size_t nCursize = dfaTab.size();
					dfaTab.resize(nCursize + 1);
					for(std::vector<size_t>::iterator iter = group->begin(); iter != group->end(); ++iter )
					{
						dfaTab[curStaNum][*iter] = nextSta;
					}
					if(finFlag == 1)
					{
						dfaTab.back().SetnFlag(dfaTab.back().GetnFlag() | dfaTab.back().TERMINAL);
						finFlag = 0;
					}
					nfaStasStack.push(nextNfaVec);
				}
				else
				{
					for(std::vector<size_t>::iterator iter = group->begin(); iter != group->end(); ++iter )
					{
						dfaTab[curStaNum][*iter] = ssh[nextNfaVec];
					}
				}
			}
		}
	}
	//dAll = t.Reset();

	//printDfa(dfaTab);
//	dfaTab.clear();

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
		if(*vecIter == oneNfaTab.size())
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

	if (eNfaVec.capacity() < 1000)
	{
		eNfaVec.reserve(1000);
	}

	for(std::vector<size_t>::const_iterator vecIter = curNfaVec.begin(); vecIter != curNfaVec.end(); ++vecIter)
	{
		eStack.push_back(*vecIter);
		eNfaVec.push_back(*vecIter);
		if(*vecIter == oneNfaTab.size())
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
				if(curSta < oneNfaTab.size())
				{
					const CStateSet &eTempVec = oneNfaTab[curSta][EMPTY];
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
		if(*iter == oneNfaTab.size())
		{
			finFlag = 1;
			break;
		}
	}
}

void printNfa(CNfa oneNfaTab)
{
	//std::vector<std::vector<std::vector<size_t>>> allStas;
	//for(int i = 0; i < oneNfaTab.size() + 1; ++i)
	//{
	//	std::vector<size_t> oneStaEdges;
	//	std::vector<std::vector<size_t>>  allStaEdges;
	//	allStas.push_back(allStaEdges);
	//	for(int j = 0; j < oneNfaTab.size() + 1; ++j)
	//	{
	//		allStas[i].push_back(oneStaEdges);
	//	}

	//}
	//for(size_t i = 0; i < oneNfaTab.size(); ++i)
	//{
	//	for(int j = 0; j < CHARSETSIZE; ++j)
	//	{
	//		if(oneNfaTab[i][j].size() > 0)
	//		{
	//			for(int k = 0; k < oneNfaTab[i][j].size(); ++k)
	//			{
	//				size_t oneSta = oneNfaTab[i][j][k];
	//				allStas[i][oneSta].push_back(j);
	//			}
	//		}
	//	}
	//}

	//for(std::vector<std::vector<std::vector<size_t>>>::iterator iter = allStas.begin(); iter != allStas.end(); ++iter)
	//{
	//	std::cout << iter - allStas.begin() << "  :  ";
	//	for(std::vector<std::vector<size_t>>::iterator curIter = iter->begin(); curIter != iter->end(); ++curIter)
	//	{
	//		if(curIter->size() > 0)
	//		{
	//			std::cout << curIter - iter->begin() << " (";
	//			for(std::vector<size_t>::iterator curcurIter = curIter->begin(); curcurIter != curIter->end(); ++curcurIter)
	//			{
	//				std::cout << *curcurIter << ",";
	//			}
	//			std::cout << ")  ";
	//		}
	//	}
	//	std::cout << std::endl;

	//}
}

void printDfa(CDfa dfaTab)
{
	//int tabSize = dfaTab.size();
	//std::vector<std::vector<std::vector<size_t>>> matrix;
	//std::vector<std::vector<size_t>> matRow;
	//std::vector<size_t> matRowRow;
	//for(int i = 0; i < tabSize; ++i)
	//{
	//	matrix.push_back(matRow);
	//	for(int j = 0; j < tabSize; ++j)
	//	{
	//		matrix[i].push_back(matRowRow);
	//	}
	//}

	//for(std::vector<CDfaRow>::iterator dfaiter = dfaTab.begin(); dfaiter != dfaTab.end(); ++dfaiter)
	//{
	//	for(int i = 0 ; i < CHARSETSIZE - 4; ++i)
	//	{
	//		if(dfaiter->destState[i] < 30000)
	//		{
	//			size_t suf = dfaiter->destState[i];
	//			matrix[dfaiter - dfaTab.begin()][suf].push_back(i);
	//		}
	//	}

	//}


	//for(std::vector<std::vector<std::vector<size_t>>>::iterator iter = matrix.begin(); iter != matrix.end(); ++iter)
	//{
	//	std::cout << iter - matrix.begin() << "  :  ";
	//	for(std::vector<std::vector<size_t>>::iterator curIter = iter->begin(); curIter != iter->end(); ++curIter)
	//	{
	//		if(curIter->size() > 0)
	//		{
	//			std::cout << curIter - iter->begin() << " (";
	//			for(std::vector<size_t>::iterator curcurIter = curIter->begin(); curcurIter != curIter->end(); ++curcurIter)
	//			{
	//				std::cout << *curcurIter << ",";
	//			}
	//			std::cout << ")  ";
	//		}
	//	}
	//	std::cout << std::endl;

	//}

}