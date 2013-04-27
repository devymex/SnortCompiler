#include "stdafx.h"
#include "dfanew.h"
#include "CreDfa.h"

DFANEWSC CDfanew::CDfanew()
	: m_nId(size_t(-1)), m_nColNum(size_t(0)), m_StartId(STATEID(0))
{
	std::fill(m_pGroup, m_pGroup + DFACOLSIZE, BYTE(-1));
	m_pDfa = new std::vector<CDfaRow>;
	m_TermSet = new std::vector<TERMSET>;
}

DFANEWSC CDfanew::~CDfanew()
{
	delete m_pDfa;
	delete m_TermSet;
}

DFANEWSC CDfanew::CDfanew(const CDfanew &other)
{
	m_pDfa = new std::vector<CDfaRow>;
	m_TermSet = new std::vector<TERMSET>;
	*this = other;
}

DFANEWSC CDfanew& CDfanew::operator=(const CDfanew &other)
{
	Clear();
	m_nId = other.m_nId;
	m_nColNum = other.m_nColNum;
	m_StartId = other.m_StartId;
	CopyMemory(m_pGroup, other.m_pGroup, DFACOLSIZE * sizeof(BYTE));
	*m_pDfa = *other.m_pDfa;
	*m_TermSet = *other.m_TermSet;
	return *this;
}

DFANEWSC STATEID CDfanew::Size() const
{
	return (STATEID)m_pDfa->size();
}

DFANEWSC CDfaRow& CDfanew::operator[](STATEID index)
{
	return (*m_pDfa)[index];
}

DFANEWSC const CDfaRow& CDfanew::operator[](STATEID index) const
{
	return (*m_pDfa)[index];
}

DFANEWSC void CDfanew::Init(BYTE *pGroup)
{
	Clear();
	std::vector<BYTE> tmpGroup;
	std::copy(pGroup, pGroup + DFACOLSIZE, std::back_inserter(tmpGroup));
	std::sort(tmpGroup.begin(), tmpGroup.end());
	tmpGroup.erase(std::unique(tmpGroup.begin(), tmpGroup.end()), tmpGroup.end());
	if (tmpGroup.back() == tmpGroup.size() - 1)
	{
		m_nColNum = tmpGroup.size();
		CopyMemory(m_pGroup, pGroup, DFACOLSIZE * sizeof(BYTE));
	}
	else
	{
		std::cerr << "Group error!" << std::endl;
	}
}

DFANEWSC void CDfanew::Clear()
{
	m_nId = size_t(-1);
	m_nColNum = size_t(0);
	m_StartId = STATEID(0);
	std::fill(m_pGroup, m_pGroup + DFACOLSIZE, BYTE(-1));
	delete m_pDfa;
	delete m_TermSet;
	m_pDfa = new std::vector<CDfaRow>;
	m_TermSet = new std::vector<TERMSET>;
}

