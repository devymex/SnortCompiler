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

//按照每个dfa的m_pGroup进行分组
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

//根据other的otherSta查找termset，将找到的TERMSET插入到的lastDfa的m_TermSet中，其中lastDfa的状态是lastSta中
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

//可以先根据每个dfa的分组情况，通过hash将最终的lastDfa进行分组
MERDFANEW bool NOrMerge(std::vector<CDfaNew> &dfas, CDfaNew &lastDfa)
{
	size_t dfaId = lastDfa.GetId();
	lastDfa.Clear();
	lastDfa.SetId(dfaId);
#undef max

	size_t dfasSize = dfas.size();
	size_t nTermSta = 1;//终态标记

	//对lastDfa分组
	BYTE groups[DFACOLSIZE];
	DfaColGroup(dfas, groups);
	lastDfa.Init(groups);

	size_t colCnt = lastDfa.GetGroupCount();

	typedef std::unordered_map<std::vector<size_t>, STATEID, TODFA_HASH> STATESETHASH;
	STATESETHASH statehash;

	size_t finFlag = 0;//判断是否终态
	std::stack<std::vector<size_t> > statesStack;//测试
	std::vector<size_t> startVec(dfasSize + 2);//使用一个大小为dfas.size() + 2的vector表示合并后的nfa的状态，其中第0个元素表示dfa1的状态，..., 最后两个元素表示虚拟的初始状态0和终止状态nTermSta
	
	lastDfa.ReservRow(CHARSETSIZE);
	lastDfa.ResizeRow(lastDfa.Size() + 1, colCnt);

	for(size_t i = 0; i < dfasSize; ++i)
	{
		STATEID nSta = dfas[i].GetStartId();
		if((dfas[i][nSta].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			//是终态
			finFlag = 1;
			//将dfas[i]根据nSta找到的dfaId插入到lastDfa的0状态对应的TERMSET中
			if(AddTermIntoDFA(nSta, dfas[i], 0, lastDfa) != 0)
			{
				std::cout << "Termset Error!" << std::endl;
				return false;
			}
		}
		startVec[i] = nSta;
	}
	startVec[dfasSize] = 0;//0是添加的虚拟状态
	if(finFlag)
	{
		//说明这是个终态
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
		std::vector<size_t> curVec = statesStack.top();//当前状态集合, curVec共有dfasSize + 2个状态
		statesStack.pop();
		
		STATESETHASH::iterator ir = statehash.find(curVec);
		if (ir == statehash.end())
		{
			std::cout << "hash Error!" << std::endl;
			return false;
		}
		STATEID curStaNum = ir->second;
		ZeroMemory(computFlag, sizeof(computFlag));

		//下一状态集合
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

			size_t flag = 0;//标记下一状态集是否为空
			for(size_t i = 0; i < dfasSize; ++i)
			{
				STATEID sta = curVec[i];
				
				if(sta != (STATEID)-1)
				{
					BYTE curgroup = dfas[i].Char2Group(curChar);
					STATEID nextId = dfas[i][sta][curgroup];//第i个dfa从curVec[i]状态经过curChar跳转的下一状态
					if(nextId != (STATEID)-1)
					{
						flag = 1;
						if((dfas[i][nextId].GetFlag() & CDfaRow::TERMINAL) != 0)
						{
							//判断是否终态
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
				//该状态是终态
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
					lastDfa.ResizeRow(lastDfa.Size() + 1, colCnt);
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

	//对lastDfa进行进一步按列分组
	lastDfa.Minimize();
	if(lastDfa.Size() > DFA_SIZE_LIMIT)
	{
		//std::cerr << "DFA_SIZE_LIMIT!" << std::endl;
		return false;
	}

	return true;
}

