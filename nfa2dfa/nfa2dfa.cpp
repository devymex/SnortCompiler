#include "stdafx.h"
#include "CreDfa.h"
#include "nfa2dfa.h"

CREDFA size_t NfaToDfa(CNfa &oneNfaTab, CDfa &dfaTab)
{
	std::vector<std::vector<size_t>> charGroups;
	AvaiEdges(oneNfaTab, charGroups);

	//printNfa(oneNfaTab);
	//std::vector<size_t> hashTable[HASHMODULO];
	typedef std::unordered_map<std::vector<size_t>, STATEID, STATESET_HASH> STATESETHASH;

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
	ssh[startEVec] = 0;

	size_t nCursize = dfaTab.Size();
	dfaTab.Resize(nCursize + 1);

	dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().START);

	if(finFlag == 1)
	{
		dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().TERMINAL);
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
			if( dfaTab.Size() > SC_STATELIMIT)
			{
				return (size_t)-1;
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
#undef max
					if (ssh.size() > std::numeric_limits<STATEID>::max())
					{
						std::cerr << "Fatal Error!" << std::endl;
						return (size_t)-1;
					}
					STATEID nextSta = static_cast<STATEID>(ssh.size());
					ssh[nextNfaVec] = nextSta;

					size_t nCursize = dfaTab.Size();
					dfaTab.Resize(nCursize + 1);
					for(std::vector<size_t>::iterator iter = group->begin(); iter != group->end(); ++iter )
					{
						dfaTab[curStaNum][*iter] = nextSta;
					}
					if(finFlag == 1)
					{
						dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().TERMINAL);
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

	return 0;
}

void RemoveUnreachable(const CNfa &nfa, const std::vector<size_t> &begs, std::vector<BYTE> &reachable)
{
	std::vector<BYTE> staFlags(nfa.Size(), 0);

	std::vector<size_t> staStack;
	staStack.reserve(nfa.Size());

	staStack.assign(begs.begin(), begs.end());
	for (; !staStack.empty(); )
	{
		size_t nSta = staStack.back();
		staStack.pop_back();
		if (staFlags[nSta] == 0)
		{
			staFlags[nSta] = 1;
			for (size_t i = 0; i < CHARSETSIZE; ++i)
			{
				for (size_t j = 0; j < nfa[nSta][i].Size(); ++j)
				{
					staStack.push_back(nfa[nSta][i][j]);
				}
			}
		}
	}

	if (reachable.empty())
	{
		reachable.resize(staFlags.size(), 0);
	}
	for (std::vector<BYTE>::iterator i = staFlags.begin(); i != staFlags.end(); ++i)
	{
		reachable[i - staFlags.begin()] += *i;
	}
}

void MergeReachable(CDfa &oneDfaTab, std::vector<BYTE> &reachable, CDfa &tmpDfa)
{
	size_t num = 0;
	for (std::vector<BYTE>::iterator iter = reachable.begin(); iter != reachable.end(); ++iter)
	{
		if (*iter != 0)
		{
			num++;
		}
	}
	tmpDfa.Resize(num);
	size_t index = 0;
	for (std::vector<BYTE>::iterator iter = reachable.begin(); iter != reachable.end(); ++iter)
	{
		if (*iter != 0)
		{
			size_t stas = iter - reachable.begin();
			tmpDfa[index].SetFlag(oneDfaTab[stas].GetFlag());			
			tmpDfa[index++] = oneDfaTab[stas];
		}
	}

	for (std::vector<BYTE>::iterator iter = reachable.begin(); iter != reachable.end(); ++iter)
	{
		if (*iter == 0)
		{
			size_t stas = iter - reachable.begin();
			for (size_t i = 0; i < tmpDfa.Size(); ++i)
			{
				for (size_t j = 0; j < tmpDfa.GetColNum(); ++j)
				{
					if (tmpDfa[i][j] == stas)
					{
						tmpDfa[i][j] = size_t(-1); 
					}
				}
			}
		}
	}

	index = 0;
	for (std::vector<BYTE>::iterator iter = reachable.begin(); iter != reachable.end(); ++iter)
	{
		if (*iter != 0)
		{
			size_t stas = iter - reachable.begin();
			for (size_t i = 0; i < tmpDfa.Size(); ++i)
			{
				for (size_t j = 0; j < tmpDfa.GetColNum(); ++j)
				{
					if (tmpDfa[i][j] == stas)
					{
						tmpDfa[i][j] = index; 
					}
				}
			}
			index++;
		}
	}
}