DFANEWSC size_t CDfanew::FromNFA(CNfa &nfa, NFALOG *nfalog, size_t Count, bool combine)
{
	BYTE groups[DFACOLSIZE];
	AvaiEdges(nfa, groups);
	Init(groups);

	std::vector<std::pair<std::vector<size_t>, STATEID>> termStasVec;

	typedef std::unordered_map<std::vector<size_t>, STATEID, STATESET_HASH> STATESETHASH;

	STATESETHASH ssh;
	ssh.rehash(STATESET_HASH::MAX_SIZE);

	std::stack<std::vector<size_t>> nfaStasStack;
	std::vector<size_t> startEVec;
	std::vector<size_t> startVec;

	char finFlag = 0;
	startVec.push_back(0);
	EClosure(nfa, startVec, startEVec, finFlag);


	nfaStasStack.push(startEVec);
	//ssh.insert(std::make_pair(startEVec, ssh.size()));
	ssh[startEVec] = 0;

	STATEID nCursize = this->Size();
	m_pDfa->resize(nCursize + 1, m_nColNum);

	m_pDfa->back().SetFlag(m_pDfa->back().GetFlag() | m_pDfa->back().START);

	if(finFlag == 1)
	{
		m_pDfa->back().SetFlag(m_pDfa->back().GetFlag() | m_pDfa->back().TERMINAL);
		finFlag = 0;
	}
	std::vector<size_t> curNfaVec;
	std::vector<size_t> compuFlag;
	while(nfaStasStack.size() > 0)
	{
		compuFlag.clear();
		STATEID curStaNum;
		curNfaVec = nfaStasStack.top();
		nfaStasStack.pop();

		for(size_t nCurChar = 0; nCurChar < DFACOLSIZE; ++nCurChar)
		{
			if( m_pDfa->size() > SC_STATELIMIT)
			{
				return (size_t)-1;
			}

			STATEID curGroup = m_pGroup[nCurChar];
			if(std::find(compuFlag.begin(), compuFlag.end(), curGroup) != compuFlag.end())
			{
				continue;
			}

			compuFlag.push_back(curGroup);

			STATESETHASH::iterator ir = ssh.find(curNfaVec);
			if (ir == ssh.end())
			{
				std::cout << "Fatal Error!" << std::endl;
				break;
			}
			curStaNum = ir->second;

			std::vector<size_t> nextNfaVec;

			NextNfaSet(nfa, curNfaVec, nCurChar, nextNfaVec, finFlag);

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
					STATEID nextSta = (STATEID)ssh.size();
					ssh[nextNfaVec] = nextSta;

					STATEID nCursize = (STATEID)m_pDfa->size();
					m_pDfa->resize(nCursize + 1, m_nColNum);

					(*m_pDfa)[curStaNum][curGroup] = nextSta;

					if(finFlag == 1)
					{
						m_pDfa->back().SetFlag(m_pDfa->back().GetFlag() | m_pDfa->back().TERMINAL);
						finFlag = 0;
						if(combine)
						{
							termStasVec.push_back(std::make_pair(nextNfaVec, nextSta));
						}
						else
						{
							TERMSET term;
							term.dfaSta = nextSta;
							term.dfaId = m_nId;
							m_TermSet->push_back(term);
						}

					}
					nfaStasStack.push(nextNfaVec);
				}
				else
				{
					(*m_pDfa)[curStaNum][curGroup] = ssh[nextNfaVec];
				}
			}
		}
	}

	if(combine)
	{
		if(!termStasVec.empty())
		{
			for(size_t i = 0; i < Count; ++i)
			{
				for(size_t j = 0; j < termStasVec.size(); ++j)
				{
					for(size_t k = 0; k < termStasVec[j].first.size(); ++k)
					{
						if(nfalog[i].nfaStateId == termStasVec[j].first[k])
						{
							TERMSET term;
							term.dfaSta = termStasVec[j].second;
							term.dfaId = nfalog[i].dfaId;
							m_TermSet->push_back(term);;
							break;
						}
					}
				}
			}
		}
	}
	return 0;
}

