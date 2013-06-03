#include "stdafx.h"
#include "dfanew.h"
#include "NCreDfa.h"

struct PARTSET
{
	std::list<STATEID> StaSet;
	std::vector<BYTE*> AbleTo;
	std::vector<STATEID> Ones;
};

void fdisplay(CDfaNew &newdfa, const char* fileName)
{
	std::ofstream fout(fileName);
	fout << "digraph G {" << std::endl;
	fout << "S -> " << (size_t)newdfa.GetStartId() << std::endl;

	for(size_t i = 0; i != newdfa.Size(); ++i)
	{
		std::map<STATEID, size_t> rowStateCnt;
		for(size_t j = 0; j != newdfa.GetGroupCount(); ++j)
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
		for(size_t j = 0; j != newdfa.GetGroupCount(); ++j)
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
	for (size_t i = 0; i < newdfa.Size(); ++i)
	{
		if (newdfa[i].GetFlag() & CDfaRow::TERMINAL)
		{
			fout << (size_t)i << " [peripheries=2];" << std::endl;
		}
	}
	fout << "}" << std::endl;
	fout.close();
}


//���Ժ���:���һ��nfa
DFANEWSC void outPut(CNfa &nfa, const char* fileName)
{
	size_t stateNum = nfa.Size();
	std::ofstream fout(fileName);
	fout << "\t";
	for(size_t t = 0; t < 257; ++t)
	{
		fout << t << "\t";
	}
	fout << std::endl;
	for(size_t i = 0; i < stateNum; ++i)
	{
		fout << i << "\t";
		const CNfaRow &row = nfa[i];
		for(size_t j = 0; j < 257; ++j)
		{
			size_t nCnt = row.DestCnt(j);
			if(nCnt == 0)
			{
				fout << -1 << "\t";
			}
			else
			{
				for(size_t k = 0; k < nCnt; ++k)
				{
					fout << row.GetDest(j, k) << ", ";
				}
				fout << "\t";
			}
		}
		fout << std::endl;
	}
	fout.close();
}

//�������һ��dfa
DFANEWSC void outPutDfa(CDfaNew &dfa, const char* filename)
{
	std::ofstream fout(filename);
	if(!fout)
	{
		std::cout << "file open error!" << std::endl;
	}

	fout << "�ַ������Ӧ��ϵ��" << std::endl;
	for(BYTE i = 0; i < DFACOLSIZE - 1; ++i)
	{
		fout << (size_t)i << "\t" << (size_t)dfa.Char2Group(i) << std::endl;
	}

	fout << 255 << "\t" << (size_t)dfa.Char2Group(255) << std::endl;
	fout << "\t";
	//for(size_t k = 0; k != dfa.GetGroupCount(); ++k)
	//{
	//	fout << dfa.GetGroup[k] << "\t";
	//}
	for(BYTE j = 0; j != dfa.GetGroupCount(); ++j)
	{
		fout << (size_t)j << "\t";
	}
	fout << std::endl;
	for(size_t i = 0; i != dfa.Size(); ++i)
	{
		if((dfa[i].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			fout << i << ",Term\t";
		}
		else
		{
			fout << i << "\t";
		}
		for(BYTE j = 0; j != dfa.GetGroupCount(); ++j)
		{
			fout << (size_t)dfa[i][j] << "\t";
		}
		fout << std::endl;
	}
	fout << std::endl;
	for(size_t i = 0; i < dfa.GetTermCnt(); ++i)
	{
		fout << (size_t)dfa.GetTerm(i).dfaSta << "\t" << dfa.GetTerm(i).dfaId << std::endl;
	}
	fout.close();
}

DFANEWSC CDfaNew::CDfaNew()
	: m_nId(size_t(-1)), m_nColNum(size_t(0)), m_StartId(STATEID(0))
{
	std::fill(m_pGroup, m_pGroup + DFACOLSIZE, BYTE(-1));
	m_pDfa = new std::vector<CDfaRow>;
	m_TermSet = new std::vector<TERMSET>;
}

DFANEWSC CDfaNew::~CDfaNew()
{
	delete m_pDfa;
	delete m_TermSet;
}

DFANEWSC CDfaNew::CDfaNew(const CDfaNew &other)
{
	m_pDfa = new std::vector<CDfaRow>;
	m_TermSet = new std::vector<TERMSET>;
	*this = other;
}

DFANEWSC CDfaNew& CDfaNew::operator=(const CDfaNew &other)
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

DFANEWSC size_t CDfaNew::Size() const
{
	return m_pDfa->size();
}

DFANEWSC CDfaRow& CDfaNew::BackRow()
{
	return m_pDfa->back();
}
DFANEWSC void CDfaNew::ReservRow(size_t nCount)
{
	m_pDfa->reserve(nCount);
}
DFANEWSC void CDfaNew::ResizeRow(size_t nSize, size_t nCol)
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

DFANEWSC void CDfaNew::reserve(size_t Maxnum)
{
	m_pDfa->reserve(Maxnum);
}

DFANEWSC void CDfaNew::PushBackDfa(CDfaRow &sta)
{
	m_pDfa->push_back(sta);
}

DFANEWSC void CDfaNew::PushBackTermSet(TERMSET &term)
{
	m_TermSet->push_back(term);
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
			else
			{
				return false;
			}
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
			else
			{
				return false;
			}
		}
	};
	std::sort(m_TermSet->begin(), m_TermSet->end(), LESS());
	m_TermSet->erase(std::unique(m_TermSet->begin(), m_TermSet->end(), EQUAL()), m_TermSet->end());
}

