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

	// FinalStas中保存当前DFA的所有终态，Partition中保存当前DFA的终态和非终态集合，一个终态作为一个集合存入
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

	//构建正向访问表，指从起始状态开始正向扫描DFA，记录当前状态通过某一字符可到达的状态集合
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

	//初始化可达状态表reachable
	std::vector<BYTE> reachable(m_pDfa->size(), 0);
	std::list<STATEID> StartStas;
	StartStas.push_back(0);

	//record states that will be visited from positive traverse table
	RemoveUnreachable(pPosTab, StartStas, col, reachable);
	delete []pPosTab;

	//构建逆向访问表，从终态开始逆向扫描DFA，记录哪些状态可以通过某一字符到达当前状态
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
	PartitionNonDisState(GetGroupCount(), m_pDfa->size(), pRevTab, Partition);

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

//reachable中保留所有的可达状态，reachable中元素的取值有3个，
//0 表示该状态为孤立状态，1 表示该状态为不可达状态或者“死”状态，2 表示该状态为可达状态
void CDfanew::MergeReachable(std::vector<BYTE> &reachable)
{
	//统计可达状态的数目
	size_t nRcbCnt = std::count(reachable.begin(), reachable.end(), 2);

	size_t nColNum = GetGroupCount();

	//标记终态的dfaId，以保证终态编号更改后，其对应的dfaId保持不变
	std::vector<size_t> termFlag(m_pDfa->size(), size_t(-1));
	for (size_t i = 0; i < m_TermSet->size(); ++i)
	{
		termFlag[(*m_TermSet)[i].dfaSta] = (*m_TermSet)[i].dfaId;
	}
	delete m_TermSet;
	m_TermSet = new std::vector<TERMSET>;
	
	//定义一个同成员变量m_pDfa类型相同的变量，用于存储删除多余状态后的DFA跳转表
	std::vector<CDfaRow> tmpDfa;
	tmpDfa.resize(nRcbCnt, nColNum);

	STATEID nNewIdx = 0;
	//将m_pDfa中编号对应为可达状态的复制到tmpDfa中，并修改reachable中可达状态的编号
	for (std::vector<BYTE>::iterator iter = reachable.begin(); iter != reachable.end(); ++iter)
	{
		if (2 == *iter)
		{
			STATEID nStaId = STATEID(iter - reachable.begin());
			tmpDfa[nNewIdx] = (*m_pDfa)[nStaId];
			*iter = nNewIdx;

			//存入新的终态编号
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

	//将tmpDfa中的状态编号修改为相应的新编号
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

	//替换m_pDfa
	m_pDfa->clear();
	m_pDfa->resize(nRcbCnt, nColNum);
	nNewIdx = 0;
	for (STATEID idx = 0; idx < nRcbCnt; ++idx)
	{
		(*m_pDfa)[nNewIdx] = tmpDfa[idx];
		++nNewIdx;
	}
}

//Partition中的元素为一个状态的集合，集合中元素为多个等价状态，每个集合可以合并为新的DFA中一个状态
void CDfanew::MergeNonDisStates(SETLIST &Partition)
{
	std::vector<STATEID> sta2Part(m_pDfa->size());
	
	STATEID nCol = (STATEID)GetGroupCount();

	//标记终态的dfaId，以保证终态编号更改后，其对应的dfaId保持不变
	std::vector<size_t> termFlag(m_pDfa->size(), size_t(-1));
	for (size_t i = 0; i < m_TermSet->size(); ++i)
	{
		termFlag[(*m_TermSet)[i].dfaSta] = (*m_TermSet)[i].dfaId;
	}
	delete m_TermSet;
	m_TermSet = new std::vector<TERMSET>;

	//定义一个同CDfanew中成员变量m_pDfa类型相同的变量，用于存储删除多余状态后的DFA跳转表
	std::vector<CDfaRow> tmpDfa;
	tmpDfa.resize((STATEID)Partition.size(), nCol);

	//等价的状态存于同一个partition中，标记原来的状态存在哪一个新的partition中，并修改新的起始状态编号
	STATEID nSetIdx = 0;
	for (SETLIST_ITER iSet = Partition.begin(); iSet != Partition.end(); ++iSet)
	{
		for (STALIST_ITER iSta = iSet->begin(); iSta != iSet->end(); ++iSta)
		{
			sta2Part[*iSta] = nSetIdx;
			//修改新的起始状态
			if (((*m_pDfa)[*iSta].GetFlag() & (*m_pDfa)[*iSta].START) != 0)
			{
				m_StartId = nSetIdx;
			}

			//存入新的终态编号
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

	//替换m_pDfa
	m_pDfa->clear();
	m_pDfa->resize(tmpDfa.size(), nCol);
	nSetIdx = 0;
	for (STATEID idx = 0; idx < nCol; ++idx)
	{
		(*m_pDfa)[nSetIdx] = tmpDfa[idx];
		++nSetIdx;
	}
}

