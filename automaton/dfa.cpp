/**
**  @file        dfa.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CDllArray, CDllString
**
*/

#include "stdafx.h"

#include <hwprj\dfa.h>
#include <hwprj\nfa.h>
#include "dfaalgo.h"

DFAHDR CDfa::CDfa()
	: m_nId(ULONG(-1)), m_nColNum(ULONG(0)), m_nStartId(STATEID(0))
{
	std::fill(m_pGroup, m_pGroup + SC_DFACOLCNT, BYTE(-1));
	m_pDfa = new std::vector<CDfaRow>;
}

DFAHDR CDfa::~CDfa()
{
	delete m_pDfa;
}

DFAHDR CDfa::CDfa(const CDfa &other)
{
	m_pDfa = new std::vector<CDfaRow>;
	*this = other;
}

DFAHDR CDfa& CDfa::operator=(const CDfa &other)
{
	Clear();
	m_nId = other.m_nId;
	m_nColNum = other.m_nColNum;
	m_nStartId = other.m_nStartId;
	CopyMemory(m_pGroup, other.m_pGroup, SC_DFACOLCNT * sizeof(BYTE));
	*m_pDfa = *other.m_pDfa;
	m_FinStas = other.m_FinStas;
	return *this;
}

DFAHDR ULONG CDfa::Size() const
{
	return m_pDfa->size();
}

DFAHDR CDfaRow& CDfa::Back()
{
	return m_pDfa->back();
}

DFAHDR void CDfa::Reserve(ULONG nSize)
{
	m_pDfa->reserve(nSize);
}

DFAHDR void CDfa::Resize(ULONG nSize, ULONG nCol)
{
	m_pDfa->resize(nSize, CDfaRow(nCol));
}

DFAHDR CDfaRow& CDfa::operator[](STATEID index)
{
	return (*m_pDfa)[index];
}

DFAHDR const CDfaRow& CDfa::operator[](STATEID index) const
{
	return (*m_pDfa)[index];
}

DFAHDR void CDfa::PushBack(CDfaRow &sta)
{
	m_pDfa->push_back(sta);
}

DFAHDR void CDfa::Init(BYTE *pGroup)
{
	//1 if the group is used, 0 otherwise
	BYTE occurred[SC_DFACOLCNT] = {0};
	for (ULONG i = 0; i < SC_DFACOLCNT; ++i) 
	{
		occurred[pGroup[i]] = 1;
	}

	//number of groups
	WORD nZeroBegPos = 0;
	for (; nZeroBegPos < SC_DFACOLCNT; ++nZeroBegPos)
	{
		if (occurred[nZeroBegPos] == 0)
		{
			break;
		}
	}

	//flag is true if 1 does not occur after nZeroBegPos, false otherwise
	//flag is true indicates that the group is reasonable
	bool flag = true;
	for (ULONG i = nZeroBegPos + 1; i < SC_DFACOLCNT; ++i)
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
		CopyMemory(m_pGroup, pGroup, SC_DFACOLCNT * sizeof(BYTE));
	}
	else
	{
		std::cerr << "Group error!" << std::endl;
	}
}

DFAHDR void CDfa::Clear()
{
	m_nId = ULONG(-1);
	m_nColNum = ULONG(0);
	m_nStartId = STATEID(0);
	memset(m_pGroup, 0xFF, SC_DFACOLCNT);
	m_pDfa->clear();
	m_FinStas.Clear();
}

