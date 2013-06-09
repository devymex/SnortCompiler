#include "stdafx.h"
#include "dfanew.h"
#include "dfaalgo.h"

DFANEWSC CFinalStates::CFinalStates()
{
	m_pStates = new std::vector<STATEID>;
	m_pDfaIds = new std::unordered_map<STATEID, std::set<size_t>>;
}

DFANEWSC CFinalStates::~CFinalStates()
{
	delete m_pStates;
	delete m_pDfaIds;
}

DFANEWSC CFinalStates::CFinalStates(const CFinalStates &other)
{
	m_pStates = new std::vector<STATEID>;
	m_pDfaIds = new std::unordered_map<STATEID, std::set<size_t>>;
	*this = other;
}

DFANEWSC CFinalStates& CFinalStates::operator=(const CFinalStates &other)
{
	*m_pStates = *other.m_pStates;
	*m_pDfaIds = *other.m_pDfaIds;
	return *this;
}

DFANEWSC STATEID CFinalStates::operator[](size_t nIdx) const
{
	return (*m_pStates)[nIdx];
}

DFANEWSC size_t CFinalStates::Size() const
{
	return m_pStates->size();
}

DFANEWSC void CFinalStates::Clear()
{
	m_pStates->clear();
}

DFANEWSC void CFinalStates::PushBack(STATEID nStaId, size_t nDfaId)
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		(*m_pDfaIds)[nStaId].insert(nDfaId);
		(*m_pStates).push_back(nStaId);
	}
	else
	{
		iter->second.insert(nDfaId);
	}
}

DFANEWSC void CFinalStates::GetDfaIds(STATEID nStaId, CVectorUnsigned &ids) const
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		throw 0;
	}
	const std::set<size_t> &curSet = iter->second;
	for (std::set<size_t>::const_iterator i = curSet.begin();
		i != curSet.end(); ++i)
	{
		ids.PushBack(*i);
	}
}

DFANEWSC std::set<size_t>& CFinalStates::_GetDfaIds(STATEID nStaId)
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		throw 0;
	}
	return iter->second;
}

DFANEWSC const std::set<size_t>& CFinalStates::_GetDfaIds(STATEID nStaId) const
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		throw 0;
	}
	return iter->second;
}

DFANEWSC size_t CFinalStates::GetDfaIdCount(STATEID nStaId) const
{
	return (*m_pDfaIds)[nStaId].size();
}

DFANEWSC size_t CFinalStates::GetAllDfaIdCount() const
{
	size_t nSum = 0;
	for (FINSTAMAP_CITER i = m_pDfaIds->cbegin(); i != m_pDfaIds->end(); ++i)
	{
		nSum += i->second.size();
	}
	return nSum;
}

DFANEWSC CDfaRow::CDfaRow(size_t col)
	: m_nFlag(NORMAL), m_nColNum(col)
{
	m_pDest = new STATEVEC;
	m_pDest->resize(m_nColNum);
	Fill(STATEID(-1));
}

DFANEWSC CDfaRow::~CDfaRow()
{
	delete m_pDest;
}
DFANEWSC CDfaRow::CDfaRow(const CDfaRow &other)
{
	m_pDest = new STATEVEC;
	*this = other;
}

DFANEWSC CDfaRow& CDfaRow::operator=(const CDfaRow &other)
{
	m_nFlag = other.m_nFlag;
	m_nColNum = other.m_nColNum;
	*m_pDest = *other.m_pDest;
	return *this;
}

DFANEWSC STATEID& CDfaRow::operator[](BYTE nIdx)
{
	return (*m_pDest)[nIdx];
}

DFANEWSC const STATEID& CDfaRow::operator[](BYTE nIdx) const
{
	return (*m_pDest)[nIdx];
}

DFANEWSC void CDfaRow::Fill(STATEID _Val)
{
	std::fill(m_pDest->begin(), m_pDest->end(), _Val);
}

DFANEWSC void CDfaRow::SetFlag(size_t nFlag)
{
	m_nFlag = nFlag;
}

DFANEWSC size_t CDfaRow::GetFlag() const
{
	return m_nFlag;
}

DFANEWSC size_t CDfaRow::GetColNum() const
{
	return m_nColNum;
}

DFANEWSC CDfaNew::CDfaNew()
	: m_nId(size_t(-1)), m_nColNum(size_t(0)), m_nStartId(STATEID(0))
{
	std::fill(m_pGroup, m_pGroup + DFACOLSIZE, BYTE(-1));
	m_pDfa = new std::vector<CDfaRow>;
}

DFANEWSC CDfaNew::~CDfaNew()
{
	delete m_pDfa;
}

DFANEWSC CDfaNew::CDfaNew(const CDfaNew &other)
{
	m_pDfa = new std::vector<CDfaRow>;
	*this = other;
}

