#include "stdafx.h"
#include "OrDfanew.h"
#include "MergeDfanew.h"

//原始代码，有小问题，慢
//MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
//{
//	CTimer c;//用于测试
//#undef max
//
//	size_t nTermSta = 0;//合并后nfa总状态数
//	for(size_t i = 0; i < dfas.size(); ++i)
//	{
//		if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
//		{
//			return false;
//		}
//		nTermSta += dfas[i].Size();
//	}
//	++nTermSta;
//
//	NFALOG nfalog[DFACOLSIZE];//nfa状态与dfaID的对应， 这样的数组有问题！
//	size_t count = 0;
//
//	std::vector<CNfa> nfas;
//	
//	//STATEID nTermSta = 0;//合并后总状态数
//	CNfa oneNfa;//合并后的nfa
//	oneNfa.Reserve(nTermSta + 1);
//	oneNfa.Resize(1);
//
//	for(size_t i = 0; i < dfas.size(); ++i)
//	{
//		oneNfa[0].AddDest(EMPTYEDGE, oneNfa.Size() + dfas[i].GetStartId());
//		NIncreDfaNum(dfas[i], (size_t)oneNfa.Size());
//		NInsertDfa(dfas[i], oneNfa, nTermSta, nfalog, count);
//	}
//
//	size_t tmp = lastDfa.FromNFA(oneNfa, nfalog, count, true);
//	//std::cout << "方法一合并dfa用时：" << c.Reset() << std::endl;
//
//	//if(lastDfa.FromNFA(oneNfa, nfalog, count, true) == 0)
//	if(tmp == 0)
//	{
//		lastDfa.Minimize();
//		//std::cout << "方法一lastDfa最小化用时: " << c.Reset() << std::endl;
//
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

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
void DfaColGroup(std::vector<CDfanew> &dfas, BYTE* groups)
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
	typedef std::unordered_map<GROUPKEY, size_t, GROUPKEYHASH> GROUPMAP;;
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

//可以先根据每个dfa的分组情况，通过hash将最终的lastDfa进行分组
MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
{
	lastDfa.Clear();
	//CTimer mergtime;//用于测试
#undef max

	size_t dfasSize = dfas.size();
	size_t nTermSta = 0;//合并后nfa总状态数,也是nfa的终态编号
	for(size_t i = 0; i < dfasSize; ++i)
	{
		if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
		{
			return false;
		}
		nTermSta += dfas[i].Size();
	}
	++nTermSta;

	//对lastDfa分组
	BYTE groups[DFACOLSIZE];
	DfaColGroup(dfas, groups);
	lastDfa.Init(groups);

	size_t colCnt = lastDfa.GetGroupCount();

	typedef std::unordered_map<std::vector<size_t>, STATEID, TODFA_HASH> STATESETHASH;
	STATESETHASH statehash;

	size_t finFlag = 0;//判断是否终态
	std::deque<std::vector<size_t> > statesStack;
	std::vector<size_t> startVec(dfasSize + 2);//使用一个大小为dfas.size() + 2的vector表示合并后的nfa的状态，其中第0个元素表示dfa1的状态，..., 最后两个元素表示虚拟的初始状态0和终止状态nTermSta
	
	lastDfa.ResizeRow(lastDfa.Size() + 1, colCnt);

	for(size_t i = 0; i < dfasSize; ++i)
	{
		size_t nSta = dfas[i].GetStartId();
		if((dfas[i][nSta].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			//是终态
			finFlag = 1;
			//dfaIds.dfaIds.push_back(dfas[i].GetId());
			lastDfa.PushBackTermSet(TERMSET());
			lastDfa.BackTermSet().dfaId = dfas[i].GetId();
			lastDfa.BackTermSet().dfaId = 0;
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

	statesStack.push_back(startVec);

	std::vector<size_t> NextVec;
	BYTE computFlag[CHARSETSIZE];

	while(!statesStack.empty())
	{
		//if(lastDfa.Size() > std::numeric_limits<STATEID>::max())
		//{
		//	return (size_t)-1;
		//}
		//std::cout << (size_t)lastDfa.Size() << std::endl;//用于测试

		std::vector<size_t> curVec = statesStack.front();//当前状态集合, curVec共有dfasSize + 2个状态
		statesStack.pop_front();

		STATESETHASH::iterator ir = statehash.find(curVec);
		if (ir == statehash.end())
		{
			std::cout << "hash Error!" << std::endl;
			return false;
		}
		STATEID curStaNum = ir->second;
		memset(computFlag, 0, sizeof(computFlag));

		//下一状态集合
		for(size_t curChar = 0; curChar < DFACOLSIZE; ++curChar)
		{
			finFlag = 0;
			NextVec.clear();
			NextVec.resize(dfasSize + 2);

			size_t lastDfaGroup = lastDfa.Char2Group(curChar);
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
					//if (statehash.size() >= std::numeric_limits<STATEID>::max())
					//{
					//	std::cerr << "Size limit!" << std::endl;
					//	return false;
					//}
					STATEID nextSta = (STATEID)statehash.size();
					statehash[NextVec] = nextSta;
					lastDfa.ResizeRow(lastDfa.Size() + 1, colCnt);
					std::cout << (size_t)lastDfa.Size() << std::endl;//用于测试
					if(lastDfa.Size() >= SC_STATELIMIT)
					{
						std::cerr << "SC_STATELIMIT!" << std::endl;
						return false;
					}
					//lastDfa.ResizeRow(nextSta + 1, colCnt);

					if(finFlag)
					{
						for(size_t k = 0; k < dfasSize; ++k)
						{
							if(NextVec[k] != (STATEID)-1)
							{
								if((dfas[k][NextVec[k]].GetFlag() & CDfaRow::TERMINAL) != 0)
								{
									lastDfa.PushBackTermSet(TERMSET());
									lastDfa.BackTermSet().dfaId = dfas[k].GetId();
									lastDfa.BackTermSet().dfaSta = nextSta;
								}
							}
						}
						lastDfa[nextSta].SetFlag(lastDfa[nextSta].GetFlag() | CDfaRow::TERMINAL);
					}

					lastDfa[curStaNum][lastDfaGroup] = nextSta;
					
					statesStack.push_back(NextVec);
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

	//std::cout << "方法二合并dfa用时: " << mergtime.Reset() << std::endl;//用于测试

	lastDfa.Minimize();
	//std::cout << "方法二lastDfa最小化用时：" << mergtime.Reset() << std::endl;//用于测试
	return true;
}