int SearchInSet(std::vector<size_t>::iterator beg, std::vector<size_t>::iterator end, std::vector<std::vector<size_t>> &NewSplitStates)
{
	int post = 0;
	if (!NewSplitStates.empty())
	{
		for (std::vector<std::vector<size_t>>::iterator i = NewSplitStates.begin(); i != NewSplitStates.end(); ++i)
		{
			std::vector<size_t>::iterator pos = std::search(i->begin(), i->end(), beg, end);
			if (pos != i->end())
			{
				post = pos - i->begin();
			}
		}
	}
	return post;
}

void SubPartition(std::vector<std::vector<size_t>> &partition_states, std::vector<std::vector<size_t>> &NewSplitStates, std::vector<size_t> &equstates, std::vector<std::vector<size_t>> &NextPartition)
{
	std::vector<std::vector<size_t>> temp = NewSplitStates;
	for (std::vector<std::vector<size_t>>::iterator iPS = partition_states.begin(); iPS != partition_states.end(); ++iPS)
	{
		std::vector<size_t> intersect;
		std::vector<size_t> difference;
		std::sort(iPS->begin(),iPS->end());
		std::sort(equstates.begin(),equstates.end());
		std::set_intersection(iPS->begin(), iPS->end(), equstates.begin(), equstates.end(), std::back_inserter(intersect));
		std::set_difference(iPS->begin(), iPS->end(), equstates.begin(), equstates.end(), std::back_inserter(difference));
		if (!intersect.empty())
		{
			NextPartition.push_back(intersect);
			NextPartition.push_back(difference);
			int post = SearchInSet(iPS->begin(), iPS->end(), temp);
			if (post != 0)
			{
				NewSplitStates.erase(std::remove(NewSplitStates.begin(), NewSplitStates.end(), NewSplitStates[post]));
				NewSplitStates.push_back(intersect);
				NewSplitStates.push_back(difference);
			}
			else if (intersect.size() <= difference.size())
			{
				NewSplitStates.push_back(intersect);
			}
			else
			{
				NewSplitStates.push_back(difference);
			}
		}
		else 
		{
			NextPartition.push_back(*iPS);
		}
	}

	for (std::vector<std::vector<size_t>>::iterator i = NextPartition.begin(); i != NextPartition.end();)
	{
		if (i->size() == 0)
		{
			i = NextPartition.erase(i);
		}
		else
		{
			++i;
		}
	}

	for (std::vector<std::vector<size_t>>::iterator i = NewSplitStates.begin(); i != NewSplitStates.end();)
	{
		if (i->size() == 0)
		{
			i = NewSplitStates.erase(i);
		}
		else
		{
			++i;
		}
	}
}


void PartitionNonDisState(CDfa &tmpDfa, CNfa &RevTab, std::vector<std::vector<size_t>> &partition, std::vector<size_t> &FinalStas)
{
	std::vector<std::vector<size_t>> NewSplit;
	CDfa tmp = tmpDfa;
	
	for (NewSplit.push_back(FinalStas); !NewSplit.empty(); )
	{
		std::vector<size_t> oneset;
		oneset = NewSplit.front();
		NewSplit.erase(NewSplit.begin());

		for (size_t character = 0; character < CHARSETSIZE; ++character)
		{
			std::vector<size_t> equstates;
			for (std::vector<size_t>::iterator iState = oneset.begin(); iState != oneset.end(); ++iState)
			{
				if (RevTab[*iState][character].Size() != 0)
				{
					for (size_t i = 0; i < RevTab[*iState][character].Size(); ++i)
					{
						equstates.push_back(RevTab[*iState][character][i]);
					}
				}
			}

			if (!equstates.empty())
			{
				std::vector<std::vector<size_t>> NextPartition;
				SubPartition(partition, NewSplit, equstates, NextPartition);
				partition.clear();
				partition = NextPartition;	
			}
		}
	}
}