DFANEWSC CDfaNew& CDfaNew::operator=(const CDfaNew &other)
{
	Clear();
	m_nId = other.m_nId;
	m_nColNum = other.m_nColNum;
	m_nStartId = other.m_nStartId;
	CopyMemory(m_pGroup, other.m_pGroup, DFACOLSIZE * sizeof(BYTE));
	*m_pDfa = *other.m_pDfa;
	m_FinStas = other.m_FinStas;
	return *this;
}

DFANEWSC size_t CDfaNew::Size() const
{
	return m_pDfa->size();
}

DFANEWSC CDfaRow& CDfaNew::Back()
{
	return m_pDfa->back();
}

DFANEWSC void CDfaNew::Reserve(size_t nSize)
{
	m_pDfa->reserve(nSize);
}

DFANEWSC void CDfaNew::Resize(size_t nSize, size_t nCol)
{
	m_pDfa->resize(nSize, CDfaRow(nCol));
}

DFANEWSC CDfaRow& CDfaNew::operator[](STATEID index)
{
	return (*m_pDfa)[index];
}

DFANEWSC const CDfaRow& CDfaNew::operator[](STATEID index) const
{
	return (*m_pDfa)[index];
}

DFANEWSC void CDfaNew::PushBack(CDfaRow &sta)
{
	m_pDfa->push_back(sta);
}

DFANEWSC void CDfaNew::Init(BYTE *pGroup)
{
	//1 if the group is used, 0 otherwise
	BYTE occurred[DFACOLSIZE] = {0};
	for (size_t i = 0; i < DFACOLSIZE; ++i) 
	{
		occurred[pGroup[i]] = 1;
	}

	//number of groups
	WORD nZeroBegPos = 0;
	for (; nZeroBegPos < DFACOLSIZE; ++nZeroBegPos)
	{
		if (occurred[nZeroBegPos] == 0)
		{
			break;
		}
	}

	//flag is true if 1 does not occur after nZeroBegPos, false otherwise
	//flag is true indicates that the group is reasonable
	bool flag = true;
	for (size_t i = nZeroBegPos + 1; i < DFACOLSIZE; ++i)
	{
		if (occurred[i] == 1)
		{
			flag = false;
			break;
		}
	}

	//the group is reasonable, and nZeroBegPos is the number of groups
	if (flag)
	{
		m_nColNum = nZeroBegPos;
		CopyMemory(m_pGroup, pGroup, DFACOLSIZE * sizeof(BYTE));
	}
	else
	{
		std::cerr << "Group error!" << std::endl;
	}
}

DFANEWSC void CDfaNew::Clear()
{
	m_nId = size_t(-1);
	m_nColNum = size_t(0);
	m_nStartId = STATEID(0);
	memset(m_pGroup, 0xFF, DFACOLSIZE);
	m_pDfa->clear();
	m_FinStas.Clear();
}

DFANEWSC size_t CDfaNew::FromNFA(const CNfa &nfa)
{
	typedef std::unordered_map<std::vector<size_t>, STATEID, NSTATESET_HASH> STATESETHASH;
	//std::vector<std::pair<std::vector<size_t>, STATEID>> termStasVec;

	size_t dfaId = m_nId;
	Clear();
	m_nId = dfaId;
	
	std::vector<std::vector<size_t>> eClosure;
	NfaEClosure(nfa, eClosure);

	BYTE groups[DFACOLSIZE];
	NAvaiEdges(nfa, groups);
	Init(groups);

	std::stack<std::vector<size_t>> nfaStasStack;
	std::vector<size_t> &startEVec = eClosure.front();
	nfaStasStack.push(startEVec);

	STATESETHASH ssh;
	ssh[startEVec] = 0;

	m_pDfa->reserve(300);
	m_pDfa->push_back(CDfaRow(m_nColNum));
	CDfaRow &firstRow = m_pDfa->back();
	firstRow.SetFlag(firstRow.GetFlag() | firstRow.START);

	if (startEVec.back() == nfa.Size())
	{
		firstRow.SetFlag(firstRow.GetFlag() | firstRow.TERMINAL);
		m_FinStas.PushBack(0, m_nId);
	}

	std::vector<size_t> curNfaVec;
	static std::vector<size_t> nextNfaVec; //
	nextNfaVec.clear();
	BYTE compuFlag[CHARSETSIZE];

	for (; nfaStasStack.size() > 0; )
	{
		curNfaVec = nfaStasStack.top();
		nfaStasStack.pop();

		memset(compuFlag, 0, sizeof(compuFlag));
		for (size_t nCurChar = 0; nCurChar < DFACOLSIZE; ++nCurChar)
		{
			BYTE curGroup = m_pGroup[nCurChar];
			if(compuFlag[curGroup] == 1)
			{
				continue;
			}
			compuFlag[curGroup] = 1;

			nextNfaVec.clear();
			GetNextEClosureSet(nfa, eClosure, curNfaVec, nCurChar, nextNfaVec);
			if(!nextNfaVec.empty())
			{
				if(ssh.count(nextNfaVec) == 0)
				{
					m_pDfa->push_back(CDfaRow(m_nColNum));
					if (m_pDfa->size() > SC_STATELIMIT)
					{

						//std::cout << "SC_STATELIMIT!" << std::endl;
						return (size_t)-1;
					}
					STATEID nextSta = (STATEID)ssh.size();
					ssh[nextNfaVec] = nextSta;

					(*m_pDfa)[ssh[curNfaVec]][curGroup] = nextSta;

					// is final state
					if (nextNfaVec.back() == nfa.Size())
					{
						CDfaRow &lastRow = m_pDfa->back();
						lastRow.SetFlag(lastRow.GetFlag() | lastRow.TERMINAL);
						m_FinStas.PushBack(nextSta, m_nId);
					}
					nfaStasStack.push(nextNfaVec);
				}
				else
				{
					(*m_pDfa)[ssh[curNfaVec]][curGroup] = ssh[nextNfaVec];
				}
			}
		}
	}

	m_pDfa->shrink_to_fit();
	return 0;
}

