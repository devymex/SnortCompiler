#include "stdafx.h"
#include "OrDfanew.h"
#include "MergeDfanew.h"

//ԭʼ���룬��С���⣬��
MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
{
#undef max

	size_t nTermSta = 0;//�ϲ���nfa��״̬��
	for(size_t i = 0; i < dfas.size(); ++i)
	{
		if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
		{
			return false;
		}
		nTermSta += dfas[i].Size();
	}
	++nTermSta;

	NFALOG nfalog[DFACOLSIZE];//nfa״̬��dfaID�Ķ�Ӧ�� ���������������⣡
	size_t count = 0;

	std::vector<CNfa> nfas;
	
	//STATEID nTermSta = 0;//�ϲ�����״̬��
	CNfa oneNfa;//�ϲ����nfa
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

//ֱ�ӽ����dfa�ϲ���ת����һ��dfa(��nfa��ʾ)���ڶ�nfa���飨Ҳ���Ƕ�dfa���飩ͬʱ��dfa�ı�
//��Ϊdfaֻ��Ψһ����һ״̬��ת����û�п���ת����˲�����ձհ�
//MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
//{
//#undef max
//
//	size_t nTermSta = 0;//�ϲ���nfa��״̬��
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
//	NFALOG nfalog[DFACOLSIZE];//nfa״̬��dfaID�Ķ�Ӧ
//	size_t count = 0;
//
//	typedef std::unordered_map<std::vector<size_t>, STATEID, TODFA_HASH> STATESETHASH;
//	STATESETHASH statehash;
//
//	std::stack<std::vector<size_t> > ;
//	
//	CNfa oneNfa;//�ϲ����nfa
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

