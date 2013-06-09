/**
**  @file        MergeDfaNew.cpp
**
**  @author      Lab 435, Xidian University
**
**  @brief       merge mutiple dfas into one dfa and
**				 mark the merged dfa's terminal states
**
*/

#include "stdafx.h"
#include "MergeDfanew.h"

struct TODFA_HASH
{
	size_t operator()(const std::vector<size_t> &vec)
	{
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;

		size_t _Val = _FNV_offset_basis;
		for (size_t _Next = 0; _Next < vec.size(); ++_Next)
			{	
				_Val ^= vec[_Next];
				_Val *= _FNV_prime;
			}

		return (_Val);
	}
};

struct GROUPKEY
{
	std::vector<size_t> key;
	size_t hash;
	__forceinline bool operator == (const GROUPKEY &other) const
	{
		size_t nSize = key.size();
		if (nSize != other.key.size())
		{
			return false;
		}
		if (nSize == 0)
		{
			return true;
		}
		return (0 == memcmp(key.data(), other.key.data(), nSize * sizeof(size_t)));
	}
};


/*
**	This function groups the merged dfa's columns on the ground of
**	the column groups of dfas to be merged.
**
**	We use a hash function to group.
**	
**	First of all, merge the dfas's column groups into a table of n*256, n means the size of dfas
**	Then hash the table columns to get the merged dfa's colums group.
*/
void DfaColGroup(std::vector<CDfaNew> &dfas, BYTE* groups)
{
	struct GROUPKEYHASH
	{
		__forceinline size_t operator ()(const GROUPKEY &column)
		{
			return column.hash;
		}
	};

	const size_t _FNV_offset_basis = 2166136261U;
	const size_t _FNV_prime = 16777619U;

	GROUPKEY colum[CHARSETSIZE];
	typedef std::unordered_map<GROUPKEY, BYTE, GROUPKEYHASH> GROUPMAP;;
	GROUPMAP groupMap;

	for(size_t c = 0; c < DFACOLSIZE; ++c)
	{
		colum[c].key.clear();
		colum[c].hash = _FNV_offset_basis;
	}

	for(size_t i = 0; i < dfas.size(); ++i)
	{
		for(size_t c = 0; c < DFACOLSIZE; ++c)
		{
			size_t group = dfas[i].Char2Group(c);
			colum[c].key.push_back(group);
			colum[c].hash ^= group;
			colum[c].hash *= _FNV_prime;
		}
	}

	for(size_t c = 0; c < DFACOLSIZE; ++c)
	{
		GROUPKEY &curCol = colum[c];
		GROUPMAP::iterator it = groupMap.find(curCol);
		if(it == groupMap.end())
		{
			size_t curId = groupMap.size();
			groups[c] = curId;
			groupMap[curCol] = curId;
		}
		else
		{
			groups[c] = it->second;
		}
	}
}

/*
**	this function marks the lastDfa's terminal states.
**	
**  @param otherSta	   the state to be found in other
**  @param other       one dfa to be merged
**	@param lastSta     the state to be marked
**	@param lastDfa     the merged dfa
**
**  @return a integer
**
**  @retval 0 function successful
**  @retval -1 fatal error
*/
size_t AddTermIntoDFA(STATEID otherSta, CDfaNew &other, STATEID lastSta, CDfaNew &lastDfa)
{
	size_t flag = (size_t)-1;
	for(size_t i = 0; i < other.GetTermCnt(); ++i)
	{
		if(other.GetTerm(i).dfaSta == otherSta)
		{
			flag = 0;
			lastDfa.PushBackTermSet(TERMSET());
			lastDfa.BackTermSet().dfaSta = lastSta;
			lastDfa.BackTermSet().dfaId = other.GetTerm(i).dfaId;
		}
	}
	return flag;
}

/*
**  NAME
**    MergeMultipleDfas::
*/
/**
**  This function merges mutiple dfas into one dfa. And mark the terminal states to 
**	distinguish which dfas the terminal state belongs to.
**
**	In order to speed up, we need one support function:DfaColGroup to group
**	the lastDfa's columns.
**
**  @param dfas      a vector contains mutiple CDfaNew
**  @param lastDfa   the merged dfa
**
**  @return bool
**
**  @retval true function successful
**  @retval fasle fatal error
*/