DFAHDR ULONG CDfa::FromNFA(const CNfa &nfa)
{
	typedef std::unordered_map<std::vector<ULONG>, STATEID, NSTATESET_HASH> STATESETHASH;
	//std::vector<std::pair<std::vector<ULONG>, STATEID>> termStasVec;

	ULONG dfaId = m_nId;
	Clear();
	m_nId = dfaId;
	
	std::vector<std::vector<ULONG>> eClosure;
	NfaEClosure(nfa, eClosure);

	BYTE groups[SC_DFACOLCNT];
	NAvaiEdges(nfa, groups);
	Init(groups);

	std::stack<std::vector<ULONG>> nfaStasStack;
	std::vector<ULONG> &startEVec = eClosure.front();
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

	std::vector<ULONG> curNfaVec;
	static std::vector<ULONG> nextNfaVec; //
	nextNfaVec.clear();
	BYTE compuFlag[SC_CHARSETSIZE];

	for (; nfaStasStack.size() > 0; )
	{
		curNfaVec = nfaStasStack.top();
		nfaStasStack.pop();

		memset(compuFlag, 0, sizeof(compuFlag));
		for (ULONG nCurChar = 0; nCurChar < SC_DFACOLCNT; ++nCurChar)
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
						return (ULONG)-1;
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

DFAHDR ULONG CDfa::Minimize()
{
	//error: DFA is empty

	//CTimer time1;//���ڲ���
	ULONG nSize = m_pDfa->size();
	ULONG nCols = GetGroupCount();
	if (nSize == 0)
	{
		return ULONG(-1);
	}
#ifdef DFA_REMOVE_UNREACHABLE
	ULONG nMatHeight = nSize + 1;
	ULONG nMatWidth = nMatHeight;

	if (nMatWidth % 16 != 0)
	{
		nMatWidth = (nMatWidth / 16) * 16 + 16;
	}
	BYTE *pMat = NULL;
	STATEVEC reachable;

	pMat = (BYTE*)_aligned_malloc(nMatWidth * nMatHeight, 128);
	memset(pMat, 0, nMatWidth * nMatHeight);

	for (ULONG i = 0; i < nSize; ++i)
	{
		for (ULONG j = 0; j < nCols; ++j)
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

	ULONG nStartRow = m_nStartId * nMatWidth;
	pMat[nStartRow + m_nStartId] = 1;
	for (ULONG i = 0; i < nSize; ++i)
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
	ULONG nr = PartitionNonDisState(pRevTab, partSet);
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

DFAHDR WORD CDfa::GetGroupCount() const
{
	return m_nColNum;
}

DFAHDR BYTE CDfa::Char2Group(BYTE nIdx)
{
	return m_pGroup[nIdx];
}

DFAHDR const BYTE CDfa::GetGroup(STATEID charNum) const
{
	return m_pGroup[charNum];
}

DFAHDR STATEID CDfa::GetStartId() const
{
	return m_nStartId;
}

DFAHDR void CDfa::SetStartId(STATEID id)
{
	m_nStartId = id;
}

DFAHDR void CDfa::SetId(ULONG id)
{
	m_nId = id;
}

DFAHDR ULONG CDfa::GetId()
{
	return m_nId;
}

DFAHDR ULONG CDfa::Process(BYTE *ByteStream, ULONG len, CStateSet &StaSet)
{
	std::vector<bool> res(m_pDfa->size(), false);
	STATEID ActiveState = m_nStartId;
	ULONG nPos = 0;
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
	for (ULONG i = 0; i < res.size(); ++i)
	{
		if (res[i])
		{
			StaSet.PushBack(i);
		}
	}

	return nPos;
}

DFAHDR ULONG CDfa::Save(BYTE *beg)
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
	for (ULONG i = 0; i < SC_DFACOLCNT; ++i)
	{
		WriteNum(beg, m_pGroup[i]);
	}

	//write dfa table
	for (ULONG i = 0; i < m_pDfa->size(); ++i)
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
	for (ULONG i = 0; i < m_FinStas.Size(); ++i)
	{
		STATEID nStaId = m_FinStas[i];
		CDllArray ids;
		m_FinStas.GetDfaIds(nStaId, ids);
		for (ULONG j = 0; j < ids.Size(); ++j)
		{
			WriteNum(beg, nStaId, sizeof(BYTE));
			WriteNum(beg, ids[j]);
		}
	}

	return beg - pOld;
}

DFAHDR void CDfa::Load(BYTE *beg, ULONG len)
{
	len = 0;
	//read dfa id
	ULONG dfaId;
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
	BYTE pGroup[SC_DFACOLCNT];
	for (ULONG i = 0; i < SC_DFACOLCNT; ++i)
	{
		ReadNum(beg, pGroup[i]);
	}
	Init(pGroup);
	m_nId = dfaId;

	//read dfa table
	m_pDfa->resize(dfaSize, CDfaRow(m_nColNum));
	ULONG nFlag;
	for (ULONG i = 0; i < m_pDfa->size(); ++i)
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
	ULONG nFinStasSize;
	ReadNum(beg, nFinStasSize);
	for (ULONG i = 0; i < nFinStasSize; ++i)
	{
		STATEID nStaId = 0;
		ULONG nDfaId;
		ReadNum(beg, nStaId, sizeof(BYTE));
		ReadNum(beg, nDfaId);
		m_FinStas.PushBack(nStaId, nDfaId);
	}
}

DFAHDR void CDfa::Dump(const char *pFile)
{
	std::ofstream fout(pFile);
	if(!fout)
	{
		std::cout << "file open error!" << std::endl;
	}

	fout << "�ַ������Ӧ��ϵ��" << std::endl;
	for(BYTE i = 0; i < SC_DFACOLCNT - 1; ++i)
	{
		fout << (ULONG)i << "\t" << (ULONG)m_pGroup[i] << std::endl;
	}

	fout << 255 << "\t" << (ULONG)m_pGroup[255] << std::endl;
	fout << "\t";
	for(BYTE j = 0; j != m_nColNum; ++j)
	{
		fout << (ULONG)j << "\t";
	}
	fout << std::endl;
	for(ULONG i = 0; i != m_pDfa->size(); ++i)
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
			fout << (ULONG)(*m_pDfa)[i][j] << "\t";
		}
		fout << std::endl;
	}
	fout << std::endl;
	fout.close();
}

//reachable�б������еĿɴ�״̬��reachable��Ԫ�ص�ȡֵ��3����
//0 ��ʾ��״̬Ϊ����״̬��1 ��ʾ��״̬Ϊ���ɴ�״̬���ߡ�����״̬��2 ��ʾ��״̬Ϊ�ɴ�״̬
void CDfa::MergeReachable(STATEVEC &reachable)
{
	ULONG nDfaSize = m_pDfa->size();
	//�����̬��dfaId���Ա�֤��̬��Ÿ��ĺ����Ӧ��dfaId���ֲ���
	std::vector<ULONG> finFlag(nDfaSize, ULONG(-1));
	//for (ULONG i = 0; i < m_FinStas.Size(); ++i)
	//{
	//	TERMSET &curSet = (*m_pTermSet)[i];
	//	termFlag[curSet.dfaSta] = curSet.dfaId;
	//}
	//m_pTermSet->clear();
	
	//ͳ�ƿɴ�״̬����Ŀ
	ULONG nRcbCnt = reachable.size();
	ULONG nColNum = GetGroupCount();

	//����һ��ͬ��Ա����m_pDfa������ͬ�ı��������ڴ洢ɾ������״̬���DFA��ת��
	std::vector<CDfaRow> *pNewDfa = new std::vector<CDfaRow>(nRcbCnt, CDfaRow(nColNum));
	std::vector<CDfaRow> &tmpDfa = *pNewDfa;

	ULONG nMemSize = nDfaSize * sizeof(STATEID);
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
			//TERMSET tmpSta;
			//tmpSta.dfaSta = nNewId;
			//tmpSta.dfaId = termFlag[nOldId];
			//m_pTermSet->push_back(tmpSta);
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

void CDfa::InitPartSet(std::vector<PARTSET> &partSet) const
{
	ULONG nStaNum = m_pDfa->size();

	partSet.clear();

	//�����������ڲ�ͬDFA����̬����
	std::set<ULONG> *pTerm2Dfa = new std::set<ULONG>[nStaNum];

	for (ULONG i = 0; i < m_FinStas.Size(); ++i)
	{
		STATEID nFinStaId = m_FinStas[i];
		pTerm2Dfa[nFinStaId] = m_FinStas._GetDfaIds(nFinStaId);
	}

	//������̬���Ϻͷ���̬���ϣ�map��firstΪ�գ����ʾ��Ӧ��PARTSETΪ����̬���ϣ���֮��Ϊ��̬����
	//initBSet�м�������
	std::map<std::set<ULONG>, PARTSET> initBSet;
	for (STATEID i = 0; i < nStaNum; ++i)
	{
		std::set<ULONG> &curDfaSet = pTerm2Dfa[i];
		initBSet[curDfaSet].StaSet.push_back(i);
	}
	delete []pTerm2Dfa;

	//������̬�ͷ���̬���ϵ�˳��vector�е�lastΪ����̬����
	for (std::map<std::set<ULONG>, PARTSET>::iterator i = initBSet.begin();
		i != initBSet.end(); ++i)
	{
		if (i->first.size() != 0)
		{
			partSet.push_back(i->second);
		}
	}
	partSet.push_back(initBSet[std::set<ULONG>()]);
}

ULONG CDfa::PartitionNonDisState(STATEVEC *pRevTbl,
									 std::vector<PARTSET> &partSet) const
{
	ULONG nGrpNum = GetGroupCount();
	ULONG nStaNum = m_pDfa->size();

	InitPartSet(partSet);

	for (std::vector<PARTSET>::iterator i = partSet.begin();
		i != partSet.end(); ++i)
	{
		//����partSet��ÿ�����ϣ����ݲ�ͬ��nGrpNum���㲻ͬAbleTo��AbleTo��Ӧ�����е�a(i)
		CalcAbleTo(pRevTbl, nGrpNum, nStaNum, *i);
	}

	//pWait��ʾ�����з�partSet��ÿ�����ϵļ����±꣬
	//���ڲ�ͬ�������ַ��ᱣ������ͬ�����±ꡣ��Ӧ������L(a)
	std::vector<ULONG> pWait[256];

	//��ʼ��pWait��Ҫ��������0<|ij|<=|ik|����j����pWait����֮������k
	//��Ӧ�����г�ʼ��L(a)���̣������i��Ӧa
	for (ULONG i = 0; i < nGrpNum; ++i)
	{
		ULONG AcpSum = 0, NonAcpSum = 0;
		for (std::vector<PARTSET>::iterator j = partSet.begin();
			j != partSet.end() - 1; ++j)
		{
			AcpSum += j->Ones[i];
		}
		NonAcpSum = partSet.back().Ones[i];
		if (AcpSum != 0 && NonAcpSum == 0)
		{
			for (ULONG k = 0; k < partSet.size() - 1; ++k)
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
			for (ULONG k = 0; k < partSet.size() - 1; ++k)
			{
				pWait[i].push_back(k);
			}
		}
	}

	//����ܹ�����ĳһ�����ַ����ܹ�����ISet�е�״̬��״̬����
	BYTE *pAbleToI = new BYTE[nStaNum];
	partSet.reserve(1000);
	ULONG nr = 0;
	for (; nr == 0; )
	{
		BYTE byCurGrp = (BYTE)-1;
		ULONG nCurSet = (ULONG)-1;
		//��pWait��ȡһ��ֵ��remove��ֵ
		for (ULONG i = 0; i < nGrpNum; ++i)
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
		ULONG nRevCnt = 0;
		//��ʼ��pAbleToI
		for (STATELIST_ITER iSta = pISet->StaSet.begin();
			iSta != pISet->StaSet.end(); ++iSta)
		{
			ULONG nRevIdx = *iSta * nGrpNum + byCurGrp;
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
			for (ULONG j = 0; j != partSet.size(); ++j)
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
					nr = ULONG(-1);
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
					std::vector<ULONG> &curWait = pWait[m];
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
void CDfa::MergeNonDisStates(std::vector<PARTSET> &partSet)
{
	STATEVEC sta2Part(m_pDfa->size());
	
	STATEID nCol = (STATEID)GetGroupCount();

	//�����̬��dfaId���Ա�֤��̬��Ÿ��ĺ����Ӧ��dfaId���ֲ���
	std::set<ULONG> *finFlag = new std::set<ULONG>[m_pDfa->size()];
	for (ULONG i = 0; i < m_FinStas.Size(); ++i)
	{
		STATEID nFinStaId = m_FinStas[i];
		finFlag[nFinStaId] = m_FinStas._GetDfaIds(nFinStaId);
	}

	m_FinStas.Clear();

	//����һ��ͬCDfa�г�Ա����m_pDfa������ͬ�ı��������ڴ洢�ϲ����DFA��ת��
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
				for (std::set<ULONG>::iterator i = finFlag[*iSta].begin();
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


	//�滻m_pDfa
	swap(m_pDfa, pNewDfa);
	delete pNewDfa;
}

CFinalStates& CDfa::GetFinalState()
{
	return m_FinStas;
}

const CFinalStates& CDfa::GetFinalState() const
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
**  @param dfas      a vector contains mutiple CDfa
**  @param lastDfa   the merged dfa
**
**  @return bool
**
**  @retval true function successful
**  @retval fasle fatal error
*/

DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName)
{
	std::ofstream fout(fileName);
	fout << "digraph G {" << std::endl;
	fout << "S -> " << (ULONG)newdfa.GetStartId() << std::endl;

	for(STATEID i = 0; i != newdfa.Size(); ++i)
	{
		std::map<STATEID, ULONG> rowStateCnt;
		for(BYTE j = 0; j != newdfa.GetGroupCount(); ++j)
		{
			rowStateCnt[newdfa[i][j]]++;
		}
		STATEID maxId = 0;
		for (std::map<STATEID, ULONG>::iterator j = rowStateCnt.begin(); j != rowStateCnt.end(); ++j)
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
				fout << i << " -> " << (ULONG)newdfa[i][j] << " [label=\"" << j << "\"];" << std::endl;
			}
			else if (maxId != (STATEID)-1)
			{
				fout << i << " -> "  << (ULONG)maxId << " [label=\"" << j << "\"];" << std::endl;
			}
		}
	}
	for (STATEID i = 0; i < newdfa.Size(); ++i)
	{
		if (newdfa[i].GetFlag() & CDfaRow::TERMINAL)
		{
			fout << (ULONG)i << " [peripheries=2];" << std::endl;
		}
	}
	fout << "}" << std::endl;
	fout.close();
}

DFAHDR bool MergeMultipleDfas(std::vector<CDfa> &dfas, CDfa &lastDfa)
{
	ULONG dfaId = lastDfa.GetId();
	lastDfa.Clear();
	lastDfa.SetId(dfaId);
#undef max

	ULONG dfasSize = dfas.size();
	STATEID nTermSta = 1;//nTermSta: terminal flag. 1: terminal, -1: non-terminal

	//group the lastDfa's columns
	BYTE groups[SC_DFACOLCNT];
	DfaColGroup(dfas, groups);
	lastDfa.Init(groups);

	ULONG colCnt = lastDfa.GetGroupCount();

	typedef std::unordered_map<std::vector<STATEID>, STATEID, TODFA_HASH> STATESETHASH;
	STATESETHASH statehash;

	ULONG finFlag = 0;//terminal state flag, 1: terminal state, 0: normal state
	std::stack<std::vector<STATEID>> statesStack;

	/*
	**	use a size of (dfas.size() + 2) vector to represent a state of the merged dfa.
	**	element 0 represents the state of dfas[0], ..., element n represents the state of dfas[n].
	**	element n + 1 and element n + 2 are flags which show that whether this state is a start state or terminal state.
	*/
	std::vector<STATEID> startVec(dfasSize + 2);
	
	lastDfa.Reserve(SC_CHARSETSIZE);
	lastDfa.Resize(lastDfa.Size() + 1, colCnt);

	for(ULONG i = 0; i < dfasSize; ++i)
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
	BYTE computFlag[SC_CHARSETSIZE];

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
		for(ULONG curChar = 0; curChar < SC_DFACOLCNT; ++curChar)
		{
			finFlag = 0;
			ZeroMemory(NextVec.data(), NextVec.size() * sizeof(ULONG));
			BYTE lastDfaGroup = groups[curChar];
			if(computFlag[lastDfaGroup] == 1)
			{
				continue;
			}
			computFlag[lastDfaGroup] = 1;

			ULONG flag = 0;//flag = 0: empty next state
			for(ULONG i = 0; i < dfasSize; ++i)
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
	if(lastDfa.Size() > SC_MAXDFASIZE)
	{
		//std::cerr << "SC_MAXDFASIZE!" << std::endl;
		return false;
	}

	return true;
}

