#include "stdafx.h"
#include "OrDfanew.h"
#include "MergeDfanew.h"

//原始代码，有小问题，慢
MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
{
#undef max

	size_t nTermSta = 0;//合并后nfa总状态数
	for(size_t i = 0; i < dfas.size(); ++i)
	{
		if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
		{
			return false;
		}
		nTermSta += dfas[i].Size();
	}
	++nTermSta;

	NFALOG nfalog[DFACOLSIZE];//nfa状态与dfaID的对应， 这样的数组有问题！
	size_t count = 0;

	std::vector<CNfa> nfas;
	
	//STATEID nTermSta = 0;//合并后总状态数
	CNfa oneNfa;//合并后的nfa
	oneNfa.Reserve(nTermSta + 1);
	oneNfa.Resize(1);

	for(size_t i = 0; i < dfas.size(); ++i)
	{
		oneNfa[0].AddDest(EMPTYEDGE, oneNfa.Size() + dfas[i].GetStartId());
		NIncreDfaNum(dfas[i], (size_t)oneNfa.Size());
		NInsertDfa(dfas[i], oneNfa, nTermSta, nfalog, count);
	}

	if(lastDfa.FromNFA(oneNfa, nfalog, count, true) == 0)
	{
		lastDfa.Minimize();
		//size_t nSpaceSize = 0;
		//for(size_t i = 0; i < dfas.size(); ++i)
		//{
		//	nSpaceSize += ((size_t)dfas[i].Size()) * ((size_t)dfas[i].GetGroupCount());
		//}

		//if(((size_t)lastDfa.Size()) * ((size_t)lastDfa.GetGroupCount()) < nSpaceSize)
		//{
		//	return true;
		//}
		//else
		//{
		//	return false;
		//}
		return true;
	}
	else
	{
		return false;
	}
}

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

//直接将多个dfa合并成转换成一个dfa(用nfa表示)，在对nfa分组（也就是对dfa分组）同时填dfa的表
//因为dfa只有唯一的下一状态跳转，且没有空跳转，因此不用求空闭包
MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
{
#undef max

	size_t dfasSize = dfas.size();
	size_t nTermSta = 0;//合并后nfa总状态数
	for(size_t i = 0; i < dfasSize; ++i)
	{
		if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
		{
			return false;
		}
		nTermSta += dfas[i].Size();
	}
	++nTermSta;

	//记录终态和lastDfa状态的对应关系
	struct DID2STATE
	{
		std::vector<STATEID> dfaIds;
		STATEID stateId;
	};

	std::vector<DID2STATE> dfaId2StaId;//记录终态
	typedef std::unordered_map<std::vector<size_t>, STATEID, TODFA_HASH> STATESETHASH;
	STATESETHASH statehash;
	CNfa lastNfa;

	size_t finFlag = 0;//判断是否终态
	std::stack<std::vector<size_t> > statesStack;
	std::vector<size_t> startVec(dfasSize + 2);//使用一个大小为dfas.size() + 1的vector表示合并后的nfa的状态，其中第0个元素表示dfa1的状态，..., 最后两个元素表示虚拟的初始状态0和终止状态nTermSta
	
	DID2STATE dfaIds;
	for(size_t i = 0; i < dfasSize; ++i)
	{
		size_t nSta = dfas[i].GetStartId();
		if((dfas[i][nSta].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			finFlag = 1;
			dfaIds.dfaIds.push_back(dfas[i].GetId());
		}
		startVec[i] = nSta;
	}
	startVec[dfasSize] = 0;//0是添加的虚拟状态
	if(finFlag)
	{
		//说明这是个终态
		startVec[dfasSize + 1] = nTermSta;
	}
	else
	{
		startVec[dfasSize + 1] = -1;
	}
	
	statehash[startVec] = 0;
	dfaIds.stateId = 0;
	dfaId2StaId.push_back(dfaIds);

	statesStack.push(startVec);

	std::vector<size_t> NextVec;

	while(!statesStack.empty())
	{
		std::vector<size_t> curVec = statesStack.top();//当前状态集合, curVec共有dfasSize + 2个状态
		statesStack.pop();

		STATESETHASH::iterator ir = statehash.find(curVec);
		if (ir == statehash.end())
		{
			std::cout << "Fatal Error!" << std::endl;
			break;
		}
		STATEID curStaNum = ir->second;
		lastNfa.Resize(lastNfa.Size() + 1);

		//下一状态集合
		for(size_t curChar = 0; curChar < DFACOLSIZE; ++curChar)
		{
			DID2STATE dfaStaIDs;
			finFlag = 0;
			NextVec.clear();
			NextVec.resize(dfasSize + 2);

			for(size_t i = 0; i < dfasSize; ++i)
			{
				BYTE curgroup = dfas[i].Char2Group(curChar);
				STATEID nextId = dfas[i][curVec[i]][curgroup];//第i个dfa从curVec[i]状态经过curChar跳转的下一状态
				if((dfas[i][nextId].GetFlag() & CDfaRow::TERMINAL) != 0)
				{
					//判断是否终态
					finFlag = 1;
					dfaStaIDs.dfaIds.push_back(dfas[i].GetId());
				}
				NextVec[i] = nextId; 
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

			if(!NextVec.empty())
			{
				if(statehash.count(NextVec) == 0)
				{
#undef max
					if (statehash.size() > std::numeric_limits<STATEID>::max())
					{
						std::cerr << "Fatal Error!" << std::endl;
						return (size_t)-1;
					}
					STATEID nextSta = (STATEID)statehash.size();
					statehash[NextVec] = nextSta;

					dfaStaIDs.stateId = nextSta;

					lastNfa[curStaNum].AddDest(curChar, nextSta);
					
					statesStack.push(NextVec);
				}
				else
				{
					dfaStaIDs.stateId = statehash[NextVec];
					lastNfa[curStaNum].AddDest(curChar, dfaStaIDs.stateId);
				}
				dfaId2StaId.push_back(dfaStaIDs);
			}
			else
			{
				lastNfa[curStaNum].AddDest(curChar, (STATEID)-1);
			}
		}
	}

	//对lastNfa分组，填lastDfa表
	BYTE groups[DFACOLSIZE];
	NAvaiEdges(lastNfa, groups);
	lastDfa.Init(groups);
	lastDfa.ReservRow(lastNfa.Size());

	//将lastNfa拷贝到lastDfa中
	for(size_t i = 0; i < lastNfa.Size(); ++i)
	{
		std::vector<size_t> computFlag(lastDfa.GetGroupCount(), 0);
		for(size_t c = 0; c < DFACOLSIZE; ++c)
		{
			BYTE curG = lastDfa.Char2Group(c);
			if(computFlag[c] == 0)
			{
				computFlag[c] = 1;
				lastDfa.ResizeRow(lastDfa.Size() + 1);
				lastDfa.BackRow()[c] = lastNfa[i].GetDest(c, 0);
			}
		}
	}
}

//处理终态