DFANEWSC size_t CDfanew::Minimize()
{
	//error: DFA is empty 
	if (m_pDfa->size() == 0)
	{
		return size_t(-1);
	}

	// FinalStas�б��浱ǰDFA��������̬��Partition�б��浱ǰDFA����̬�ͷ���̬���ϣ�һ����̬��Ϊһ�����ϴ���
	std::list<std::list<STATEID>> Partition(1);
	std::list<STATEID> FinalStas;
	for (STATEID i = 0; i < m_pDfa->size(); ++i)
	{
		if (((*m_pDfa)[i].GetFlag() & (*m_pDfa)[i].TERMINAL) != 0)
		{
			FinalStas.push_back(i);
			Partition.push_front(std::list<STATEID>());
			Partition.front().push_back(i);
		}
		else
		{
			Partition.back().push_back(i);
		}
	}

	//error: terminal states or normal states are empty
	if (Partition.size() == 1 || Partition.back().empty())
	{
		return size_t(-2);
	}

	//����������ʱ�ָ����ʼ״̬��ʼ����ɨ��DFA����¼��ǰ״̬ͨ��ĳһ�ַ��ɵ����״̬����
	size_t row = m_pDfa->size();
	size_t col = GetGroupCount();
	std::vector<STATEID> *pPosTab = new std::vector<STATEID>[row * col];
	for (STATEID i = 0; i < row; ++i)
	{
		for (STATEID j = 0; j < col; ++j)
		{
			STATEID nDest = (STATEID)(*m_pDfa)[i][j];
			if (nDest != STATEID(-1))
			{
				pPosTab[i * col + j].push_back(nDest);
			}
		}
	}

	//��ʼ���ɴ�״̬��reachable
	std::vector<BYTE> reachable(m_pDfa->size(), 0);
	std::list<STATEID> StartStas;
	StartStas.push_back(0);

	//record states that will be visited from positive traverse table
	RemoveUnreachable(pPosTab, StartStas, col, reachable);
	delete []pPosTab;

	//����������ʱ�����̬��ʼ����ɨ��DFA����¼��Щ״̬����ͨ��ĳһ�ַ����ﵱǰ״̬
	std::vector<STATEID> *pRevTab = new std::vector<STATEID>[row * col];
	for (STATEID i = 0; i < row; ++i)
	{
		for (STATEID j = 0; j < col; ++j)
		{
			STATEID nDest = (STATEID)(*m_pDfa)[i][j];
			if (nDest != STATEID(-1))
			{
				pRevTab[nDest * col + j].push_back(STATEID(i));
			}
		}
	}

	//record states that will be visited from reverse traverse table
	RemoveUnreachable(pRevTab, FinalStas, col, reachable);
	delete []pRevTab;

	//remove unreachable states, generate new DFA
	MergeReachable(reachable);

	row = m_pDfa->size();
	pRevTab = new std::vector<STATEID>[row * col];
	for (STATEID i = 0; i < row; ++i)
	{
		for (STATEID j = 0; j < col; ++j)
		{
			STATEID nDest = (STATEID)(*m_pDfa)[i][j];
			if (nDest != STATEID(-1))
			{
				pRevTab[nDest * col + j].push_back(STATEID(i));
			}
		}
	}

	//divide nondistinguishable states
	PartitionNonDisState(pRevTab, Partition);

	//DFA minization
	MergeNonDisStates(Partition);

	delete []pRevTab;
	return 0;
}

DFANEWSC size_t CDfanew::GetGroupCount() const
{
	return m_nColNum;
}

DFANEWSC BYTE CDfanew::Char2Group(BYTE nIdx)
{
	return m_pGroup[nIdx];
}

DFANEWSC const BYTE* CDfanew::GetGroup() const
{
	return m_pGroup;
}

DFANEWSC STATEID CDfanew::GetStartId() const
{
	return m_StartId;
}

DFANEWSC void CDfanew::SetId(size_t id)
{
	m_nId = id;
}

DFANEWSC size_t CDfanew::GetId()
{
	return m_nId;
}

DFANEWSC size_t CDfanew::Process(BYTE *ByteStream, size_t len, CStateSet &StaSet)
{
	std::vector<bool> res(m_pDfa->size(), false);
	STATEID ActiveState = m_StartId;
	size_t nPos = 0;
	for (size_t nPos = 0; nPos < len; ++nPos)
	{
		if ((*this)[ActiveState].GetFlag() & CDfaRow::TERMINAL)
		{
			res[ActiveState] = true;
		}
		ActiveState = (*this)[ActiveState][m_pGroup[*(ByteStream + nPos)]];
		if (ActiveState == STATEID(-1))
		{
			break;
		}
	}
	for (size_t i = 0; i < res.size(); ++i)
	{
		if (res[i])
		{
			StaSet.PushBack(i);
		}
	}

	return nPos;
}