DFANEWSC TERMSET& CDfaNew::GetTerm(size_t nIdx) const
{
	return (*m_TermSet)[nIdx];
}


//����other��sta����termset�����ҵ���TERMSET���뵽��this��m_TermSet�У�����this��״̬��thisSta��
//DFANEWSC size_t CDfaNew::AddTermIntoDFA(STATEID sta, const CDfaNew &other, STATEID thisSta)
//{
//	size_t flag = (size_t)-1;
//	for(size_t i = 0; i < other.GetTermCnt(); ++i)
//	{
//		if(other.GetTerm(i).dfaSta == sta)
//		{
//			flag = 0;
//			m_TermSet->push_back(TERMSET());
//			m_TermSet->back().dfaSta = thisSta;
//			m_TermSet->back().dfaId = other.GetTerm(i).dfaId;
//		}
//	}
//	return flag;
//}

DFANEWSC size_t CDfaNew::GetTermCnt() const
{
	return m_TermSet->size();
}

DFANEWSC TERMSET& CDfaNew::BackTermSet()
{
	return m_TermSet->back();
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
	size_t ZeroBegPos = 0;
	for (; ZeroBegPos < DFACOLSIZE; ++ZeroBegPos)
	{
		if (occurred[ZeroBegPos] == 0)
		{
			break;
		}
	}
	for (size_t i = ZeroBegPos + 1; i < DFACOLSIZE; ++i)
	{
		if (occurred[i] == 1)
		{
			flag = false;
			break;
		}
	}
	if (flag)
	{
		m_nColNum = ZeroBegPos;
		CopyMemory(m_pGroup, pGroup, DFACOLSIZE * sizeof(BYTE));
	}
	else
	{
		std::cerr << "Group error!" << std::endl;
	}
	//std::vector<BYTE> tmpGroup;
	//std::copy(pGroup, pGroup + DFACOLSIZE, std::back_inserter(tmpGroup));
	//std::sort(tmpGroup.begin(), tmpGroup.end());
	//tmpGroup.erase(std::unique(tmpGroup.begin(), tmpGroup.end()), tmpGroup.end());
	//if (tmpGroup.back() == tmpGroup.size() - 1)
	//{
	//	m_nColNum = tmpGroup.size();
	//	CopyMemory(m_pGroup, pGroup, DFACOLSIZE * sizeof(BYTE));
	//}
	//else
	//{
	//	std::cerr << "Group error!" << std::endl;
	//}
}

DFANEWSC void CDfaNew::Clear()
{
	m_nId = size_t(-1);
	m_nColNum = size_t(0);
	m_StartId = STATEID(0);
	memset(m_pGroup, 0xFF, DFACOLSIZE);
	//std::fill(m_pGroup, m_pGroup + DFACOLSIZE, BYTE(-1));
	m_pDfa->clear();
	m_TermSet->clear();
	//delete m_pDfa;
	//delete m_TermSet;
	//m_pDfa = new std::vector<CDfaRow>;
	//m_TermSet = new std::vector<TERMSET>;
}

