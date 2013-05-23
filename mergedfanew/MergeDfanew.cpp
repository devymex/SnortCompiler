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
//MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
//{
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
//	NFALOG nfalog[DFACOLSIZE];//nfa状态与dfaID的对应
//	size_t count = 0;
//
//	typedef std::unordered_map<std::vector<size_t>, STATEID, TODFA_HASH> STATESETHASH;
//	STATESETHASH statehash;
//
//	std::stack<std::vector<size_t> > ;
//	
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
//	if(lastDfa.FromNFA(oneNfa, nfalog, count, true) == 0)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