DFANEWSC size_t CDfaNew::Minimize()
{
	//error: DFA is empty

	//CTimer time1;//用于测试
	size_t nSize = m_pDfa->size();
	size_t nCols = GetGroupCount();
	if (nSize == 0)
	{
		return size_t(-1);
	}
#ifdef DFA_REMOVE_UNREACHABLE
	size_t nMatHeight = nSize + 1;
	size_t nMatWidth = nMatHeight;

	if (nMatWidth % 16 != 0)
	{
		nMatWidth = (nMatWidth / 16) * 16 + 16;
	}
	BYTE *pMat = NULL;
	STATEVEC reachable;

	pMat = (BYTE*)_aligned_malloc(nMatWidth * nMatHeight, 128);
	memset(pMat, 0, nMatWidth * nMatHeight);

	for (size_t i = 0; i < nSize; ++i)
	{
		for (size_t j = 0; j < nCols; ++j)
		{
			STATEID nextSta = (STATEID)(*m_pDfa)[i][j];
			if (nextSta != STATEID(-1))
			{
				pMat[i * nMatWidth + nextSta] = 1;
			}
		}
	}
	for (STATEID i = 0; i < nSize; ++i)
	{
		if ((*m_pDfa)[i].GetFlag() & CDfaRow::TERMINAL)
		{
			pMat[i * nMatWidth + nSize] = 1;
		}
	}
	Warshall(pMat, nMatWidth, nMatHeight);

	size_t nStartRow = m_nStartId * nMatWidth;
	pMat[nStartRow + m_nStartId] = 1;
	for (size_t i = 0; i < nSize; ++i)
	{
		if (pMat[nStartRow + i] && pMat[i * nMatWidth + nSize])
		{
			reachable.push_back(i);
		}
	}
	_aligned_free(pMat);

	//std::cout << "准备工作： " << time1.Reset() << std::endl;//测试

	if (reachable.size() < nSize)
	{
		std::cout << "Has unreachables" << std::endl;
		system("pause");
		//remove unreachable states, generate new DFA
		MergeReachable(reachable);
	}
#endif
	////计算逆向状态查找表
	nSize = m_pDfa->size();
	STATEVEC *pRevTab = NULL;
	pRevTab = new STATEVEC[nSize * nCols];
	for (STATEID i = 0; i < nSize; ++i)
	{
		for (BYTE j = 0; j < nCols; ++j)
		{
			STATEID nDest = (STATEID)(*m_pDfa)[i][j];
			if (nDest != STATEID(-1))
			{
				pRevTab[nDest * nCols + j].push_back(STATEID(i));
			}
		}
	}

	std::vector<PARTSET> partSet;
	size_t nr = PartitionNonDisState(pRevTab, partSet);
	//divide nondistinguishable states
	if (0 == nr)
	{
		if (partSet.size() < nSize)
		{
			//DFA minization
			MergeNonDisStates(partSet);
			std::cout << "Minimized: " << nSize - partSet.size() << std::endl;
		}
	}

	delete []pRevTab;

	return nr;
}

DFANEWSC WORD CDfaNew::GetGroupCount() const
{
	return m_nColNum;
}

DFANEWSC BYTE CDfaNew::Char2Group(BYTE nIdx)
{
	return m_pGroup[nIdx];
}

DFANEWSC const BYTE CDfaNew::GetGroup(STATEID charNum) const
{
	return m_pGroup[charNum];
}