typedef std::vector<size_t> STATESET;

void Warshall(BYTE *pMat, size_t nWidth, size_t nHeight)
{
	for (size_t k = 0; k < nHeight; ++k)
	{
		for (size_t i = 0; i < nHeight; ++i)
		{
			size_t row = i * nWidth;
			if (pMat[row + k])
			{
				__m128i *pBeg1 = (__m128i*)(pMat + row);
				__m128i *pBeg2 = (__m128i*)(pMat + k * nWidth);
				size_t nInts = nWidth >> 4;
				for (size_t j = 0; j < nInts; ++j)
				{
					*pBeg1 = _mm_or_si128(*pBeg1, *pBeg2);
					++pBeg1;
					++pBeg2;
				}
			}
		}
	}
}

void NfaEClosure(const CNfa &nfa, std::vector<STATESET> &eClosure)
{
	//printNfa(nfa);
	size_t nNfaSize = nfa.Size();
	size_t nMatHeight = nNfaSize + 1;
	size_t nMatWidth = nMatHeight;
	if (nMatWidth % 16 != 0)
	{
		nMatWidth = (nMatWidth / 16) * 16 + 16;
	}
	BYTE *pMat = (BYTE*)_aligned_malloc(nMatWidth * nMatHeight, 128);
	memset(pMat, 0, nMatWidth * nMatHeight);

	for (size_t i = 0; i < nNfaSize; ++i)
	{
		const CNfaRow &row = nfa[i];
		size_t nCnt = row.DestCnt(EMPTY);
		const size_t *pDest = row.GetCol(EMPTY);
		for (size_t k = 0; k < nCnt; ++k)
		{
			pMat[i * nMatWidth + pDest[k]] = 1;
		}
		pMat[i * nMatWidth + i] = 1;
	}

	Warshall(pMat, nMatWidth, nMatHeight);

	eClosure.resize(nNfaSize + 1);
	for (size_t i = 0; i < nNfaSize; ++i)
	{
		for (size_t j = 0; j < nMatWidth; ++j)
		{
			if (pMat[i * nMatWidth + j])
			{
				eClosure[i].push_back(j);
			}
		}
	}
	eClosure[nNfaSize].push_back(nNfaSize);
	_aligned_free(pMat);
}

void GetNextEClosureSet(const CNfa &nfa, const std::vector<STATESET> &eClosure,
	const STATESET &curSet, size_t edge, STATESET &eClosureSet)
{
	if (edge >= CHARSETSIZE)
	{
		std::cout << "Fatal Error!" << std::endl;
		return;
	}

	static STATESET nextSet;
	nextSet.clear();
	nextSet.reserve(1000);
	size_t nSize = nfa.Size();
	for(STATESET::const_iterator i = curSet.begin(); i != curSet.end(); ++i)
	{
		size_t nCurSta = *i;
		if(nCurSta != nSize)
		{
			const CNfaRow &row = nfa[nCurSta];
			size_t nCurCnt = nextSet.size();
			size_t nAddCnt = row.DestCnt(edge);
			const size_t *pStates = row.GetCol(edge);
			if (nAddCnt != 0)
			{
				if (nAddCnt == 1)
				{
					nextSet.push_back(*pStates);
				}
				else
				{
					nextSet.resize(nCurCnt + nAddCnt);
					memcpy(nextSet.data() + nCurCnt, pStates, nAddCnt * sizeof(size_t));
				}
			}
		}
	}
	std::sort(nextSet.begin(), nextSet.end());
	nextSet.erase(std::unique(nextSet.begin(), nextSet.end()), nextSet.end());

	for (STATESET::const_iterator i = nextSet.cbegin(); i != nextSet.cend(); ++i)
	{
		const std::vector<size_t> &ci = eClosure[*i];
		eClosureSet.insert(eClosureSet.end(), ci.cbegin(), ci.cend());
	}
	std::sort(eClosureSet.begin(), eClosureSet.end());
	eClosureSet.erase(std::unique(eClosureSet.begin(),
		eClosureSet.end()), eClosureSet.end());
}

