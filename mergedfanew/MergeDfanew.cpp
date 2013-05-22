#include "stdafx.h"
#include "OrDfanew.h"
#include "MergeDfanew.h"

//ԭʼ���룬��С���⣬��
MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
{
	CTimer c;//���ڲ���
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

	size_t tmp = lastDfa.FromNFA(oneNfa, nfalog, count, true);
	std::cout << "����һ�ϲ�dfa��ʱ��" << c.Reset() << std::endl;

	//if(lastDfa.FromNFA(oneNfa, nfalog, count, true) == 0)
	if(tmp == 0)
	{
		lastDfa.Minimize();
		std::cout << "����һlastDfa��С����ʱ: " << c.Reset() << std::endl;
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

//����ÿ��dfa��m_pGroup���з���
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

//�����ȸ���ÿ��dfa�ķ��������ͨ��hash�����յ�lastDfa���з���
//MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
//{
//	CTimer mergtime;//���ڲ���
//#undef max
//
//	size_t dfasSize = dfas.size();
//	size_t nTermSta = 0;//�ϲ���nfa��״̬��,Ҳ��nfa����̬���
//	for(size_t i = 0; i < dfasSize; ++i)
//	{
//		if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
//		{
//			return false;
//		}
//		nTermSta += dfas[i].Size();
//	}
//	++nTermSta;
//
//	//��lastDfa����
//	BYTE groups[DFACOLSIZE];
//	DfaColGroup(dfas, groups);
//	lastDfa.Init(groups);
//
//	size_t colCnt = lastDfa.GetGroupCount();
//
//	typedef std::unordered_map<std::vector<size_t>, STATEID, TODFA_HASH> STATESETHASH;
//	STATESETHASH statehash;
//
//	size_t finFlag = 0;//�ж��Ƿ���̬
//	std::deque<std::vector<size_t> > statesStack;
//	std::vector<size_t> startVec(dfasSize + 2);//ʹ��һ����СΪdfas.size() + 2��vector��ʾ�ϲ����nfa��״̬�����е�0��Ԫ�ر�ʾdfa1��״̬��..., �������Ԫ�ر�ʾ����ĳ�ʼ״̬0����ֹ״̬nTermSta
//	
//	lastDfa.ResizeRow(lastDfa.Size() + 1, colCnt);
//
//	for(size_t i = 0; i < dfasSize; ++i)
//	{
//		size_t nSta = dfas[i].GetStartId();
//		if((dfas[i][nSta].GetFlag() & CDfaRow::TERMINAL) != 0)
//		{
//			//����̬
//			finFlag = 1;
//			//dfaIds.dfaIds.push_back(dfas[i].GetId());
//			lastDfa.PushBackTermSet(TERMSET());
//			lastDfa.BackTermSet().dfaId = dfas[i].GetId();
//			lastDfa.BackTermSet().dfaId = 0;
//		}
//		startVec[i] = nSta;
//	}
//	startVec[dfasSize] = 0;//0����ӵ�����״̬
//	if(finFlag)
//	{
//		//˵�����Ǹ���̬
//		startVec[dfasSize + 1] = nTermSta;
//		lastDfa[0].SetFlag(CDfaRow::START | CDfaRow::TERMINAL);
//	}
//	else
//	{
//		startVec[dfasSize + 1] = -1;
//	}
//	
//	statehash[startVec] = 0;
//
//	statesStack.push_back(startVec);
//
//	std::vector<size_t> NextVec;
//	BYTE computFlag[CHARSETSIZE];
//
//	while(!statesStack.empty())
//	{
//		std::vector<size_t> curVec = statesStack.front();//��ǰ״̬����, curVec����dfasSize + 2��״̬
//		statesStack.pop_front();
//
//		STATESETHASH::iterator ir = statehash.find(curVec);
//		if (ir == statehash.end())
//		{
//			std::cout << "Fatal Error!" << std::endl;
//			break;
//		}
//		STATEID curStaNum = ir->second;
//		memset(computFlag, 0, sizeof(computFlag));
//
//		//��һ״̬����
//		for(size_t curChar = 0; curChar < DFACOLSIZE; ++curChar)
//		{
//			//DID2STATE dfaStaIDs;
//			finFlag = 0;
//			NextVec.clear();
//			NextVec.resize(dfasSize + 2);
//
//			size_t lastDfaGroup = lastDfa.Char2Group(curChar);
//			if(computFlag[lastDfaGroup] == 1)
//			{
//				continue;
//			}
//			computFlag[lastDfaGroup] = 1;
//
//			size_t flag = 0;//�����һ״̬���Ƿ�Ϊ��
//			for(size_t i = 0; i < dfasSize; ++i)
//			{
//				STATEID sta = curVec[i];
//				
//				if(sta != (STATEID)-1)
//				{
//					BYTE curgroup = dfas[i].Char2Group(curChar);
//					STATEID nextId = dfas[i][sta][curgroup];//��i��dfa��curVec[i]״̬����curChar��ת����һ״̬
//					if(nextId != (STATEID)-1)
//					{
//						flag = 1;
//						if((dfas[i][nextId].GetFlag() & CDfaRow::TERMINAL) != 0)
//						{
//							//�ж��Ƿ���̬
//							finFlag = 1;
//						}
//					}
//					NextVec[i] = nextId; 
//				}
//				else
//				{
//					NextVec[i] = (STATEID)-1;
//				}
//			}
//			NextVec[dfasSize] = -1;
//			if(finFlag)
//			{
//				//��״̬����̬
//				NextVec[dfasSize + 1] = nTermSta;
//			}
//			else
//			{
//				NextVec[dfasSize + 1] = -1;
//			}
//
//			if(flag)
//			{
//				STATESETHASH::iterator nextIt = statehash.find(NextVec);
//				if(nextIt == statehash.end())
//				{
//#undef max
//					if (statehash.size() > std::numeric_limits<STATEID>::max())
//					{
//						std::cerr << "Fatal Error!" << std::endl;
//						return (size_t)-1;
//					}
//					STATEID nextSta = (STATEID)statehash.size();
//					statehash[NextVec] = nextSta;
//					lastDfa.ResizeRow(lastDfa.Size() + 1, colCnt);
//
//					if(finFlag)
//					{
//						for(size_t k = 0; k < dfasSize; ++k)
//						{
//							if(NextVec[k] != (STATEID)-1)
//							{
//								if((dfas[k][NextVec[k]].GetFlag() & CDfaRow::TERMINAL) != 0)
//								{
//									lastDfa.PushBackTermSet(TERMSET());
//									lastDfa.BackTermSet().dfaId = dfas[k].GetId();
//									lastDfa.BackTermSet().dfaSta = nextSta;
//								}
//							}
//						}
//						lastDfa[nextSta].SetFlag(lastDfa[nextSta].GetFlag() | CDfaRow::TERMINAL);
//					}
//
//					lastDfa[curStaNum][lastDfaGroup] = nextSta;
//					
//					statesStack.push_back(NextVec);
//				}
//				else
//				{
//					lastDfa[curStaNum][lastDfaGroup] = nextIt->second;
//				}
//			}
//			else
//			{
//				lastDfa[curStaNum][lastDfaGroup] = (STATEID)-1;
//			}
//		}
//	}
//
//	std::cout << "�������ϲ�dfa��ʱ: " << mergtime.Reset() << std::endl;//���ڲ���
//
//	lastDfa.Minimize();
//	std::cout << "������lastDfa��С����ʱ��" << mergtime.Reset() << std::endl;//���ڲ���
//}






//ֱ�ӽ����dfa�ϲ���ת����һ��dfa(��nfa��ʾ)���ڶ�nfa���飨Ҳ���Ƕ�dfa���飩ͬʱ��dfa�ı�
//��Ϊdfaֻ��Ψһ����һ״̬��ת����û�п���ת����˲�����ձհ�
//MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
//{
//	CTimer mergtime;//���ڲ���
//#undef max
//
//	size_t dfasSize = dfas.size();
//	size_t nTermSta = 0;//�ϲ���nfa��״̬��,Ҳ��nfa����̬���
//	for(size_t i = 0; i < dfasSize; ++i)
//	{
//		if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
//		{
//			return false;
//		}
//		nTermSta += dfas[i].Size();
//	}
//	++nTermSta;
//
//	//��¼��̬��lastDfa״̬�Ķ�Ӧ��ϵ
//	struct DID2STATE
//	{
//		std::vector<STATEID> dfaIds;
//		STATEID stateId;
//	};
//
//	std::vector<DID2STATE> dfaId2StaId;//��¼��̬
//	typedef std::unordered_map<std::vector<size_t>, STATEID, TODFA_HASH> STATESETHASH;
//	STATESETHASH statehash;
//	CNfa lastNfa;
//
//	size_t finFlag = 0;//�ж��Ƿ���̬
//	std::deque<std::vector<size_t> > statesStack;
//	std::vector<size_t> startVec(dfasSize + 2);//ʹ��һ����СΪdfas.size() + 2��vector��ʾ�ϲ����nfa��״̬�����е�0��Ԫ�ر�ʾdfa1��״̬��..., �������Ԫ�ر�ʾ����ĳ�ʼ״̬0����ֹ״̬nTermSta
//	
//	DID2STATE dfaIds;
//	for(size_t i = 0; i < dfasSize; ++i)
//	{
//		size_t nSta = dfas[i].GetStartId();
//		if((dfas[i][nSta].GetFlag() & CDfaRow::TERMINAL) != 0)
//		{
//			//����̬
//			finFlag = 1;
//			dfaIds.dfaIds.push_back(dfas[i].GetId());
//		}
//		startVec[i] = nSta;
//	}
//	startVec[dfasSize] = 0;//0����ӵ�����״̬
//	if(finFlag)
//	{
//		//˵�����Ǹ���̬
//		startVec[dfasSize + 1] = nTermSta;
//	}
//	else
//	{
//		startVec[dfasSize + 1] = -1;
//	}
//	
//	statehash[startVec] = 0;
//	if(finFlag)
//	{
//		dfaIds.stateId = 0;
//		dfaId2StaId.push_back(dfaIds);
//	}
//
//	statesStack.push_back(startVec);
//
//	std::vector<size_t> NextVec;
//
//	while(!statesStack.empty())
//	{
//		std::vector<size_t> curVec = statesStack.front();//��ǰ״̬����, curVec����dfasSize + 2��״̬
//		statesStack.pop_front();
//
//		STATESETHASH::iterator ir = statehash.find(curVec);
//		if (ir == statehash.end())
//		{
//			std::cout << "Fatal Error!" << std::endl;
//			break;
//		}
//		STATEID curStaNum = ir->second;
//		lastNfa.Resize(lastNfa.Size() + 1);
//
//		//��һ״̬����
//		for(size_t curChar = 0; curChar < DFACOLSIZE; ++curChar)
//		{
//			DID2STATE dfaStaIDs;
//			finFlag = 0;
//			NextVec.clear();
//			NextVec.resize(dfasSize + 2);
//
//			size_t flag = 0;//�����һ״̬���Ƿ�Ϊ��
//			for(size_t i = 0; i < dfasSize; ++i)
//			{
//				STATEID sta = curVec[i];
//				
//				if(sta != (STATEID)-1)
//				{
//					BYTE curgroup = dfas[i].Char2Group(curChar);
//					STATEID nextId = dfas[i][sta][curgroup];//��i��dfa��curVec[i]״̬����curChar��ת����һ״̬
//					if(nextId != (STATEID)-1)
//					{
//						flag = 1;
//						if((dfas[i][nextId].GetFlag() & CDfaRow::TERMINAL) != 0)
//						{
//							//�ж��Ƿ���̬
//							finFlag = 1;
//							dfaStaIDs.dfaIds.push_back(dfas[i].GetId());
//						}
//					}
//					NextVec[i] = nextId; 
//				}
//				else
//				{
//					NextVec[i] = (STATEID)-1;
//				}
//			}
//			NextVec[dfasSize] = -1;
//			if(finFlag)
//			{
//				//��״̬����̬
//				NextVec[dfasSize + 1] = nTermSta;
//			}
//			else
//			{
//				NextVec[dfasSize + 1] = -1;
//			}
//
//			if(flag)
//			{
//				STATESETHASH::iterator nextIt = statehash.find(NextVec);
//				if(nextIt == statehash.end())
//				{
//#undef max
//					if (statehash.size() > std::numeric_limits<STATEID>::max())
//					{
//						std::cerr << "Fatal Error!" << std::endl;
//						return (size_t)-1;
//					}
//					STATEID nextSta = (STATEID)statehash.size();
//					statehash[NextVec] = nextSta;
//
//					if(finFlag)
//					{
//						dfaStaIDs.stateId = nextSta;
//						dfaId2StaId.push_back(dfaStaIDs);
//					}
//
//					lastNfa[curStaNum].AddDest(curChar, nextSta);
//					
//					statesStack.push_back(NextVec);
//				}
//				else
//				{
//					lastNfa[curStaNum].AddDest(curChar, nextIt->second);
//				}
//			}
//			else
//			{
//				lastNfa[curStaNum].AddDest(curChar, (STATEID)-1);
//			}
//		}
//	}
//
//	std::cout << "����һ���ȼ۵�nfa: " << mergtime.Reset() << std::endl;//���ڲ���
//
//	//��lastNfa���飬��lastDfa��
//	BYTE groups[DFACOLSIZE];
//	NAvaiEdges(lastNfa, groups);
//	lastDfa.Init(groups);
//	lastDfa.ReservRow(lastNfa.Size());
//	size_t colCnt = lastDfa.GetGroupCount();
//
//	//��lastNfa������lastDfa��
//	size_t computFlag[CHARSETSIZE];
//	for(size_t i = 0; i < lastNfa.Size(); ++i)
//	{
//		lastDfa.ResizeRow(lastDfa.Size() + 1, colCnt);
//		memset(computFlag, 0, sizeof(computFlag));
//		for(size_t c = 0; c < DFACOLSIZE; ++c)
//		{
//			BYTE curG = lastDfa.Char2Group(c);
//			if(computFlag[curG] == 0)
//			{
//				computFlag[curG] = 1;
//				STATEID tmpId = lastNfa[i].GetDest(c, 0);
//				lastDfa.BackRow()[curG] = lastNfa[i].GetDest(c, 0);
//			}
//		}
//		if(i == 0)
//		{
//			lastDfa[0].SetFlag(lastDfa[0].GetFlag() | CDfaRow::START);
//		}
//	}
//	std::cout << "��nfa������dfa��" << mergtime.Reset() << std::endl;//���ڲ���
//
//	//������̬
//	for(std::vector<DID2STATE>::iterator it = dfaId2StaId.begin(); it != dfaId2StaId.end(); ++it)
//	{
//		for(std::vector<STATEID>::iterator idIter = it->dfaIds.begin(); idIter != it->dfaIds.end(); ++idIter)
//		{
//			TERMSET termset;
//			termset.dfaId = *idIter;
//			termset.dfaSta = it->stateId;
//			lastDfa.PushBackTermSet(termset);
//			lastDfa[it->stateId].SetFlag(lastDfa[it->stateId].GetFlag() | CDfaRow::TERMINAL);
//		}
//	}
//	std::cout << "������̬" << mergtime.Reset() << std::endl;//���ڲ���
//
//	lastDfa.Minimize();
//	std::cout << "lastDfa��С����" << mergtime.Reset() << std::endl;//���ڲ���
//}

