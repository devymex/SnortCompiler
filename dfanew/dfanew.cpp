#include "stdafx.h"
#include "dfanew.h"
#include "dfaalgo.h"

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
	m_pTermSet = new std::vector<TERMSET>;
}

DFANEWSC CDfaNew::~CDfaNew()
{
	delete m_pDfa;
	delete m_pTermSet;
}

DFANEWSC CDfaNew::CDfaNew(const CDfaNew &other)
{
	m_pDfa = new std::vector<CDfaRow>;
	m_pTermSet = new std::vector<TERMSET>;
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
	*m_pTermSet = *other.m_pTermSet;
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

DFANEWSC void CDfaNew::PushBackTermSet(TERMSET &term)
{
	m_pTermSet->push_back(term);
}

DFANEWSC void CDfaNew::UniqueTermSet()
{
	struct EQUAL
	{
		bool operator()(TERMSET &t1, TERMSET &t2)
		{
			if((t1.dfaId == t2.dfaId) && (t1.dfaSta == t2.dfaSta))
			{
				return true;
			}
			return false;
		}
	};
	struct LESS
	{
		bool operator()(TERMSET &t1, TERMSET &t2)
		{
			if((t1.dfaSta < t2.dfaSta) || ((t1.dfaSta == t2.dfaSta) && (t1.dfaId < t2.dfaId)))
			{
				return true;
			}
			return false;
		}
	};
	std::sort(m_pTermSet->begin(), m_pTermSet->end(), LESS());
	m_pTermSet->erase(std::unique(m_pTermSet->begin(), m_pTermSet->end(),
		EQUAL()), m_pTermSet->end());
}

DFANEWSC TERMSET& CDfaNew::GetTerm(size_t nIdx) const
{
	return (*m_pTermSet)[nIdx];
}

DFANEWSC size_t CDfaNew::GetTermCnt() const
{
	return m_pTermSet->size();
}

DFANEWSC TERMSET& CDfaNew::BackTermSet()
{
	return m_pTermSet->back();
}

DFANEWSC void CDfaNew::Init(BYTE *pGroup)
{
	//Clear();
	BYTE occurred[DFACOLSIZE] = {0};
	for (size_t i = 0; i < DFACOLSIZE; ++i) 
	{
		occurred[pGroup[i]] = 1;
	}
	bool flag = true;
	WORD nZeroBegPos = 0;
	for (; nZeroBegPos < DFACOLSIZE; ++nZeroBegPos)
	{
		if (occurred[nZeroBegPos] == 0)
		{
			break;
		}
	}
	for (size_t i = nZeroBegPos + 1; i < DFACOLSIZE; ++i)
	{
		if (occurred[i] == 1)
		{
			flag = false;
			break;
		}
	}
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
	m_pTermSet->clear();
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
	}

	std::vector<size_t> curNfaVec;
	static std::vector<size_t> nextNfaVec; //
	nextNfaVec.clear();
	BYTE compuFlag[CHARSETSIZE];

	size_t nTotalSize = m_pTermSet->size() * sizeof(TERMSET) +
				sizeof(m_pGroup) + sizeof(m_nStartId) + sizeof(m_nId) +
				m_pDfa->size() * m_nColNum;

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
					if (m_pDfa->size() > SC_STATELIMIT)// || nTotalSize >= 2048
					{

						//std::cout << "SC_STATELIMIT!" << std::endl;
						return (size_t)-1;
					}
					STATEID nextSta = (STATEID)ssh.size();
					ssh[nextNfaVec] = nextSta;

					nTotalSize += m_nColNum;
					(*m_pDfa)[ssh[curNfaVec]][curGroup] = nextSta;

					// is final state
					if (nextNfaVec.back() == nfa.Size())
					{
						CDfaRow &lastRow = m_pDfa->back();
						lastRow.SetFlag(lastRow.GetFlag() | lastRow.TERMINAL);
						m_pTermSet->push_back(TERMSET());
						m_pTermSet->back().dfaSta = nextSta;
						m_pTermSet->back().dfaId = m_nId;
						nTotalSize += sizeof(TERMSET);
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

DFANEWSC void CDfaNew::printTerms()
{
	for(std::vector<TERMSET>::iterator iter = m_pTermSet->begin(); iter != m_pTermSet->end(); ++iter)
	{
		std::cout << (size_t)iter->dfaSta <<"  :  " << "dfa " << iter->dfaId << std::endl;
	}
}

DFANEWSC size_t CDfaNew::Minimize()
{
	//error: DFA is empty

	//CTimer time1;//���ڲ���
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

	//std::cout << "׼�������� " << time1.Reset() << std::endl;//����

	if (reachable.size() < nSize)
	{
		std::cout << "Has unreachables" << std::endl;
		system("pause");
		//remove unreachable states, generate new DFA
		MergeReachable(reachable);
	}
#endif
	////��������״̬���ұ�
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

DFANEWSC void CDfaNew::GetAcceptedId(STATEID id, CVectorUnsigned &dfaIds)
{
	struct COMPFORSORT
	{
		bool operator()(const TERMSET &t1, const TERMSET &t2)
		{
			if (t1.dfaSta < t2.dfaSta || (t1.dfaSta == t2.dfaSta && t1.dfaId < t2.dfaId))
			{
				return true;
			}
			return false;
		}
	};

	struct TERMSETCMP
	{
		bool operator()(const TERMSET &t1, const TERMSET &t2)
		{
			if (t1.dfaSta < t2.dfaSta)
			{
				return true;
			}
			return false;
		}
	};

	std::sort(m_pTermSet->begin(), m_pTermSet->end(), COMPFORSORT());
	std::vector<TERMSET>::iterator Beg = std::lower_bound(m_pTermSet->begin(),
		m_pTermSet->end(), TERMSET(id, 0), TERMSETCMP());
	std::vector<TERMSET>::iterator End = std::upper_bound(m_pTermSet->begin(),
		m_pTermSet->end(), TERMSET(id, 0), TERMSETCMP());
	for (std::vector<TERMSET>::iterator i = Beg; i != End; ++i)
	{
		dfaIds.PushBack(i->dfaId);
	}
	dfaIds.Unique();
}

DFANEWSC size_t CDfaNew::Save(BYTE *beg)
{
	BYTE *pOld = beg;
	//дDFA��Id
	WriteNum(beg, m_nId);
	//дDFA��״̬����
	WriteNum(beg, m_pDfa->size(), sizeof(BYTE));
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
		for (BYTE j = 0; j < m_nColNum; ++j)
		{
			WriteNum(beg, (*m_pDfa)[i][j], sizeof(BYTE));
		}
	}
	//дDFA�Ŀ�ʼ״̬���
	WriteNum(beg, m_nStartId, sizeof(BYTE));
	//дDFA����̬��DFAId�Ķ�Ӧ��ϵ
	WriteNum(beg, m_pTermSet->size());
	for (size_t i = 0; i < m_pTermSet->size(); ++i)
	{
		WriteNum(beg, (*m_pTermSet)[i].dfaSta, sizeof(BYTE));
		WriteNum(beg, (*m_pTermSet)[i].dfaId);
	}

	return beg - pOld;
}

DFANEWSC void CDfaNew::Load(BYTE *beg, size_t len)
{
	len = 0;
	//��DFA��Id
	size_t dfaId;
	ReadNum(beg, dfaId);
	m_nId = dfaId;
	//��DFA��״̬����
	BYTE dfaSize;//DFA��״̬��
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
	m_pDfa->resize(dfaSize, CDfaRow(m_nColNum));
	size_t nFlag;
	for (size_t i = 0; i < m_pDfa->size(); ++i)
	{
		//����״̬��Flag(NORMAL��START��TERMINAL)
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
	//��DFA�Ŀ�ʼ״̬���
	m_nStartId = 0;
	ReadNum(beg, m_nStartId, sizeof(BYTE));
	//��DFA����̬��DFAId�Ķ�Ӧ��ϵ
	size_t TermSetSize;
	ReadNum(beg, TermSetSize);
	m_pTermSet->resize(TermSetSize);
	for (size_t i = 0; i < TermSetSize; ++i)
	{
		(*m_pTermSet)[i].dfaSta = 0;
		ReadNum(beg, (*m_pTermSet)[i].dfaSta, sizeof(BYTE));
		ReadNum(beg, (*m_pTermSet)[i].dfaId);
	}
}

DFANEWSC void CDfaNew::Dump(const char *pFile)
{
	std::ofstream fout(pFile);
	if(!fout)
	{
		std::cout << "file open error!" << std::endl;
	}

	fout << "�ַ������Ӧ��ϵ��" << std::endl;
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
	fout << "dfa state" << "\t" << "dfaId" << std::endl;
	for(size_t i = 0; i < m_pTermSet->size(); ++i)
	{
		fout << (size_t)(*m_pTermSet)[i].dfaSta << "\t" <<
			(*m_pTermSet)[i].dfaId << std::endl;
	}
	fout.close();
}

//reachable�б������еĿɴ�״̬��reachable��Ԫ�ص�ȡֵ��3����
//0 ��ʾ��״̬Ϊ����״̬��1 ��ʾ��״̬Ϊ���ɴ�״̬���ߡ�����״̬��2 ��ʾ��״̬Ϊ�ɴ�״̬
void CDfaNew::MergeReachable(STATEVEC &reachable)
{
	size_t nDfaSize = m_pDfa->size();
	//�����̬��dfaId���Ա�֤��̬��Ÿ��ĺ����Ӧ��dfaId���ֲ���
	std::vector<size_t> termFlag(nDfaSize, size_t(-1));
	for (size_t i = 0; i < m_pTermSet->size(); ++i)
	{
		TERMSET &curSet = (*m_pTermSet)[i];
		termFlag[curSet.dfaSta] = curSet.dfaId;
	}
	m_pTermSet->clear();
	
	//ͳ�ƿɴ�״̬����Ŀ
	size_t nRcbCnt = reachable.size();
	size_t nColNum = GetGroupCount();

	//����һ��ͬ��Ա����m_pDfa������ͬ�ı��������ڴ洢ɾ������״̬���DFA��ת��
	std::vector<CDfaRow> *pNewDfa = new std::vector<CDfaRow>(nRcbCnt, CDfaRow(nColNum));
	std::vector<CDfaRow> &tmpDfa = *pNewDfa;

	size_t nMemSize = nDfaSize * sizeof(STATEID);
	STATEID *pOldToNew = new STATEID[nMemSize];
	memset(pOldToNew, 0xFF, nMemSize);

	//��m_pDfa�б�Ŷ�ӦΪ�ɴ�״̬�ĸ��Ƶ�tmpDfa�У����޸�reachable�пɴ�״̬�ı��
	for (STATEVEC_ITER iter = reachable.begin(); iter != reachable.end(); ++iter)
	{
		STATEID nOldId = *iter, nNewId = STATEID(iter - reachable.begin());
		CDfaRow &oldRow = (*m_pDfa)[nOldId];
		tmpDfa[nNewId] = oldRow;
		pOldToNew[nOldId] = nNewId;

		//�����µ���̬���
		if (oldRow.GetFlag() & oldRow.TERMINAL)
		{
			TERMSET tmpSta;
			tmpSta.dfaSta = nNewId;
			tmpSta.dfaId = termFlag[nOldId];
			m_pTermSet->push_back(tmpSta);
		}
	}

	//��tmpDfa�е�״̬����޸�Ϊ��Ӧ���±��
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

	//�滻m_pDfa
	delete m_pDfa;
	m_pDfa = pNewDfa;
}

void CDfaNew::InitPartSet(std::vector<PARTSET> &partSet) const
{
	size_t nStaNum = m_pDfa->size();

	partSet.clear();

	//�����������ڲ�ͬDFA����̬����
	std::set<size_t> *pTerm2Dfa = new std::set<size_t>[nStaNum];

	for (STATEID i = 0; i < m_pTermSet->size(); ++i)
	{
		TERMSET &ts = (*m_pTermSet)[i];
		pTerm2Dfa[ts.dfaSta].insert(ts.dfaId);
	}

	//������̬���Ϻͷ���̬���ϣ�map��firstΪ�գ����ʾ��Ӧ��PARTSETΪ����̬���ϣ���֮��Ϊ��̬����
	//initBSet�м�������
	std::map<std::set<size_t>, PARTSET> initBSet;
	for (STATEID i = 0; i < nStaNum; ++i)
	{
		std::set<size_t> &curDfaSet = pTerm2Dfa[i];
		initBSet[curDfaSet].StaSet.push_back(i);
	}
	delete []pTerm2Dfa;

	//������̬�ͷ���̬���ϵ�˳��vector�е�lastΪ����̬����
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
		//����partSet��ÿ�����ϣ����ݲ�ͬ��nGrpNum���㲻ͬAbleTo��AbleTo��Ӧ�����е�a(i)
		CalcAbleTo(pRevTbl, nGrpNum, nStaNum, *i);
	}

	//pWait��ʾ�����з�partSet��ÿ�����ϵļ����±꣬
	//���ڲ�ͬ�������ַ��ᱣ������ͬ�����±ꡣ��Ӧ������L(a)
	std::vector<size_t> pWait[256];

	//��ʼ��pWait��Ҫ��������0<|ij|<=|ik|����j����pWait����֮������k
	//��Ӧ�����г�ʼ��L(a)���̣������i��Ӧa
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

	//����ܹ�����ĳһ�����ַ����ܹ�����ISet�е�״̬��״̬����
	BYTE *pAbleToI = new BYTE[nStaNum];
	partSet.reserve(1000);
	size_t nr = 0;
	for (; nr == 0; )
	{
		BYTE byCurGrp = (BYTE)-1;
		size_t nCurSet = (size_t)-1;
		//��pWait��ȡһ��ֵ��remove��ֵ
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
		//�����ѭ������ֹ����
		if (nCurSet == -1)
		{
			break;
		}

		PARTSET *pISet = &partSet[nCurSet];
		ZeroMemory(pAbleToI, nStaNum);
		size_t nRevCnt = 0;
		//��ʼ��pAbleToI
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
				//ȡpartSet�е�һ������j��B(j)�д���t����������(t,a)��a(i)��
				//B��(j)Ϊt�ļ��ϣ�B��(j)ΪB(j)-B��(j)
				PARTSET *pJSet = &partSet[j];
				STATELIST_ITER t = pJSet->StaSet.begin();
				//������������t���ڼ���j��ǰ�Σ�������Ĵ��ڼ���j�ĺ��
				//����������t���Ϊ1������ȥpAbleToI��ǰ��Ϊ1��ֵ
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
				//������г��ֵ�ֵΪ1�Ĳ�����ǰ��
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
				//���Ҳ���0,1�ֶε��зֵ㣬��Ϊpart
				for (; part != pJSet->StaSet.end() && pAbleToI[*part] != 0; ++part);
				if (partSet.size() > 254)
				{
					nr = size_t(-1);
					break;
				}
				//����������µĻ���
				PARTSET &lastPart = partSet.back();
				lastPart.StaSet.splice(lastPart.StaSet.begin(),
					pJSet->StaSet, part, pJSet->StaSet.end());

				CalcAbleTo(pRevTbl, nGrpNum, nStaNum, *pJSet);
				CalcAbleTo(pRevTbl, nGrpNum, nStaNum, lastPart);


				////����pWait
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

//Partition�е�Ԫ��Ϊһ��״̬�ļ��ϣ�������Ԫ��Ϊ����ȼ�״̬��ÿ�����Ͽ��Ժϲ�Ϊ�µ�DFA��һ��״̬
void CDfaNew::MergeNonDisStates(std::vector<PARTSET> &partSet)
{
	STATEVEC sta2Part(m_pDfa->size());
	
	STATEID nCol = (STATEID)GetGroupCount();

	//�����̬��dfaId���Ա�֤��̬��Ÿ��ĺ����Ӧ��dfaId���ֲ���
	std::vector<size_t> *termFlag = new std::vector<size_t>[m_pDfa->size()];
	for (size_t i = 0; i < m_pTermSet->size(); ++i)
	{
		termFlag[(*m_pTermSet)[i].dfaSta].push_back((*m_pTermSet)[i].dfaId);
	}
	m_pTermSet->clear();

	//����һ��ͬCDfaNew�г�Ա����m_pDfa������ͬ�ı��������ڴ洢�ϲ����DFA��ת��
	std::vector<CDfaRow> *pNewDfa = new std::vector<CDfaRow>(
		(STATEID)partSet.size(), CDfaRow(nCol));
	std::vector<CDfaRow> &tmpDfa = *pNewDfa;

	//�ȼ۵�״̬����ͬһ��partition�У����ԭ����״̬������һ���µ�partition�У����޸��µ���ʼ״̬���
	STATEID nSetIdx = 0;
	for (std::vector<PARTSET>::iterator iPart = partSet.begin();
		iPart != partSet.end(); ++iPart)
	{
		for (STATELIST_ITER iSta = iPart->StaSet.begin();
			iSta != iPart->StaSet.end(); ++iSta)
		{
			CDfaRow &curRow = (*m_pDfa)[*iSta];
			sta2Part[*iSta] = nSetIdx;
			//�޸��µ���ʼ״̬
			if (curRow.GetFlag() & CDfaRow::START)
			{
				m_nStartId = nSetIdx;
			}

			//�����µ���̬���
			if (curRow.GetFlag() & CDfaRow::TERMINAL)
			{
				TERMSET tmpSta;
				tmpSta.dfaSta = nSetIdx;
				for (std::vector<size_t>::iterator i = termFlag[*iSta].begin(); i != termFlag[*iSta].end(); ++i)
				{
					tmpSta.dfaId = *i;
					m_pTermSet->push_back(tmpSta);
				}
			}
		}
		++nSetIdx;
	}

	delete []termFlag;
	UniqueTermSet();

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


	//�滻m_pDfa
	swap(m_pDfa, pNewDfa);
	delete pNewDfa;
}