DFANEWSC size_t CDfaNew::FromNFA(const CNfa &nfa, NFALOG *nfalog, size_t Count, bool combine)
{
	typedef std::unordered_map<std::vector<size_t>, STATEID, NSTATESET_HASH> STATESETHASH;
	std::vector<std::pair<std::vector<size_t>, STATEID>> termStasVec;

	size_t dfaId = m_nId;
	Clear();
	m_nId = dfaId;
	
	size_t nNfaSize = nfa.Size();
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
	static std::vector<size_t> nextNfaVec;
	nextNfaVec.clear();
	BYTE compuFlag[CHARSETSIZE];

	size_t nTotalSize = m_TermSet->size() * sizeof(TERMSET) +
				sizeof(m_pGroup) + sizeof(m_StartId) + sizeof(m_nId) +
				m_pDfa->size() * m_nColNum;

	for (; nfaStasStack.size() > 0; )
	{
		curNfaVec = nfaStasStack.top();
		nfaStasStack.pop();

		memset(compuFlag, 0, sizeof(compuFlag));
		for (size_t nCurChar = 0; nCurChar < DFACOLSIZE; ++nCurChar)
		{
			STATEID curGroup = m_pGroup[nCurChar];
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
					//if (ssh.size() > std::numeric_limits<STATEID>::max())
					//{
					//	std::cerr << "Fatal Error!" << std::endl;
					//	return (size_t)-1;
					//}
					m_pDfa->push_back(CDfaRow(m_nColNum));
					//std::cout << m_pDfa->size() << std::endl;//���ڲ���
					if (m_pDfa->size() > SC_STATELIMIT)// || nTotalSize >= 2048
					{

						std::cout << "SC_STATELIMIT!" << std::endl;
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
						if(combine)
						{
							termStasVec.push_back(std::make_pair(nextNfaVec, nextSta));
						}
						else
						{
							m_TermSet->push_back(TERMSET());
							m_TermSet->back().dfaSta = nextSta;
							m_TermSet->back().dfaId = m_nId;
							nTotalSize += sizeof(TERMSET);
						}
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
							m_TermSet->push_back(TERMSET());
							m_TermSet->back().dfaSta = termStasVec[j].second;
							m_TermSet->back().dfaId = nfalog[i].dfaId;
							nTotalSize += sizeof(TERMSET);
							break;
						}
					}
				}
			}
		}
		if (nTotalSize >= 2048)
		{
			//return (size_t)-1;
		}
	}
	m_pDfa->shrink_to_fit();
	return 0;
}

DFANEWSC  void CDfaNew:: printTerms()
{
	for(std::vector<TERMSET>::iterator iter = m_TermSet->begin(); iter != m_TermSet->end(); ++iter)
	{
		std::cout << (size_t)iter->dfaSta <<"  :  " << "dfa " << iter->dfaId << std::endl;
	}
}