struct COMPARE
{
	bool operator()(std::vector<size_t> &x, std::vector<size_t> &y)
	{
		 return x.front() < y.front();
	}
};


void MergeNonDisStates(CDfa &tmpDfa, std::vector<std::vector<size_t>> &Partition, CDfa &minDfaTab)
{
	std::sort(Partition.begin(), Partition.end(), COMPARE());
	minDfaTab.Resize(Partition.size());
	size_t k = 0;
	for (size_t i = 0, j = 0; i < Partition.size() && j < tmpDfa.Size(); )
	{
		if (j == Partition[i].front())
		{
			minDfaTab[k++] = tmpDfa[j];			
			minDfaTab[k-1].SetFlag(tmpDfa[j].GetFlag());
			++i;
			++j;
		}
		else
		{
			++j;
		}
	}
	for (std::vector<std::vector<size_t>>::iterator iP = Partition.begin(); iP != Partition.end(); ++iP)
	{
		STATEID tmp = (STATEID)(iP - Partition.begin());
		for (std::vector<size_t>::iterator iS = iP->begin(); iS != iP->end(); ++iS)
		{
			for (size_t i = 0; i < minDfaTab.Size(); ++i)
			{
				for (size_t j = 0; j < CHARSETSIZE; ++j)
				{
					if (minDfaTab[i][j] == *iS)
					{
						minDfaTab[i][j] = tmp;
					}
				}
			}
		}
	}
}

#define CHAR256 256

CREDFA size_t DfaMin(CDfa &oneDfaTab, CDfa &minDfaTab)
{
	if (oneDfaTab.Size() == 0)
	{
		return size_t(-1);
	}

	minDfaTab.SetId(oneDfaTab.GetId());

	// Extract final states from a dfa;
	std::vector<size_t> FinalStas;
	std::vector<size_t> NormalStas;
	for (size_t i = 0; i < oneDfaTab.Size(); ++i)
	{
		if ((oneDfaTab[i].GetFlag() & oneDfaTab[i].TERMINAL) != 0)
		{
			FinalStas.push_back(i);
		}
		else
		{
			NormalStas.push_back(i);
		}
	}

	if (FinalStas.empty())
	{
		return size_t(-2);
	}


	CNfa nfaTab;
	nfaTab.FromDfa(oneDfaTab);

	std::vector<BYTE> reachable;
	std::vector<size_t> StartStas;
	StartStas.push_back(0);

	RemoveUnreachable(nfaTab, StartStas, reachable);

	CNfa revTab;
	revTab.Resize(oneDfaTab.Size());
	STATEID* tmpgroup = oneDfaTab.GetGroup();
	for (size_t i = 0; i < oneDfaTab.Size(); ++i)
	{
		for (size_t j = 0; j < CHAR256; ++j)
		{
			if (oneDfaTab[i][tmpgroup[j]] != -1)
			{
				revTab[oneDfaTab[i][tmpgroup[j]]][j].PushBack(i);
			}
		}
	}
	RemoveUnreachable(revTab, FinalStas, reachable);

	CDfa tmpDfa;
	MergeReachable(oneDfaTab, reachable, tmpDfa);

	//std::vector<std::vector<size_t>> Partition;
	//Partition.push_back(FinalStas);
	//Partition.push_back(NormalStas);

	//PartitionNonDisState(tmpDfa, RevTab, Partition, FinalStas);
	//MergeNonDisStates(tmpDfa, Partition, minDfaTab);

	return 0;
}