DFANEWSC STATEID CDfaNew::GetStartId() const
{
	return m_nStartId;
}

DFANEWSC void CDfaNew::SetStartId(STATEID id)
{
	m_nStartId = id;
}

DFANEWSC void CDfaNew::SetId(size_t id)
{
	m_nId = id;
}

DFANEWSC size_t CDfaNew::GetId()
{
	return m_nId;
}

DFANEWSC size_t CDfaNew::Process(BYTE *ByteStream, size_t len, CStateSet &StaSet)
{
	std::vector<bool> res(m_pDfa->size(), false);
	STATEID ActiveState = m_nStartId;
	size_t nPos = 0;
	for (nPos = 0; nPos < len; ++nPos)
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

DFANEWSC size_t CDfaNew::Save(BYTE *beg)
{
	BYTE *pOld = beg;

	//write dfa id
	WriteNum(beg, m_nId);

	//write number of dfa states
	WriteNum(beg, m_pDfa->size(), sizeof(BYTE));
	if (m_pDfa->size() == 0)
	{
		return beg - pOld;
	}

	//write group
	for (size_t i = 0; i < DFACOLSIZE; ++i)
	{
		WriteNum(beg, m_pGroup[i]);
	}

	//write dfa table
	for (size_t i = 0; i < m_pDfa->size(); ++i)
	{
		//write the flag of dfa state
		WriteNum(beg, (*m_pDfa)[i].GetFlag());
		for (BYTE j = 0; j < m_nColNum; ++j)
		{
			WriteNum(beg, (*m_pDfa)[i][j], sizeof(BYTE));
		}
	}
	
	//write the dfa's start state id
	WriteNum(beg, m_nStartId, sizeof(BYTE));

	//write the relationship between dfa's terminal state and dfa id
	WriteNum(beg, m_FinStas.GetAllDfaIdCount());
	for (size_t i = 0; i < m_FinStas.Size(); ++i)
	{
		STATEID nStaId = m_FinStas[i];
		CVectorUnsigned ids;
		m_FinStas.GetDfaIds(nStaId, ids);
		for (size_t j = 0; j < ids.Size(); ++j)
		{
			WriteNum(beg, nStaId, sizeof(BYTE));
			WriteNum(beg, ids[j]);
		}
	}

	return beg - pOld;
}

DFANEWSC void CDfaNew::Load(BYTE *beg, size_t len)
{
	len = 0;
	//read dfa id
	size_t dfaId;
	ReadNum(beg, dfaId);
	m_nId = dfaId;

	//read number of dfa states
	BYTE dfaSize;
	ReadNum(beg, dfaSize);
	if (dfaSize == 0)
	{
		return;
	}
	
	//read group
	BYTE pGroup[DFACOLSIZE];
	for (size_t i = 0; i < DFACOLSIZE; ++i)
	{
		ReadNum(beg, pGroup[i]);
	}
	Init(pGroup);
	m_nId = dfaId;

	//read dfa table
	m_pDfa->resize(dfaSize, CDfaRow(m_nColNum));
	size_t nFlag;
	for (size_t i = 0; i < m_pDfa->size(); ++i)
	{
		//read the flag of dfa state
		ReadNum(beg, nFlag);
		(*m_pDfa)[i].SetFlag(nFlag);
		for (BYTE j = 0; j < m_nColNum; ++j)
		{
			(*m_pDfa)[i][j] = 0;
			ReadNum(beg, (*m_pDfa)[i][j], sizeof(BYTE));
			if ((*m_pDfa)[i][j] == BYTE(-1))
			{
				(*m_pDfa)[i][j] = STATEID(-1);
			}
		}
	}
	
	//read the dfa's start state id
	m_nStartId = 0;
	ReadNum(beg, m_nStartId, sizeof(BYTE));
	
	//read the relationship between dfa's terminal state and dfa id
	size_t nFinStasSize;
	ReadNum(beg, nFinStasSize);
	for (size_t i = 0; i < nFinStasSize; ++i)
	{
		STATEID nStaId = 0;
		size_t nDfaId;
		ReadNum(beg, nStaId, sizeof(BYTE));
		ReadNum(beg, nDfaId);
		m_FinStas.PushBack(nStaId, nDfaId);
	}
}

DFANEWSC void CDfaNew::Dump(const char *pFile)
{
	std::ofstream fout(pFile);
	if(!fout)
	{
		std::cout << "file open error!" << std::endl;
	}

	fout << "字符和组对应关系：" << std::endl;
	for(BYTE i = 0; i < DFACOLSIZE - 1; ++i)
	{
		fout << (size_t)i << "\t" << (size_t)m_pGroup[i] << std::endl;
	}

	fout << 255 << "\t" << (size_t)m_pGroup[255] << std::endl;
	fout << "\t";
	for(BYTE j = 0; j != m_nColNum; ++j)
	{
		fout << (size_t)j << "\t";
	}
	fout << std::endl;
	for(size_t i = 0; i != m_pDfa->size(); ++i)
	{
		if(((*m_pDfa)[i].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			fout << i << ",Term\t";
		}
		else
		{
			fout << i << "\t";
		}
		for(BYTE j = 0; j != m_nColNum; ++j)
		{
			fout << (size_t)(*m_pDfa)[i][j] << "\t";
		}
		fout << std::endl;
	}
	fout << std::endl;
	fout.close();
}

//reachable中保留所有的可达状态，reachable中元素的取值有3个，
//0 表示该状态为孤立状态，1 表示该状态为不可达状态或者“死”状态，2 表示该状态为可达状态
void CDfaNew::MergeReachable(STATEVEC &reachable)
{
	size_t nDfaSize = m_pDfa->size();
	//标记终态的dfaId，以保证终态编号更改后，其对应的dfaId保持不变
	std::vector<size_t> finFlag(nDfaSize, size_t(-1));
	//for (size_t i = 0; i < m_FinStas.Size(); ++i)
	//{
	//	TERMSET &curSet = (*m_pTermSet)[i];
	//	termFlag[curSet.dfaSta] = curSet.dfaId;
	//}
	//m_pTermSet->clear();
	
	//统计可达状态的数目
	size_t nRcbCnt = reachable.size();
	size_t nColNum = GetGroupCount();

	//定义一个同成员变量m_pDfa类型相同的变量，用于存储删除多余状态后的DFA跳转表
	std::vector<CDfaRow> *pNewDfa = new std::vector<CDfaRow>(nRcbCnt, CDfaRow(nColNum));
	std::vector<CDfaRow> &tmpDfa = *pNewDfa;

	size_t nMemSize = nDfaSize * sizeof(STATEID);
	STATEID *pOldToNew = new STATEID[nMemSize];
	memset(pOldToNew, 0xFF, nMemSize);

	//将m_pDfa中编号对应为可达状态的复制到tmpDfa中，并修改reachable中可达状态的编号
	for (STATEVEC_ITER iter = reachable.begin(); iter != reachable.end(); ++iter)
	{
		STATEID nOldId = *iter, nNewId = STATEID(iter - reachable.begin());
		CDfaRow &oldRow = (*m_pDfa)[nOldId];
		tmpDfa[nNewId] = oldRow;
		pOldToNew[nOldId] = nNewId;

		//存入新的终态编号
		if (oldRow.GetFlag() & oldRow.TERMINAL)
		{
			//TERMSET tmpSta;
			//tmpSta.dfaSta = nNewId;
			//tmpSta.dfaId = termFlag[nOldId];
			//m_pTermSet->push_back(tmpSta);
		}
	}

	//将tmpDfa中的状态编号修改为相应的新编号
	for (STATEID i = 0; i < nRcbCnt; ++i)
	{
		CDfaRow &curRow = tmpDfa[i];
		for (BYTE j = 0; j < nColNum; ++j)
		{
			STATEID &cur = curRow[j];
			if (cur != STATEID(-1))
			{
				cur = pOldToNew[cur];
			}
		}
	}
	delete []pOldToNew;

	//替换m_pDfa
	delete m_pDfa;
	m_pDfa = pNewDfa;
}

void CDfaNew::InitPartSet(std::vector<PARTSET> &partSet) const
{
	size_t nStaNum = m_pDfa->size();

	partSet.clear();

	//用于区分属于不同DFA的终态集合
	std::set<size_t> *pTerm2Dfa = new std::set<size_t>[nStaNum];

	for (size_t i = 0; i < m_FinStas.Size(); ++i)
	{
		STATEID nFinStaId = m_FinStas[i];
		pTerm2Dfa[nFinStaId] = m_FinStas._GetDfaIds(nFinStaId);
	}

	//区别终态集合和非终态集合，map的first为空，则表示对应的PARTSET为非终态集合，反之，为终态集合
	//initBSet中集合无序
	std::map<std::set<size_t>, PARTSET> initBSet;
	for (STATEID i = 0; i < nStaNum; ++i)
	{
		std::set<size_t> &curDfaSet = pTerm2Dfa[i];
		initBSet[curDfaSet].StaSet.push_back(i);
	}
	delete []pTerm2Dfa;

	//调整终态和非终态集合的顺序，vector中的last为非终态集合
	for (std::map<std::set<size_t>, PARTSET>::iterator i = initBSet.begin();
		i != initBSet.end(); ++i)
	{
		if (i->first.size() != 0)
		{
			partSet.push_back(i->second);
		}
	}
	partSet.push_back(initBSet[std::set<size_t>()]);
}

size_t CDfaNew::PartitionNonDisState(STATEVEC *pRevTbl,
									 std::vector<PARTSET> &partSet) const
{
	size_t nGrpNum = GetGroupCount();
	size_t nStaNum = m_pDfa->size();

	InitPartSet(partSet);

	for (std::vector<PARTSET>::iterator i = partSet.begin();
		i != partSet.end(); ++i)
	{
		//对于partSet中每个集合，根据不同的nGrpNum计算不同AbleTo，AbleTo对应论文中的a(i)
		CalcAbleTo(pRevTbl, nGrpNum, nStaNum, *i);
	}

	//pWait表示用于切分partSet中每个集合的集合下标，
	//对于不同的输入字符会保存多个不同集合下标。对应论文中L(a)
	std::vector<size_t> pWait[256];

	//初始化pWait，要求若满足0<|ij|<=|ik|，则将j存入pWait，反之，存入k
	//对应论文中初始化L(a)过程，这里的i对应a
	for (size_t i = 0; i < nGrpNum; ++i)
	{
		size_t AcpSum = 0, NonAcpSum = 0;
		for (std::vector<PARTSET>::iterator j = partSet.begin();
			j != partSet.end() - 1; ++j)
		{
			AcpSum += j->Ones[i];
		}
		NonAcpSum = partSet.back().Ones[i];
		if (AcpSum != 0 && NonAcpSum == 0)
		{
			for (size_t k = 0; k < partSet.size() - 1; ++k)
			{
				pWait[i].push_back(k);
			}
		}
		else if (AcpSum == 0 && NonAcpSum != 0)
		{
			pWait[i].push_back(partSet.size() - 1);
		}
		else if (AcpSum != 0 && NonAcpSum != 0)
		{
			if (AcpSum < NonAcpSum)
			{
				pWait[i].push_back(partSet.size() - 1);
			}
			for (size_t k = 0; k < partSet.size() - 1; ++k)
			{
				pWait[i].push_back(k);
			}
		}
	}

	//标记能够经过某一输入字符，能够到达ISet中的状态的状态集合
	BYTE *pAbleToI = new BYTE[nStaNum];
	partSet.reserve(1000);
	size_t nr = 0;
	for (; nr == 0; )
	{
		BYTE byCurGrp = (BYTE)-1;
		size_t nCurSet = (size_t)-1;
		//从pWait中取一个值并remove该值
		for (size_t i = 0; i < nGrpNum; ++i)
		{
			if (!pWait[i].empty())
			{
				byCurGrp = (BYTE)i;
				nCurSet = pWait[i].back();
				pWait[i].pop_back();
				break;
			}
		}
		//最外层循环的终止条件
		if (nCurSet == -1)
		{
			break;
		}

		PARTSET *pISet = &partSet[nCurSet];
		ZeroMemory(pAbleToI, nStaNum);
		size_t nRevCnt = 0;
		//初始化pAbleToI
		for (STATELIST_ITER iSta = pISet->StaSet.begin();
			iSta != pISet->StaSet.end(); ++iSta)
		{
			size_t nRevIdx = *iSta * nGrpNum + byCurGrp;
			STATEVEC &revI = pRevTbl[nRevIdx];
			for (STATEVEC_ITER iRevSta = revI.begin();
				iRevSta != revI.end(); ++iRevSta)
			{
				pAbleToI[*iRevSta] = 1;
				++nRevCnt;
			}
		}

		if (nRevCnt != 0)
		{
			for (size_t j = 0; j != partSet.size(); ++j)
			{
				//取partSet中的一个集合j，B(j)中存在t满足条件δ(t,a)∈a(i)，
				//B’(j)为t的集合，B“(j)为B(j)-B‘(j)
				PARTSET *pJSet = &partSet[j];
				STATELIST_ITER t = pJSet->StaSet.begin();
				//将满足条件的t存在集合j的前段，不满足的存在集合j的后段
				//满足条件的t标记为1，先滤去pAbleToI中前段为1的值
				for (; t != pJSet->StaSet.end() && pAbleToI[*t] != 0; ++t);
				bool bHasAble = true;
				if (t == pJSet->StaSet.end())
				{
					continue;
				}
				if (t == pJSet->StaSet.begin())
				{
					bHasAble = false;
				}
				//将后段中出现的值为1的插入至前段
				for (; t != pJSet->StaSet.end();)
				{
					if (pAbleToI[*t] == 1)
					{
						pJSet->StaSet.insert(pJSet->StaSet.begin(), *t);
						t = pJSet->StaSet.erase(t);
						bHasAble = true;
					}
					else
					{
						++t;
					}
				}
				if (bHasAble == false)
				{
					continue;
				}

				partSet.push_back(PARTSET());
				pJSet = &partSet[j];
				pISet = &partSet[nCurSet];

				STATELIST_ITER part = pJSet->StaSet.begin();
				//查找产生0,1分段的切分点，记为part
				for (; part != pJSet->StaSet.end() && pAbleToI[*part] != 0; ++part);
				if (partSet.size() > 254)
				{
					nr = size_t(-1);
					break;
				}
				//保存产生的新的划分
				PARTSET &lastPart = partSet.back();
				lastPart.StaSet.splice(lastPart.StaSet.begin(),
					pJSet->StaSet, part, pJSet->StaSet.end());

				CalcAbleTo(pRevTbl, nGrpNum, nStaNum, *pJSet);
				CalcAbleTo(pRevTbl, nGrpNum, nStaNum, lastPart);


				////更新pWait
				for (BYTE m = 0; m < nGrpNum; ++m)
				{
					int k = partSet.size() - 1;
					std::vector<size_t> &curWait = pWait[m];
					int aj = pJSet->Ones[m];
					int ak = lastPart.Ones[m];
					if (aj > 0 && aj <= ak)
					{
						if (std::find(curWait.begin(), curWait.end(), j) == curWait.end())
						{
							k = j;
						}
					}
					curWait.push_back(k);
				}
			}
		}
	}
	delete []pAbleToI;

	for (std::vector<PARTSET>::iterator i = partSet.begin(); i != partSet.end(); ++i)
	{
		ReleaseAbleTo(*i);
	}
	return nr;
}

//Partition中的元素为一个状态的集合，集合中元素为多个等价状态，每个集合可以合并为新的DFA中一个状态
void CDfaNew::MergeNonDisStates(std::vector<PARTSET> &partSet)
{
	STATEVEC sta2Part(m_pDfa->size());
	
	STATEID nCol = (STATEID)GetGroupCount();

	//标记终态的dfaId，以保证终态编号更改后，其对应的dfaId保持不变
	std::set<size_t> *finFlag = new std::set<size_t>[m_pDfa->size()];
	for (size_t i = 0; i < m_FinStas.Size(); ++i)
	{
		STATEID nFinStaId = m_FinStas[i];
		finFlag[nFinStaId] = m_FinStas._GetDfaIds(nFinStaId);
	}

	m_FinStas.Clear();

	//定义一个同CDfaNew中成员变量m_pDfa类型相同的变量，用于存储合并后的DFA跳转表
	std::vector<CDfaRow> *pNewDfa = new std::vector<CDfaRow>(
		(STATEID)partSet.size(), CDfaRow(nCol));
	std::vector<CDfaRow> &tmpDfa = *pNewDfa;

	//等价的状态存于同一个partition中，标记原来的状态存在哪一个新的partition中，并修改新的起始状态编号
	STATEID nSetIdx = 0;
	for (std::vector<PARTSET>::iterator iPart = partSet.begin();
		iPart != partSet.end(); ++iPart)
	{
		for (STATELIST_ITER iSta = iPart->StaSet.begin();
			iSta != iPart->StaSet.end(); ++iSta)
		{
			CDfaRow &curRow = (*m_pDfa)[*iSta];
			sta2Part[*iSta] = nSetIdx;
			//修改新的起始状态
			if (curRow.GetFlag() & CDfaRow::START)
			{
				m_nStartId = nSetIdx;
			}

			//存入新的终态编号
			if (curRow.GetFlag() & CDfaRow::TERMINAL)
			{
				for (std::set<size_t>::iterator i = finFlag[*iSta].begin();
					i != finFlag[*iSta].end(); ++i)
				{
					m_FinStas.PushBack(nSetIdx, *i);
				}
			}
		}
		++nSetIdx;
	}

	delete []finFlag;

	//set new DFA and modify new number
	nSetIdx = 0;
	for (std::vector<PARTSET>::iterator iPart = partSet.begin(); iPart != partSet.end(); ++iPart)
	{
		CDfaRow &curRow = tmpDfa[nSetIdx];
		CDfaRow &orgRow = (*m_pDfa)[iPart->StaSet.front()];
		for (BYTE iCol = 0; iCol != nCol; ++iCol)
		{
			STATEID nDest = STATEID(-1);
			STATEID nCur = orgRow[iCol];
			if (nCur != STATEID(-1))
			{
				nDest = sta2Part[nCur];
			}
			curRow[iCol] = nDest;
		}
		//set a state attribute
		curRow.SetFlag(orgRow.GetFlag());
		++nSetIdx;
	}


	//替换m_pDfa
	swap(m_pDfa, pNewDfa);
	delete pNewDfa;
}

CFinalStates& CDfaNew::GetFinalState()
{
	return m_FinStas;
}

const CFinalStates& CDfaNew::GetFinalState() const
{
	return m_FinStas;
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

DFANEWSC void PrintDfaToGv(CDfaNew &newdfa, const char* fileName)
{
	std::ofstream fout(fileName);
	fout << "digraph G {" << std::endl;
	fout << "S -> " << (size_t)newdfa.GetStartId() << std::endl;

	for(STATEID i = 0; i != newdfa.Size(); ++i)
	{
		std::map<STATEID, size_t> rowStateCnt;
		for(BYTE j = 0; j != newdfa.GetGroupCount(); ++j)
		{
			rowStateCnt[newdfa[i][j]]++;
		}
		STATEID maxId = 0;
		for (std::map<STATEID, size_t>::iterator j = rowStateCnt.begin(); j != rowStateCnt.end(); ++j)
		{
			if (j->second > rowStateCnt[maxId])
			{
				maxId = j->first;
			}
		}
		for(BYTE j = 0; j != newdfa.GetGroupCount(); ++j)
		{
			if (newdfa[i][j] != maxId)
			{
				fout << i << " -> " << (size_t)newdfa[i][j] << " [label=\"" << j << "\"];" << std::endl;
			}
			else if (maxId != (STATEID)-1)
			{
				fout << i << " -> "  << (size_t)maxId << " [label=\"" << j << "\"];" << std::endl;
			}
		}
	}
	for (STATEID i = 0; i < newdfa.Size(); ++i)
	{
		if (newdfa[i].GetFlag() & CDfaRow::TERMINAL)
		{
			fout << (size_t)i << " [peripheries=2];" << std::endl;
		}
	}
	fout << "}" << std::endl;
	fout.close();
}

DFANEWSC bool MergeMultipleDfas(std::vector<CDfaNew> &dfas, CDfaNew &lastDfa)
{
	size_t dfaId = lastDfa.GetId();
	lastDfa.Clear();
	lastDfa.SetId(dfaId);
#undef max

	size_t dfasSize = dfas.size();
	STATEID nTermSta = 1;//nTermSta: terminal flag. 1: terminal, -1: non-terminal

	//group the lastDfa's columns
	BYTE groups[DFACOLSIZE];
	DfaColGroup(dfas, groups);
	lastDfa.Init(groups);

	size_t colCnt = lastDfa.GetGroupCount();

	typedef std::unordered_map<std::vector<STATEID>, STATEID, TODFA_HASH> STATESETHASH;
	STATESETHASH statehash;

	size_t finFlag = 0;//terminal state flag, 1: terminal state, 0: normal state
	std::stack<std::vector<STATEID>> statesStack;

	/*
	**	use a size of (dfas.size() + 2) vector to represent a state of the merged dfa.
	**	element 0 represents the state of dfas[0], ..., element n represents the state of dfas[n].
	**	element n + 1 and element n + 2 are flags which show that whether this state is a start state or terminal state.
	*/
	std::vector<STATEID> startVec(dfasSize + 2);
	
	lastDfa.Reserve(CHARSETSIZE);
	lastDfa.Resize(lastDfa.Size() + 1, colCnt);

	for(size_t i = 0; i < dfasSize; ++i)
	{
		STATEID nSta = dfas[i].GetStartId();
		if((dfas[i][nSta].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			//this is a terminal state
			finFlag = 1;
			AddTermIntoDFA(nSta, dfas[i], 0, lastDfa);
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
		startVec[dfasSize + 1] = (STATEID)-1;
		lastDfa[0].SetFlag(lastDfa[0].GetFlag() | CDfaRow::START);
	}
	
	statehash[startVec] = 0;

	statesStack.push(startVec);

	std::vector<STATEID> NextVec;
	NextVec.resize(dfasSize + 2);
	BYTE computFlag[CHARSETSIZE];

	while(!statesStack.empty())
	{
		std::vector<STATEID> curVec = statesStack.top();//current state, size is:dfasSize + 2
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
					BYTE curgroup = dfas[i].Char2Group((BYTE)curChar);
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
			NextVec[dfasSize] = (STATEID)-1;
			if(finFlag)
			{
				NextVec[dfasSize + 1] = nTermSta;
			}
			else
			{
				NextVec[dfasSize + 1] = (STATEID)-1;
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
						for(STATEID k = 0; k < dfasSize; ++k)
						{
							if(NextVec[k] != (STATEID)-1)
							{
								if((dfas[k][NextVec[k]].GetFlag() & CDfaRow::TERMINAL) != 0)
								{
									AddTermIntoDFA(NextVec[k], dfas[k], nextSta, lastDfa);
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
	//lastDfa.Minimize();
	if(lastDfa.Size() > DFA_SIZE_LIMIT)
	{
		//std::cerr << "DFA_SIZE_LIMIT!" << std::endl;
		return false;
	}

	return true;
}