void PrintMatrix(BYTE *pMat, size_t nWidth, size_t nHeight)
{
	for (size_t i = 0; i < nHeight; ++i)
	{
		for (size_t j = 0; j < nHeight; ++j)
		{
			std::cout << (size_t)pMat[i * nWidth + j] << " ";
		}
		std::cout << std::endl;
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

	size_t nMatHeight = nSize + 1;
	size_t nMatWidth = nMatHeight;

	if (nMatWidth % 16 != 0)
	{
		nMatWidth = (nMatWidth / 16) * 16 + 16;
	}
	BYTE *pMat = (BYTE*)_aligned_malloc(nMatWidth * nMatHeight, 128);
	memset(pMat, 0, nMatWidth * nMatHeight);

	//pMat[0] = 1;
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
	std::vector<STATEID> reachable;

	size_t nStartRow = m_StartId * nMatWidth;
	pMat[nStartRow + m_StartId] = 1;
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

	//��������״̬���ұ�
	nSize = m_pDfa->size();
	std::vector<STATEID> *pRevTab = new std::vector<STATEID>[nSize * nCols];
	for (STATEID i = 0; i < nSize; ++i)
	{
		for (STATEID j = 0; j < nCols; ++j)
		{
			STATEID nDest = (STATEID)(*m_pDfa)[i][j];
			if (nDest != STATEID(-1))
			{
				pRevTab[nDest * nCols + j].push_back(STATEID(i));
			}
		}
	}
	std::vector<PARTSET> partSet;
	//divide nondistinguishable states
	if (0 != PartitionNonDisState(pRevTab, partSet))
	{
		return -1;
	}

	if (partSet.size() < nSize)
	{
		//DFA minization
		MergeNonDisStates(partSet);
	}

	delete []pRevTab;
	return 0;
}

DFANEWSC WORD CDfaNew::GetGroupCount() const
{
	return m_nColNum;
}

DFANEWSC BYTE CDfaNew::Char2Group(BYTE nIdx)
{
	return m_pGroup[nIdx];
}

DFANEWSC const BYTE* CDfaNew::GetGroup() const
{
	return m_pGroup;
}

DFANEWSC const BYTE CDfaNew::GetOneGroup(STATEID charNum) const
{
	return m_pGroup[charNum];
}

DFANEWSC STATEID CDfaNew::GetStartId() const
{
	return m_StartId;
}

DFANEWSC void CDfaNew::SetStartId(size_t id)
{
	m_StartId = id;
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
	STATEID ActiveState = m_StartId;
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

DFANEWSC void CDfaNew::GetAcceptedId(STATEID id, CVectorNumber &dfaIds)
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
		for (size_t j = 0; j < m_nColNum; ++j)
		{
			WriteNum(beg, (*m_pDfa)[i][j], sizeof(BYTE));
		}
	}
	//дDFA�Ŀ�ʼ״̬���
	WriteNum(beg, m_StartId, sizeof(BYTE));
	//дDFA����̬��DFAId�Ķ�Ӧ��ϵ
	WriteNum(beg, m_TermSet->size());
	for (size_t i = 0; i < m_TermSet->size(); ++i)
	{
		WriteNum(beg, (*m_TermSet)[i].dfaSta, sizeof(BYTE));
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

DFANEWSC void CDfaNew::Load(BYTE *beg, size_t len)
{
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
		for (size_t j = 0; j < m_nColNum; ++j)
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
	m_StartId = 0;
	ReadNum(beg, m_StartId, sizeof(BYTE));
	//��DFA����̬��DFAId�Ķ�Ӧ��ϵ
	size_t TermSetSize;
	ReadNum(beg, TermSetSize);
	m_TermSet->resize(TermSetSize);
	for (size_t i = 0; i < TermSetSize; ++i)
	{
		(*m_TermSet)[i].dfaSta = 0;
		ReadNum(beg, (*m_TermSet)[i].dfaSta, sizeof(BYTE));
		ReadNum(beg, (*m_TermSet)[i].dfaId);
	}
}

//reachable�б������еĿɴ�״̬��reachable��Ԫ�ص�ȡֵ��3����
//0 ��ʾ��״̬Ϊ����״̬��1 ��ʾ��״̬Ϊ���ɴ�״̬���ߡ�����״̬��2 ��ʾ��״̬Ϊ�ɴ�״̬
void CDfaNew::MergeReachable(std::vector<STATEID> &reachable)
{
	size_t nDfaSize = m_pDfa->size();
	//�����̬��dfaId���Ա�֤��̬��Ÿ��ĺ����Ӧ��dfaId���ֲ���
	std::vector<size_t> termFlag(nDfaSize, size_t(-1));
	for (size_t i = 0; i < m_TermSet->size(); ++i)
	{
		TERMSET &curSet = (*m_TermSet)[i];
		termFlag[curSet.dfaSta] = curSet.dfaId;
	}
	m_TermSet->clear();
	
	//ͳ�ƿɴ�״̬����Ŀ
	size_t nRcbCnt = reachable.size();
	size_t nColNum = GetGroupCount();

	//����һ��ͬ��Ա����m_pDfa������ͬ�ı��������ڴ洢ɾ������״̬���DFA��ת��
	std::vector<CDfaRow> *pNewDfa = new std::vector<CDfaRow>(nRcbCnt, CDfaRow(nColNum));
	std::vector<CDfaRow> &tmpDfa = *pNewDfa;

	size_t nMemSize = nDfaSize * sizeof(STATEID);
	STATEID *pOldToNew = (STATEID*)VirtualAlloc(NULL, nMemSize, MEM_COMMIT, PAGE_READWRITE);
	memset(pOldToNew, 0xFF, nMemSize);

	//��m_pDfa�б�Ŷ�ӦΪ�ɴ�״̬�ĸ��Ƶ�tmpDfa�У����޸�reachable�пɴ�״̬�ı��
	for (std::vector<STATEID>::iterator iter = reachable.begin(); iter != reachable.end(); ++iter)
	{
		STATEID nOldId = *iter, nNewId = iter - reachable.begin();
		CDfaRow &oldRow = (*m_pDfa)[nOldId];
		tmpDfa[nNewId] = oldRow;
		pOldToNew[nOldId] = nNewId;

		//�����µ���̬���
		if (oldRow.GetFlag() & oldRow.TERMINAL)
		{
			TERMSET tmpSta;
			tmpSta.dfaSta = nNewId;
			tmpSta.dfaId = termFlag[nOldId];
			m_TermSet->push_back(tmpSta);
		}
	}

	//��tmpDfa�е�״̬����޸�Ϊ��Ӧ���±��
	for (STATEID i = 0; i < nRcbCnt; ++i)
	{
		CDfaRow &curRow = tmpDfa[i];
		for (STATEID j = 0; j < nColNum; ++j)
		{
			STATEID &cur = curRow[j];
			if (cur != STATEID(-1))
			{
				cur = pOldToNew[cur];
			}
		}
	}
	VirtualFree(pOldToNew, 0, MEM_RELEASE);

	//�滻m_pDfa
	delete m_pDfa;
	m_pDfa = pNewDfa;
}

void ReleaseAbleTo(PARTSET &ps)
{
	if (!ps.AbleTo.empty())
	{
		VirtualFree(ps.AbleTo.front(), 0, MEM_RELEASE);
	}
	ps.AbleTo.clear();
	ps.Ones.clear();
}

void CalcAbleTo(std::vector<STATEID> *pRevTbl, size_t nGrpNum, size_t nStaNum, PARTSET &ps)
{
	//���AbleTo
	ReleaseAbleTo(ps);

	BYTE *pBuf = (BYTE*)VirtualAlloc(NULL, nStaNum * nGrpNum, MEM_COMMIT, PAGE_READWRITE);
	ps.AbleTo.resize(nGrpNum);
	ps.Ones.resize(nGrpNum, 0);
	//����AbleTo��ֵ��ÿ����һ���µĻ��߸���PARTSET�������һ��
	for (size_t j = 0; j < nGrpNum; ++j)
	{
		BYTE *pAbleTo = pBuf + j * nStaNum;
		//����PARTSET�е�ÿ��״̬t�������ڦ�(-1)(t,j)�٦���AbleTo[t]���Ϊ1
		for (std::list<STATEID>::iterator k = ps.StaSet.begin(); k != ps.StaSet.end(); ++k)
		{
			BOOL br = !(pRevTbl[*k * nGrpNum + j].empty());
			if (br == TRUE && pAbleTo[*k] == 0)
			{
				pAbleTo[*k] = br;
				++ps.Ones[j];
			}
		}
		ps.AbleTo[j] = pAbleTo;
	}
}

void CDfaNew::InitPartSet(std::vector<PARTSET> &partSet) const
{
	size_t nGrpNum = GetGroupCount();
	size_t nStaNum = m_pDfa->size();

	partSet.clear();

	//�����������ڲ�ͬDFA����̬����
	std::set<size_t> *pTerm2Dfa = new std::set<size_t>[nStaNum];

	for (STATEID i = 0; i < m_TermSet->size(); ++i)
	{
		TERMSET &ts = (*m_TermSet)[i];
		if (ts.dfaSta >= nStaNum)
		{
			std::cout << "ts.dfaSta >= nStaNum" << std::endl;
			system("pause");
		}
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

inline size_t CountOnes(BYTE *pBuf, size_t nBufSize)
{
	return std::count(pBuf, pBuf + nBufSize, 1);
}

size_t CDfaNew::PartitionNonDisState(std::vector<STATEID> *pRevTbl, std::vector<PARTSET> &partSet) const
{
	size_t nGrpNum = GetGroupCount();
	size_t nStaNum = m_pDfa->size();

	InitPartSet(partSet);

	for (std::vector<PARTSET>::iterator i = partSet.begin(); i != partSet.end(); ++i)
	{
		//����partSet��ÿ�����ϣ����ݲ�ͬ��nGrpNum���㲻ͬAbleTo��AbleTo��Ӧ�����е�a(i)
		CalcAbleTo(pRevTbl, nGrpNum, nStaNum, *i);
	}

	//pWait��ʾ�����з�partSet��ÿ�����ϵļ����±꣬
	//���ڲ�ͬ�������ַ��ᱣ������ͬ�����±ꡣ��Ӧ������L(a)
	//std::vector<size_t> *pWait = new std::vector<size_t>[nGrpNum];
	std::vector<size_t> pWait[256];

	//��ʼ��pWait��Ҫ��������0<|ij|<=|ik|����j����pWait����֮������k
	//��Ӧ�����г�ʼ��L(a)���̣������i��Ӧa
	for (size_t i = 0; i < nGrpNum; ++i)
	{
		size_t nMinId = 0;
		size_t nMinCnt = partSet[nMinId].Ones[i];
		for (size_t j = 1; j < partSet.size(); ++j)
		{
			size_t nCurCnt = partSet[j].Ones[i];
			if (nCurCnt != 0)
			{
				if (nMinCnt == 0 || nCurCnt < nMinCnt)
				{
					nMinId = j;
					nMinCnt = nCurCnt;
				}
			}
		}
		if (nMinCnt != 0)
		{
			pWait[i].push_back(nMinId);
		}
	}

	//����ܹ�����ĳһ�����ַ����ܹ�����ISet�е�״̬��״̬����
	BYTE *pAbleToI = (BYTE*)VirtualAlloc(NULL, nStaNum, MEM_COMMIT, PAGE_READWRITE);
	for (; ; )
	{
		BYTE byCurGrp = -1;
		size_t nCurSet = -1;
		//��pWait��ȡһ��ֵ��remove��ֵ
		for (size_t i = 0; i < nGrpNum; ++i)
		{
			if (i >= 256)
			{
				std::cout << "i >= 256" << std::endl;
				system("pause");
			}
			if (!pWait[i].empty())
			{
				byCurGrp = i;
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

		if (nCurSet >= partSet.size())
		{
			std::cout << "nCurSet >= partSet.size()" << std::endl;
			system("pause");
		}
		PARTSET *pISet = &partSet[nCurSet];
		ZeroMemory(pAbleToI, nStaNum);
		size_t nRevCnt = 0;
		//��ʼ��pAbleToI
		for (std::list<STATEID>::iterator iSta = pISet->StaSet.begin();
			iSta != pISet->StaSet.end(); ++iSta)
		{
			std::vector<STATEID> &revI = pRevTbl[*iSta * nGrpNum + byCurGrp];
			for (std::vector<STATEID>::iterator iRevSta = revI.begin();
				iRevSta != revI.end(); ++iRevSta)
			{
				if (*iRevSta >= nStaNum)
				{
					std::cout << "*iRevSta >= nStaNum" << std::endl;
					system("pause");
				}
				pAbleToI[*iRevSta] = 1;
				++nRevCnt;
			}
		}

		if (nRevCnt != 0)
		{
			partSet.reserve(1000);
			for (size_t j = 0; j != partSet.size(); ++j)
			{
				//ȡpartSet�е�һ������j��B(j)�д���t����������(t,a)��a(i)��
				//B��(j)Ϊt�ļ��ϣ�B��(j)ΪB(j)-B��(j)
				PARTSET *pJSet = &partSet[j];
				std::list<STATEID>::iterator t = pJSet->StaSet.begin();
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
					if (*t >= nStaNum)
					{
						std::cout << "*t >= nStaNum" << std::endl;
						system("pause");
					}
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

				std::list<STATEID>::iterator part = pJSet->StaSet.begin();
				//���Ҳ���0,1�ֶε��зֵ㣬��Ϊpart
				for (; part != pJSet->StaSet.end() && pAbleToI[*part] != 0; ++part);
				if (partSet.size() > 254)
				{
					VirtualFree(pAbleToI, nStaNum, MEM_RELEASE);
					for (std::vector<PARTSET>::iterator i = partSet.begin(); i != partSet.end(); ++i)
					{
						ReleaseAbleTo(*i);
					}
					//delete []pWait;
					return size_t(-1);
				}
				//����������µĻ���
				PARTSET &lastPart = partSet.back();
				lastPart.StaSet.splice(lastPart.StaSet.begin(),
					pJSet->StaSet, part, pJSet->StaSet.end());
				CalcAbleTo(pRevTbl, nGrpNum, nStaNum, *pJSet);
				CalcAbleTo(pRevTbl, nGrpNum, nStaNum, lastPart);

				//����pWait
				for (BYTE m = 0; m < nGrpNum; ++m)
				{
					int k = partSet.size() - 1;
					std::vector<size_t> &curWait = pWait[m];
					//int aj = CountOnes(pJSet->AbleTo[m], nStaNum);
					//int ak = CountOnes(lastPart.AbleTo[m], nStaNum);
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
	VirtualFree(pAbleToI, nStaNum, MEM_RELEASE);
	for (std::vector<PARTSET>::iterator i = partSet.begin(); i != partSet.end(); ++i)
	{
		ReleaseAbleTo(*i);
	}
	//delete []pWait;
	return 0;
}

//Partition�е�Ԫ��Ϊһ��״̬�ļ��ϣ�������Ԫ��Ϊ����ȼ�״̬��ÿ�����Ͽ��Ժϲ�Ϊ�µ�DFA��һ��״̬
void CDfaNew::MergeNonDisStates(std::vector<PARTSET> &partSet)
{
	std::vector<STATEID> sta2Part(m_pDfa->size());
	
	STATEID nCol = (STATEID)GetGroupCount();

	//�����̬��dfaId���Ա�֤��̬��Ÿ��ĺ����Ӧ��dfaId���ֲ���
	std::vector<size_t> *termFlag = new std::vector<size_t>[m_pDfa->size()];
	for (size_t i = 0; i < m_TermSet->size(); ++i)
	{
		termFlag[(*m_TermSet)[i].dfaSta].push_back((*m_TermSet)[i].dfaId);
	}
	m_TermSet->clear();

	//����һ��ͬCDfaNew�г�Ա����m_pDfa������ͬ�ı��������ڴ洢�ϲ����DFA��ת��
	std::vector<CDfaRow> *pNewDfa = new std::vector<CDfaRow>(
		(STATEID)partSet.size(), CDfaRow(nCol));
	std::vector<CDfaRow> &tmpDfa = *pNewDfa;

	//�ȼ۵�״̬����ͬһ��partition�У����ԭ����״̬������һ���µ�partition�У����޸��µ���ʼ״̬���
	STATEID nSetIdx = 0;
	for (std::vector<PARTSET>::iterator iPart = partSet.begin(); iPart != partSet.end(); ++iPart)
	{
		for (std::list<STATEID>::iterator iSta = iPart->StaSet.begin(); iSta != iPart->StaSet.end(); ++iSta)
		{
			CDfaRow &curRow = (*m_pDfa)[*iSta];
			sta2Part[*iSta] = nSetIdx;
			//�޸��µ���ʼ״̬
			if (curRow.GetFlag() & CDfaRow::START)
			{
				m_StartId = nSetIdx;
			}

			//�����µ���̬���
			if (curRow.GetFlag() & CDfaRow::TERMINAL)
			{
				TERMSET tmpSta;
				tmpSta.dfaSta = nSetIdx;
				for (std::vector<size_t>::iterator i = termFlag[*iSta].begin(); i != termFlag[*iSta].end(); ++i)
				{
					tmpSta.dfaId = *i;
					m_TermSet->push_back(tmpSta);
				}
			}
		}
		++nSetIdx;
	}

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

	UniqueTermSet();

	//�滻m_pDfa
	delete m_pDfa;
	m_pDfa = pNewDfa;
}
