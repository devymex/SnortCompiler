#include "stdafx.h"
#include "OrDfanew.h"
#include "MergeDfanew.h"

//ԭʼ���룬��С���⣬��
//MERDFANEW bool NOrMerge(std::vector<CDfaNew> &dfas, CDfaNew &lastDfa)
//{
//	CTimer c;//���ڲ���
//#undef max
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
//	NFALOG nfalog[CHARSETSIZE * 2];//nfa״̬��dfaID�Ķ�Ӧ�� ���������������⣡����������ϲ�������Ҳ����
//	size_t count = 0;
//	
//	CNfa oneNfa;//�ϲ����nfa
//	//oneNfa.Reserve(nTermSta + 1);
//	oneNfa.Reserve(CHARSETSIZE * 2);
//	oneNfa.Resize(1);
//
//	for(size_t i = 0; i < dfas.size(); ++i)
//	{
//		oneNfa[0].AddDest(EMPTYEDGE, oneNfa.Size() + dfas[i].GetStartId());
//		//NIncreDfaNum(dfas[i], oneNfa.Size());
//		NInsertDfa(dfas[i], oneNfa, nTermSta, nfalog, count);
//	}
//
//	size_t tmp = lastDfa.FromNFA(oneNfa, nfalog, count, true);
//	//if(lastDfa.FromNFA(oneNfa, nfalog, count, true) == 0)
//	if(tmp == 0)
//	{
//		lastDfa.UniqueTermSet();
//		//lastDfa.Minimize();
//		if(lastDfa.Size() > DFA_SIZE_LIMIT)
//		{
//			std::cerr << "DFA_SIZE_LIMIT" << std::endl;
//			return false;
//		}
//		//std::cout << "����һlastDfa��С����ʱ: " << c.Reset() << std::endl;
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

//����ÿ��dfa��m_pGroup���з���
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

//����other��otherSta����termset�����ҵ���TERMSET���뵽��lastDfa��m_TermSet�У�����lastDfa��״̬��lastSta��
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

//�����ȸ���ÿ��dfa�ķ��������ͨ��hash�����յ�lastDfa���з���
MERDFANEW bool NOrMerge(std::vector<CDfaNew> &dfas, CDfaNew &lastDfa)
{
	size_t dfaId = lastDfa.GetId();
	lastDfa.Clear();
	lastDfa.SetId(dfaId);
#undef max

	size_t dfasSize = dfas.size();
	size_t nTermSta = 1;//��̬���
	//for(size_t i = 0; i < dfasSize; ++i)
	//{
	//	if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
	//	{
	//		return false;
	//	}
	//	nTermSta += dfas[i].Size();
	//}
	//++nTermSta;

	//��lastDfa����
	BYTE groups[DFACOLSIZE];
	DfaColGroup(dfas, groups);
	lastDfa.Init(groups);

	size_t colCnt = lastDfa.GetGroupCount();

	typedef std::unordered_map<std::vector<size_t>, STATEID, TODFA_HASH> STATESETHASH;
	STATESETHASH statehash;

	size_t finFlag = 0;//�ж��Ƿ���̬
	std::stack<std::vector<size_t> > statesStack;//����
	std::vector<size_t> startVec(dfasSize + 2);//ʹ��һ����СΪdfas.size() + 2��vector��ʾ�ϲ����nfa��״̬�����е�0��Ԫ�ر�ʾdfa1��״̬��..., �������Ԫ�ر�ʾ����ĳ�ʼ״̬0����ֹ״̬nTermSta
	
	lastDfa.ReservRow(CHARSETSIZE);
	lastDfa.ResizeRow(lastDfa.Size() + 1, colCnt);

	for(size_t i = 0; i < dfasSize; ++i)
	{
		STATEID nSta = dfas[i].GetStartId();
		if((dfas[i][nSta].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			//����̬
			finFlag = 1;
			//��dfas[i]����nSta�ҵ���dfaId���뵽lastDfa��0״̬��Ӧ��TERMSET��
			if(AddTermIntoDFA(nSta, dfas[i], 0, lastDfa) != 0)
			{
				std::cout << "Termset Error!" << std::endl;
				return false;
			}
		}
		startVec[i] = nSta;
	}
	startVec[dfasSize] = 0;//0����ӵ�����״̬
	if(finFlag)
	{
		//˵�����Ǹ���̬
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
		std::vector<size_t> curVec = statesStack.top();//��ǰ״̬����, curVec����dfasSize + 2��״̬
		statesStack.pop();
		
		STATESETHASH::iterator ir = statehash.find(curVec);
		if (ir == statehash.end())
		{
			std::cout << "hash Error!" << std::endl;
			return false;
		}
		STATEID curStaNum = ir->second;
		ZeroMemory(computFlag, sizeof(computFlag));

		//��һ״̬����
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

			size_t flag = 0;//�����һ״̬���Ƿ�Ϊ��
			for(size_t i = 0; i < dfasSize; ++i)
			{
				STATEID sta = curVec[i];
				
				if(sta != (STATEID)-1)
				{
					BYTE curgroup = dfas[i].Char2Group(curChar);
					STATEID nextId = dfas[i][sta][curgroup];//��i��dfa��curVec[i]״̬����curChar��ת����һ״̬
					if(nextId != (STATEID)-1)
					{
						flag = 1;
						if((dfas[i][nextId].GetFlag() & CDfaRow::TERMINAL) != 0)
						{
							//�ж��Ƿ���̬
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
				//��״̬����̬
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

	//��lastDfa���н�һ�����з���
	//lastDfa.Minimize();
	//std::cout << lastDfa.Size() << std::endl;
	//std::cout << lastDfa.Size() << std::endl;
	if(lastDfa.Size() > DFA_SIZE_LIMIT)
	{
		//std::cerr << "DFA_SIZE_LIMIT!" << std::endl;
		return false;
	}

	return true;
}