MERDFANEW bool MergeMultipleDfas(std::vector<CDfaNew> &dfas, CDfaNew &lastDfa)
{
	size_t dfaId = lastDfa.GetId();
	lastDfa.Clear();
	lastDfa.SetId(dfaId);
#undef max

	size_t dfasSize = dfas.size();
	size_t nTermSta = 1;//nTermSta: terminal flag. 1: terminal, -1: non-terminal

	//group the lastDfa's columns
	BYTE groups[DFACOLSIZE];
	DfaColGroup(dfas, groups);
	lastDfa.Init(groups);

	size_t colCnt = lastDfa.GetGroupCount();

	typedef std::unordered_map<std::vector<size_t>, STATEID, TODFA_HASH> STATESETHASH;
	STATESETHASH statehash;

	size_t finFlag = 0;//terminal state flag, 1: terminal state, 0: normal state
	std::stack<std::vector<size_t> > statesStack;

	/*
	**	use a size of (dfas.size() + 2) vector to represent a state of the merged dfa.
	**	element 0 represents the state of dfas[0], ..., element n represents the state of dfas[n].
	**	element n + 1 and element n + 2 are flags which show that whether this state is a start state or terminal state.
	*/
	std::vector<size_t> startVec(dfasSize + 2);
	
	lastDfa.Reserve(CHARSETSIZE);
	lastDfa.Resize(lastDfa.Size() + 1, colCnt);

	for(size_t i = 0; i < dfasSize; ++i)
	{
		STATEID nSta = dfas[i].GetStartId();
		if((dfas[i][nSta].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			//this is a terminal state
			finFlag = 1;
			if(AddTermIntoDFA(nSta, dfas[i], 0, lastDfa) != 0)
			{
				std::cout << "Termset Error!" << std::endl;
				return false;
			}
		}
		startVec[i] = nSta;
	}
	startVec[dfasSize] = 0;//this is start state
	if(finFlag)
	{
		startVec[dfasSize + 1] = nTermSta;
		lastDfa[0].SetFlag(lastDfa[0].GetFlag() | CDfaRow::START | CDfaRow::TERMINAL);
	}
	else
	{
		startVec[dfasSize + 1] = -1;
		lastDfa[0].SetFlag(lastDfa[0].GetFlag() | CDfaRow::START);
	}
	
	statehash[startVec] = 0;

	statesStack.push(startVec);

	std::vector<size_t> NextVec;
	NextVec.resize(dfasSize + 2);
	BYTE computFlag[CHARSETSIZE];

	while(!statesStack.empty())
	{
		std::vector<size_t> curVec = statesStack.top();//current state, size is:dfasSize + 2
		statesStack.pop();
		
		STATESETHASH::iterator ir = statehash.find(curVec);
		if (ir == statehash.end())
		{
			std::cout << "hash Error!" << std::endl;
			return false;
		}
		STATEID curStaNum = ir->second;
		ZeroMemory(computFlag, sizeof(computFlag));

		//get next states
		for(size_t curChar = 0; curChar < DFACOLSIZE; ++curChar)
		{
			finFlag = 0;
			ZeroMemory(NextVec.data(), NextVec.size() * sizeof(size_t));
			BYTE lastDfaGroup = groups[curChar];
			if(computFlag[lastDfaGroup] == 1)
			{
				continue;
			}
			computFlag[lastDfaGroup] = 1;

			size_t flag = 0;//flag = 0: empty next state
			for(size_t i = 0; i < dfasSize; ++i)
			{
				STATEID sta = curVec[i];
				
				if(sta != (STATEID)-1)
				{
					BYTE curgroup = dfas[i].Char2Group(curChar);
					STATEID nextId = dfas[i][sta][curgroup];//the next state the ith dfa transforms from state curVec[i] through curChar to
					if(nextId != (STATEID)-1)
					{
						flag = 1;
						if((dfas[i][nextId].GetFlag() & CDfaRow::TERMINAL) != 0)
						{
							finFlag = 1;
						}
					}
					NextVec[i] = nextId; 
				}
				else
				{
					NextVec[i] = (STATEID)-1;
				}
			}
			NextVec[dfasSize] = -1;
			if(finFlag)
			{
				NextVec[dfasSize + 1] = nTermSta;
			}
			else
			{
				NextVec[dfasSize + 1] = -1;
			}

			if(flag)
			{
				STATESETHASH::iterator nextIt = statehash.find(NextVec);
				if(nextIt == statehash.end())
				{
#undef max 
					lastDfa.Resize(lastDfa.Size() + 1, colCnt);
					if(lastDfa.Size() > SC_STATELIMIT)
					{
						//std::cerr << "SC_STATELIMIT!" << std::endl;
						return false;
					}
					STATEID nextSta = (STATEID)statehash.size();
					statehash[NextVec] = nextSta;
					lastDfa[curStaNum][lastDfaGroup] = nextSta;

					if(finFlag)
					{
						for(size_t k = 0; k < dfasSize; ++k)
						{
							if(NextVec[k] != (STATEID)-1)
							{
								if((dfas[k][NextVec[k]].GetFlag() & CDfaRow::TERMINAL) != 0)
								{
									if(AddTermIntoDFA(NextVec[k], dfas[k], nextSta, lastDfa) != 0)
									{
										std::cout << "Termset error!" << std::endl;
										return false;
									}
								}
							}
						}
						lastDfa[nextSta].SetFlag(lastDfa[nextSta].GetFlag() | CDfaRow::TERMINAL);
					}
				
					statesStack.push(NextVec);
				}
				else
				{
					lastDfa[curStaNum][lastDfaGroup] = nextIt->second;
				}
			}
			else
			{
				lastDfa[curStaNum][lastDfaGroup] = (STATEID)-1;
			}
		}
	}
	lastDfa.UniqueTermSet();
	lastDfa.Minimize();
	if(lastDfa.Size() > DFA_SIZE_LIMIT)
	{
		//std::cerr << "DFA_SIZE_LIMIT!" << std::endl;
		return false;
	}

	return true;
}