struct COMPFORSORT
{
	bool operator()(const TERMSET &t1, const TERMSET &t2)
	{
		if (t1.dfaSta < t2.dfaSta || (t1.dfaSta == t2.dfaSta && t1.dfaId < t2.dfaId))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

struct COMP
{
	bool operator()(const TERMSET &t1, const TERMSET &t2)
	{
		if (t1.dfaSta < t2.dfaSta)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

DFANEWSC void CDfanew::GetAcceptedId(STATEID id, CVectorNumber &dfaIds)
{
	std::sort(m_TermSet->begin(), m_TermSet->end(), COMPFORSORT());
	std::vector<TERMSET>::iterator Beg = std::lower_bound(m_TermSet->begin(), m_TermSet->end(), TERMSET(id, 0), COMP());
	std::vector<TERMSET>::iterator End = std::upper_bound(m_TermSet->begin(), m_TermSet->end(), TERMSET(id, 0), COMP());
	for (std::vector<TERMSET>::iterator i = Beg; i != End; ++i)
	{
		dfaIds.PushBack(i->dfaId);
	}
	dfaIds.Unique();
}

template<typename _Ty>
void WriteNum(BYTE*& pBuf, _Ty _num, size_t nBytes = sizeof(_Ty))
{
	CopyMemory(pBuf, &_num, nBytes);
	pBuf += nBytes;
}

DFANEWSC size_t CDfanew::Save(BYTE *beg)
{
	BYTE *pOld = beg;
	//дDFA��Id
	WriteNum(beg, m_nId);
	//дDFA��״̬����
	WriteNum(beg, m_pDfa->size(), sizeof(STATEID));
	if (m_pDfa->size() == 0)
	{
		return beg - pOld;
	}
	//д����
	for (size_t i = 0; i < DFACOLSIZE; ++i)
	{
		WriteNum(beg, m_pGroup[i]);
	}
	//дDFA��ת��
	for (size_t i = 0; i < m_pDfa->size(); ++i)
	{
		//д��״̬��Flag(NORMAL��START��TERMINAL)
		WriteNum(beg, (*m_pDfa)[i].GetFlag());
		for (size_t j = 0; j < m_nColNum; ++j)
		{
			WriteNum(beg, (*m_pDfa)[i][j]);
		}
	}
	//дDFA�Ŀ�ʼ״̬���
	WriteNum(beg, m_StartId);
	//дDFA����̬��DFAId�Ķ�Ӧ��ϵ
	WriteNum(beg, m_TermSet->size());
	for (size_t i = 0; i < m_TermSet->size(); ++i)
	{
		WriteNum(beg, (*m_TermSet)[i].dfaSta);
		WriteNum(beg, (*m_TermSet)[i].dfaId);
	}

	return beg - pOld;
}

template<typename _Ty>
void ReadNum(BYTE*& pBuf, _Ty &_num, size_t nBytes = sizeof(_Ty))
{
	CopyMemory(&_num, pBuf, nBytes);
	pBuf += nBytes;
}

DFANEWSC void CDfanew::Load(BYTE *beg, size_t len)
{
	//��DFA��Id
	size_t dfaId;
	ReadNum(beg, dfaId);
	m_nId = dfaId;
	//��DFA��״̬����
	STATEID dfaSize;//DFA��״̬��
	ReadNum(beg, dfaSize);
	if (dfaSize == 0)
	{
		return;
	}
	//������
	BYTE pGroup[DFACOLSIZE];
	for (size_t i = 0; i < DFACOLSIZE; ++i)
	{
		ReadNum(beg, pGroup[i]);
	}
	Init(pGroup);
	m_nId = dfaId;
	//��DFA��ת��
	m_pDfa->resize(dfaSize, m_nColNum);
	size_t nFlag;
	for (size_t i = 0; i < m_pDfa->size(); ++i)
	{
		//����״̬��Flag(NORMAL��START��TERMINAL)
		ReadNum(beg, nFlag);
		(*m_pDfa)[i].SetFlag(nFlag);
		for (size_t j = 0; j < m_nColNum; ++j)
		{
			ReadNum(beg, (*m_pDfa)[i][j]);
		}
	}
	//��DFA�Ŀ�ʼ״̬���
	ReadNum(beg, m_StartId);
	//��DFA����̬��DFAId�Ķ�Ӧ��ϵ
	size_t TermSetSize;
	ReadNum(beg, TermSetSize);
	m_TermSet->resize(TermSetSize);
	for (size_t i = 0; i < TermSetSize; ++i)
	{
		ReadNum(beg, (*m_TermSet)[i].dfaSta);
		ReadNum(beg, (*m_TermSet)[i].dfaId);
	}
}

//ɾ�����ɴ�״̬���ߡ�����״̬��Tab��ʾ����һ��DFA��������ʱ��һ��DFA��������ʱ�
//begs��ʾ��Tab����ʼλ��
//reachable��ʾ�ɴ�״̬���ϣ���ʼֵΪ0��������
void CDfanew::RemoveUnreachable(const std::vector<STATEID> *Tab, const STALIST &begs, const size_t &col, std::vector<BYTE> &reachable)
{
	size_t stas = reachable.size();
	//mark state after traversal, 0 is unreachable and 1 is reachable
	std::vector<BYTE> staFlags(stas, 0);

	//reserve current visited states
	std::vector<size_t> staStack;
	staStack.reserve(stas);

	staStack.assign(begs.begin(), begs.end());
	//extract each state in staStack until all states have visited
	for (; !staStack.empty(); )
	{
		size_t nSta = staStack.back();
		staStack.pop_back();
		if (staFlags[nSta] == 0)
		{
			staFlags[nSta] = 1;
			//according to each of transitions add states to staStack 
			for (size_t i = 0; i < col; ++i)
			{
				for (size_t j = 0; j < Tab[nSta * col + i].size(); ++j)
				{
					staStack.push_back(Tab[nSta * col + i][j]);
				}
			}
		}
	}

	for (std::vector<BYTE>::iterator i = staFlags.begin(); i != staFlags.end(); ++i)
	{
		reachable[i - staFlags.begin()] += *i;
	}
}

//reachable�б������еĿɴ�״̬��reachable��Ԫ�ص�ȡֵ��3����
//0 ��ʾ��״̬Ϊ����״̬��1 ��ʾ��״̬Ϊ���ɴ�״̬���ߡ�����״̬��2 ��ʾ��״̬Ϊ�ɴ�״̬
void CDfanew::MergeReachable(std::vector<BYTE> &reachable)
{
	//ͳ�ƿɴ�״̬����Ŀ
	size_t nRcbCnt = std::count(reachable.begin(), reachable.end(), 2);

	size_t nColNum = GetGroupCount();

	//�����̬��dfaId���Ա�֤��̬��Ÿ��ĺ����Ӧ��dfaId���ֲ���
	std::vector<size_t> termFlag(m_pDfa->size(), size_t(-1));
	for (size_t i = 0; i < m_TermSet->size(); ++i)
	{
		termFlag[(*m_TermSet)[i].dfaSta] = (*m_TermSet)[i].dfaId;
	}
	delete m_TermSet;
	m_TermSet = new std::vector<TERMSET>;
	
	//����һ��ͬ��Ա����m_pDfa������ͬ�ı��������ڴ洢ɾ������״̬���DFA��ת��
	std::vector<CDfaRow> tmpDfa;
	tmpDfa.resize(nRcbCnt, nColNum);

	STATEID nNewIdx = 0;
	//��m_pDfa�б�Ŷ�ӦΪ�ɴ�״̬�ĸ��Ƶ�tmpDfa�У����޸�reachable�пɴ�״̬�ı��
	for (std::vector<BYTE>::iterator iter = reachable.begin(); iter != reachable.end(); ++iter)
	{
		if (2 == *iter)
		{
			STATEID nStaId = STATEID(iter - reachable.begin());
			tmpDfa[nNewIdx] = (*m_pDfa)[nStaId];
			*iter = nNewIdx;

			//�����µ���̬���
			if (((*m_pDfa)[nStaId].GetFlag() & (*m_pDfa)[nStaId].TERMINAL) != 0)
			{
				TERMSET tmpSta;
				tmpSta.dfaSta = nNewIdx;
				tmpSta.dfaId = termFlag[nStaId];
				m_TermSet->push_back(tmpSta);
			}
			++nNewIdx;
		}
		else
		{
			*iter = STATEID(-1);
		}
	}

	//��tmpDfa�е�״̬����޸�Ϊ��Ӧ���±��
	for (STATEID i = 0; i < nRcbCnt; ++i)
	{
		CDfaRow &curRow = tmpDfa[i];
		for (STATEID j = 0; j < nColNum; ++j)
		{
			if (curRow[j] != STATEID(-1))
			{
				curRow[j] = reachable[curRow[j]];
			}
		}
	}

	//�滻m_pDfa
	m_pDfa->clear();
	m_pDfa->resize(nRcbCnt, nColNum);
	nNewIdx = 0;
	for (STATEID idx = 0; idx < nRcbCnt; ++idx)
	{
		(*m_pDfa)[nNewIdx] = tmpDfa[idx];
		++nNewIdx;
	}
}

//groupnum��ʾ�ַ������ȣ�size��ʾԭDFA��״̬����pRevTbl��ʾ������ʱ�
//pSets��ʾ����һ��״̬���ĳ�ʼ���֣����һ��״̬�������ջ��ֽ��
//pSets��ʼֵΪ��̬�ͷ���̬���ϣ��������һ��Ϊ����̬����
void CDfanew::PartitionNonDisState(std::vector<STATEID> *pRevTbl, SETLIST &pSets)
{
	//����Ҫ���ҵĻ��ֵ�iterator����wSets����ʼ��ʱֻ������̬����
	std::list<SETLIST_ITER> wSets;
	SETLIST_ITER iLast = pSets.end();
	--iLast;

	//initialize wSets
	for (SETLIST_ITER iCurSet = pSets.begin(); iCurSet != iLast; ++iCurSet)
	{
		wSets.push_back(iCurSet);
	}

	//each element in ableToW present a property of according state of tmpDfa,
	//and has two labels, 0 and 1. 1 indicates the according state has the specific
	//transition to one state of curWSet, 0 otherwise
	std::vector<BYTE> ableToW(m_pDfa->size(),BYTE(0));
	bool bAllZero = true;
	BYTE groupnum = (BYTE)GetGroupCount();
	for (; !wSets.empty(); )
	{
		STALIST curWSet = *wSets.front();
		wSets.pop_front();
		for (BYTE byChar = 0; byChar < groupnum; ++byChar)
		{
			//initialize the ableToW
			//for each state in curWSet find source states of it in pRecTbl
			//if exist at last one source state to curWSet, the bAllZero is set to false
			for (STALIST_ITER iSta = curWSet.begin(); iSta != curWSet.end(); ++iSta)
			{
				std::vector<STATEID> &ableToI = pRevTbl[*iSta * groupnum + byChar];
				for (std::vector<STATEID>::iterator i = ableToI.begin(); i != ableToI.end(); ++i)
				{
					ableToW[*i] = 1;
					bAllZero = false;
				}
			}
			if (!bAllZero)
			{
				for (SETLIST_ITER iPSet = pSets.begin(); iPSet != pSets.end(); ++iPSet)
				{
					//each partition in pSets,according to the label of a state in partition adjust position 
					//all unvisited states lie in the front of list, visited states locate
					//at the rear end of list 
					for (STALIST_ITER iCurSta = iPSet->begin(); iCurSta != iPSet->end(); )
					{
						if (ableToW[*iCurSta] == 0)
						{
							STATEID tmp = *iCurSta;
							iCurSta = iPSet->erase(iCurSta);
							iPSet->insert(iPSet->begin(), tmp);
						}
						else
						{
							++iCurSta;
						}
					} 

					//mark the position of two new partition
					size_t nUnableCnt = 0;
					STALIST_ITER iCutBeg = iPSet->begin(), iCutEnd = iPSet->end();
					for (; iCutBeg != iPSet->end(); ++iCutBeg)
					{
						if (ableToW[*iCutBeg] == 1)
						{
							break;
						}
						++nUnableCnt;
					}

					//record start position of the less partition 
					if (nUnableCnt <= iPSet->size() / 2)
					{
						iCutEnd = iCutBeg;
						iCutBeg = iPSet->begin();
					}

					//the less partition insert into pSets, its iterator insert into wSets
					if (iCutBeg != iCutEnd)
					{
						SETLIST_ITER iOldSet = iPSet;
						iPSet = pSets.insert(++iPSet, STALIST());
						iPSet->splice(iPSet->begin(), *iOldSet, iCutBeg, iCutEnd);
						wSets.push_back(iPSet);
					}
				}

				//initialize ableToW and bAllZero before read the next char, 
				ZeroMemory(&ableToW[0], ableToW.size());
				bAllZero = true;
			}
		}
	}
}

//Partition�е�Ԫ��Ϊһ��״̬�ļ��ϣ�������Ԫ��Ϊ����ȼ�״̬��ÿ�����Ͽ��Ժϲ�Ϊ�µ�DFA��һ��״̬
void CDfanew::MergeNonDisStates(SETLIST &Partition)
{
	std::vector<STATEID> sta2Part(m_pDfa->size());
	
	STATEID nCol = (STATEID)GetGroupCount();

	//�����̬��dfaId���Ա�֤��̬��Ÿ��ĺ����Ӧ��dfaId���ֲ���
	std::vector<size_t> termFlag(m_pDfa->size(), size_t(-1));
	for (size_t i = 0; i < m_TermSet->size(); ++i)
	{
		termFlag[(*m_TermSet)[i].dfaSta] = (*m_TermSet)[i].dfaId;
	}
	delete m_TermSet;
	m_TermSet = new std::vector<TERMSET>;

	//����һ��ͬCDfanew�г�Ա����m_pDfa������ͬ�ı��������ڴ洢ɾ������״̬���DFA��ת��
	std::vector<CDfaRow> tmpDfa;
	tmpDfa.resize((STATEID)Partition.size(), nCol);

	//�ȼ۵�״̬����ͬһ��partition�У����ԭ����״̬������һ���µ�partition�У����޸��µ���ʼ״̬���
	STATEID nSetIdx = 0;
	for (SETLIST_ITER iSet = Partition.begin(); iSet != Partition.end(); ++iSet)
	{
		for (STALIST_ITER iSta = iSet->begin(); iSta != iSet->end(); ++iSta)
		{
			sta2Part[*iSta] = nSetIdx;
			//�޸��µ���ʼ״̬
			if (((*m_pDfa)[*iSta].GetFlag() & (*m_pDfa)[*iSta].START) != 0)
			{
				m_StartId = nSetIdx;
			}

			//�����µ���̬���
			if (((*m_pDfa)[*iSta].GetFlag() & (*m_pDfa)[*iSta].TERMINAL) != 0)
			{
				TERMSET tmpSta;
				tmpSta.dfaSta = nSetIdx;
				tmpSta.dfaId = termFlag[*iSta];
				m_TermSet->push_back(tmpSta);
			}

		}
		++nSetIdx;
	}

	//set new DFA and modify new number
	nSetIdx = 0;
	for (SETLIST_ITER iSet = Partition.begin(); iSet != Partition.end(); ++iSet)
	{
		for (BYTE iCol = 0; iCol != nCol; ++iCol)
		{
			CDfaRow &orgRow = (*m_pDfa)[iSet->front()];
			STATEID nDest = STATEID(-1);
			if (orgRow[iCol] != STATEID(-1))
			{
				nDest = sta2Part[orgRow[iCol]];
			}
			tmpDfa[nSetIdx][iCol] = nDest;
		}

		//set a state attribute
		tmpDfa[nSetIdx].SetFlag((*m_pDfa)[iSet->front()].GetFlag());
		++nSetIdx;
	}

	//�滻m_pDfa
	m_pDfa->clear();
	m_pDfa->resize(tmpDfa.size(), nCol);
	for (STATEID idx = 0; idx < tmpDfa.size(); ++idx)
	{
		(*m_pDfa)[idx] = tmpDfa[idx];
	}
}

