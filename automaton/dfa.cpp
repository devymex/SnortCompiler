/**
**	@file		dfa.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		 Common classes declaration
**
**	Include CUnsignedArray, CDllString
**
*/

#include "stdafx.h"

#include "dfaalgo.h"
#include <hwprj\dfarow.h>
#include <hwprj\nfa.h>
#include <hwprj\dfa.h>

typedef DFAROWARY::iterator			DFAROWARY_ITER;
typedef DFAROWARY::const_iterator	DFAROWARY_CITER;

DFAHDR CDfa::CDfa()
	: m_nId(ulong(-1)), m_nColNum(ulong(0)), m_nStartId(STATEID(0))
{
	std::fill(m_pGroup, m_pGroup + SC_DFACOLCNT, byte(-1));
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
	CopyMemory(m_pGroup, other.m_pGroup, SC_DFACOLCNT * sizeof(byte));
	*m_pDfa = *other.m_pDfa;
	m_FinStas = other.m_FinStas;
	return *this;
}

DFAHDR ulong CDfa::Size() const
{
	return m_pDfa->size();
}

DFAHDR CDfaRow& CDfa::Back()
{
	return m_pDfa->back();
}

DFAHDR void CDfa::Reserve(ulong nSize)
{
	m_pDfa->reserve(nSize);
}

DFAHDR void CDfa::Resize(ulong nSize, ulong nCol)
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

DFAHDR void CDfa::SetGroups(byte *pGroup)
{
	//1 if the group is used, 0 otherwise
	byte occurred[SC_DFACOLCNT] = {0};
	for (ulong i = 0; i < SC_DFACOLCNT; ++i) 
	{
		occurred[pGroup[i]] = 1;
	}

	//number of groups
	ushort nZeroBegPos = 0;
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
	for (ulong i = nZeroBegPos + 1; i < SC_DFACOLCNT; ++i)
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
		CopyMemory(m_pGroup, pGroup, SC_DFACOLCNT * sizeof(byte));
	}
	else
	{
		throw 0;
	}
}

DFAHDR void CDfa::Clear()
{
	m_nId = ulong(-1);
	m_nColNum = ulong(0);
	m_nStartId = STATEID(0);
	memset(m_pGroup, 0xFF, SC_DFACOLCNT);
	m_pDfa->clear();
	m_FinStas.Clear();
}

DFAHDR ulong CDfa::FromNFA(const CNfa &nfa)
{
	typedef std::unordered_map<STATEVEC, STATEID, NSTATESET_HASH> STATESETHASH;
	//std::vector<std::pair<std::vector<ulong>, STATEID>> termStasVec;

	ulong dfaId = m_nId;
	Clear();
	m_nId = dfaId;
	
	std::vector<STATEVEC> eClosure;
	NfaEClosure(nfa, eClosure);

	byte groups[SC_DFACOLCNT];
	NAvaiEdges(nfa, groups);
	SetGroups(groups);

	std::stack<STATEVEC> nfaStasStack;
	STATEVEC &startEVec = eClosure.front();
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

	static STATEVEC nextNfaVec; //
	nextNfaVec.clear();
	byte compuFlag[SC_CHARSETSIZE];

	for (; nfaStasStack.size() > 0; )
	{
		STATEVEC curNfaVec = nfaStasStack.top();
		nfaStasStack.pop();

		memset(compuFlag, 0, sizeof(compuFlag));
		for (ulong nCurChar = 0; nCurChar < SC_DFACOLCNT; ++nCurChar)
		{
			byte curGroup = m_pGroup[nCurChar];
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
						return (ulong)-1;
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

DFAHDR ulong CDfa::Minimize()
{
	//error: DFA is empty

	//CTimer time1;//���ڲ���
	ulong nSize = m_pDfa->size();
	ulong nCols = GetGroupCount();
	if (nSize == 0)
	{
		return ulong(-1);
	}
#ifdef DFA_REMOVE_UNREACHABLE
	ulong nMatHeight = nSize + 1;
	ulong nMatWidth = nMatHeight;

	if (nMatWidth % 16 != 0)
	{
		nMatWidth = (nMatWidth / 16) * 16 + 16;
	}
	byte *pMat = NULL;
	STATEVEC reachable;

	pMat = (byte*)_aligned_malloc(nMatWidth * nMatHeight, 128);
	memset(pMat, 0, nMatWidth * nMatHeight);

	for (ulong i = 0; i < nSize; ++i)
	{
		for (ulong j = 0; j < nCols; ++j)
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

	ulong nStartRow = m_nStartId * nMatWidth;
	pMat[nStartRow + m_nStartId] = 1;
	for (ulong i = 0; i < nSize; ++i)
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
		for (byte j = 0; j < nCols; ++j)
		{
			STATEID nDest = (STATEID)(*m_pDfa)[i][j];
			if (nDest != STATEID(-1))
			{
				pRevTab[nDest * nCols + j].push_back(STATEID(i));
			}
		}
	}

	std::vector<PARTSET> partSet;
	ulong nr = PartitionNonDisState(pRevTab);
	//divide nondistinguishable states

	delete []pRevTab;

	return nr;
}

DFAHDR ushort CDfa::GetGroupCount() const
{
	return m_nColNum;
}

DFAHDR byte CDfa::Char2Group(byte nIdx)
{
	return m_pGroup[nIdx];
}

DFAHDR STATEID CDfa::GetStartId() const
{
	return m_nStartId;
}

DFAHDR void CDfa::SetStartId(STATEID id)
{
	m_nStartId = id;
}

DFAHDR void CDfa::SetId(ulong id)
{
	m_nId = id;
}

DFAHDR ulong CDfa::GetId()
{
	return m_nId;
}

DFAHDR ulong CDfa::Process(byte *ByteStream, ulong len, STATEARY &StaSet)
{
	std::vector<bool> res(m_pDfa->size(), false);
	STATEID ActiveState = m_nStartId;
	ulong nPos = 0;
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
	for (ulong i = 0; i < res.size(); ++i)
	{
		if (res[i])
		{
			StaSet.PushBack(i);
		}
	}

	return nPos;
}

DFAHDR ulong CDfa::Save(byte *beg)
{
	byte *pOld = beg;

	//write dfa id
	WriteNum(beg, m_nId);

	//write number of dfa states
	WriteNum(beg, m_pDfa->size(), sizeof(byte));
	if (m_pDfa->size() == 0)
	{
		return beg - pOld;
	}

	//write group
	for (ulong i = 0; i < SC_DFACOLCNT; ++i)
	{
		WriteNum(beg, m_pGroup[i]);
	}

	//write dfa table
	for (ulong i = 0; i < m_pDfa->size(); ++i)
	{
		//write the flag of dfa state
		WriteNum(beg, (*m_pDfa)[i].GetFlag());
		for (byte j = 0; j < m_nColNum; ++j)
		{
			WriteNum(beg, (*m_pDfa)[i][j], sizeof(byte));
		}
	}
	
	//write the dfa's start state id
	WriteNum(beg, m_nStartId, sizeof(byte));

	//write the relationship between dfa's terminal state and dfa id
	WriteNum(beg, m_FinStas.GetAllDfaIdCount());
	for (ulong i = 0; i < m_FinStas.Size(); ++i)
	{
		STATEID nStaId = m_FinStas[i];
		CUnsignedArray ids;
		m_FinStas.GetDfaIdSet(nStaId).CopyTo(ids);
		for (ulong j = 0; j < ids.Size(); ++j)
		{
			WriteNum(beg, nStaId, sizeof(byte));
			WriteNum(beg, ids[j]);
		}
	}

	return beg - pOld;
}

DFAHDR void CDfa::Load(byte *beg, ulong len)
{
	len = 0;
	//read dfa id
	ulong dfaId;
	ReadNum(beg, dfaId);
	m_nId = dfaId;

	//read number of dfa states
	byte dfaSize;
	ReadNum(beg, dfaSize);
	if (dfaSize == 0)
	{
		return;
	}
	
	//read group
	byte pGroup[SC_DFACOLCNT];
	for (ulong i = 0; i < SC_DFACOLCNT; ++i)
	{
		ReadNum(beg, pGroup[i]);
	}
	SetGroups(pGroup);
	m_nId = dfaId;

	//read dfa table
	m_pDfa->resize(dfaSize, CDfaRow(m_nColNum));
	ulong nFlag;
	for (ulong i = 0; i < m_pDfa->size(); ++i)
	{
		//read the flag of dfa state
		ReadNum(beg, nFlag);
		(*m_pDfa)[i].SetFlag(nFlag);
		for (byte j = 0; j < m_nColNum; ++j)
		{
			(*m_pDfa)[i][j] = 0;
			ReadNum(beg, (*m_pDfa)[i][j], sizeof(byte));
			if ((*m_pDfa)[i][j] == byte(-1))
			{
				(*m_pDfa)[i][j] = STATEID(-1);
			}
		}
	}
	
	//read the dfa's start state id
	m_nStartId = 0;
	ReadNum(beg, m_nStartId, sizeof(byte));
	
	//read the relationship between dfa's terminal state and dfa id
	ulong nFinStasSize;
	ReadNum(beg, nFinStasSize);
	for (ulong i = 0; i < nFinStasSize; ++i)
	{
		STATEID nStaId = 0;
		ulong nDfaId;
		ReadNum(beg, nStaId, sizeof(byte));
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
	for(byte i = 0; i < SC_DFACOLCNT - 1; ++i)
	{
		fout << (ulong)i << "\t" << (ulong)m_pGroup[i] << std::endl;
	}

	fout << 255 << "\t" << (ulong)m_pGroup[255] << std::endl;
	fout << "\t";
	for(byte j = 0; j != m_nColNum; ++j)
	{
		fout << (ulong)j << "\t";
	}
	fout << std::endl;
	for(ulong i = 0; i != m_pDfa->size(); ++i)
	{
		if(((*m_pDfa)[i].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			fout << i << ",Term\t";
		}
		else
		{
			fout << i << "\t";
		}
		for(byte j = 0; j != m_nColNum; ++j)
		{
			fout << (ulong)(*m_pDfa)[i][j] << "\t";
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
	ulong nDfaSize = m_pDfa->size();
	//�����̬��dfaId���Ա�֤��̬��Ÿ��ĺ����Ӧ��dfaId���ֲ���
	std::vector<ulong> finFlag(nDfaSize, ulong(-1));
	//for (ulong i = 0; i < m_FinStas.Size(); ++i)
	//{
	//	TERMSET &curSet = (*m_pTermSet)[i];
	//	termFlag[curSet.dfaSta] = curSet.dfaId;
	//}
	//m_pTermSet->clear();
	
	//ͳ�ƿɴ�״̬����Ŀ
	ulong nRcbCnt = reachable.size();
	ulong nColNum = GetGroupCount();

	//����һ��ͬ��Ա����m_pDfa������ͬ�ı��������ڴ洢ɾ������״̬���DFA��ת��
	std::vector<CDfaRow> *pNewDfa = new std::vector<CDfaRow>(nRcbCnt, CDfaRow(nColNum));
	std::vector<CDfaRow> &tmpDfa = *pNewDfa;

	ulong nMemSize = nDfaSize * sizeof(STATEID);
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
		for (byte j = 0; j < nColNum; ++j)
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
	ulong nStaNum = m_pDfa->size();

	partSet.clear();

	//�����������ڲ�ͬDFA����̬����
	CDfaIdSet *pTerm2Dfa = new CDfaIdSet[nStaNum];

	for (ulong i = 0; i < m_FinStas.Size(); ++i)
	{
		STATEID nFinStaId = m_FinStas[i];
		pTerm2Dfa[nFinStaId] = m_FinStas.GetDfaIdSet(nFinStaId);
	}

	//������̬���Ϻͷ���̬���ϣ�map��firstΪ�գ����ʾ��Ӧ��PARTSETΪ����̬���ϣ���֮��Ϊ��̬����
	//initBSet�м�������
	std::map<CDfaIdSet, PARTSET> initBSet;
	for (STATEID i = 0; i < nStaNum; ++i)
	{
		CDfaIdSet &curDfaSet = pTerm2Dfa[i];
		initBSet[curDfaSet].StaSet.push_back(i);
	}
	delete []pTerm2Dfa;

	//������̬�ͷ���̬���ϵ�˳��vector�е�lastΪ����̬����
	for (std::map<CDfaIdSet, PARTSET>::iterator i = initBSet.begin();
		i != initBSet.end(); ++i)
	{
		if (i->first.Size() != 0)
		{
			partSet.push_back(i->second);
		}
	}
	partSet.push_back(initBSet[CDfaIdSet()]);
}

void SetStateFlags(byte *pFlags, STATEVEC states)
{
	for (STATEVEC_ITER i = states.begin(); i != states.end(); ++i)
	{
		pFlags[*i] = 1;
	}
}

bool SortPartition(const byte *pAbleTo, PARTSET &partSet)
{
	STATELIST_ITER t = partSet.StaSet.begin();
	//������������t���ڼ���j��ǰ�Σ�������Ĵ��ڼ���j�ĺ��
	//����������t���Ϊ1������ȥpAbleToI��ǰ��Ϊ1��ֵ
	for (; t != partSet.StaSet.end() && pAbleTo[*t] != 0; ++t);
	bool bHasAble = true;
	if (t == partSet.StaSet.end())
	{
		return false;
	}
	if (t == partSet.StaSet.begin())
	{
		bHasAble = false;
	}
	//������г��ֵ�ֵΪ1�Ĳ�����ǰ��
	for (; t != partSet.StaSet.end();)
	{
		if (pAbleTo[*t] == 1)
		{
			partSet.StaSet.insert(partSet.StaSet.begin(), *t);
			t = partSet.StaSet.erase(t);
			bHasAble = true;
		}
		else
		{
			++t;
		}
	}
	return bHasAble;
}

ulong FindNotEmpty(const std::vector<ulong> *pVecAry, ulong nCnt)
{
	ulong ulRes = ulong(-1);
	for (ulong i = 0; i < nCnt; ++i)
	{
		if (pVecAry[i].empty() != true)
		{
			ulRes = i;
			break;
		}
	}
	return ulRes;
}

void InitPartWait(const std::vector<PARTSET> &partSet,
				  std::vector<ulong> *pWait, ulong ulGrpNum)
{
	typedef std::vector<PARTSET>::const_iterator PARTSETVEC_CITER;
	for (ulong i = 0; i < ulGrpNum; ++i)
	{
		ulong AcpSum = 0, NonAcpSum = 0;
		PARTSETVEC_CITER lastPart = partSet.cend() - 1;
		for (PARTSETVEC_CITER j = partSet.cbegin(); j != lastPart; ++j)
		{
			AcpSum += j->Ones[i];
		}
		NonAcpSum = partSet.back().Ones[i];
		if (AcpSum != 0 && NonAcpSum == 0)
		{
			for (ulong k = 0; k < partSet.size() - 1; ++k)
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
			for (ulong k = 0; k < partSet.size() - 1; ++k)
			{
				pWait[i].push_back(k);
			}
		}
	}
}

ulong CDfa::PartitionNonDisState(STATEVEC *pRevTbl)
{
	ulong nGrpNum = GetGroupCount();
	ulong nStaNum = m_pDfa->size();

	std::vector<PARTSET> partSet;
	InitPartSet(partSet);

	for (std::vector<PARTSET>::iterator i = partSet.begin();
		i != partSet.end(); ++i)
	{
		//����partSet��ÿ�����ϣ����ݲ�ͬ��nGrpNum���㲻ͬAbleTo��AbleTo��Ӧ�����е�a(i)
		CalcAbleTo(pRevTbl, nGrpNum, nStaNum, *i);
	}

	//pWait��ʾ�����з�partSet��ÿ�����ϵļ����±꣬
	//���ڲ�ͬ�������ַ��ᱣ������ͬ�����±ꡣ��Ӧ������L(a)
	std::vector<ulong> pWait[256];

	//��ʼ��pWait��Ҫ��������0<|ij|<=|ik|����j����pWait����֮������k
	//��Ӧ�����г�ʼ��L(a)���̣������i��Ӧa
	InitPartWait(partSet, pWait, nGrpNum);

	//����ܹ�����ĳһ�����ַ����ܹ�����ISet�е�״̬��״̬����
	byte *pAbleToI = new byte[nStaNum];
	partSet.reserve(1000);
	ulong nr = 0;
	for (; nr == 0; )
	{
		ulong ulCurGrp = (byte)-1;
		ulong nCurSet = (ulong)-1;
		//��pWait��ȡһ��ֵ��remove��ֵ
		ulCurGrp = FindNotEmpty(pWait, nGrpNum);
		//�����ѭ������ֹ����
		if (ulCurGrp == -1)
		{
			break;
		}
		nCurSet = pWait[ulCurGrp].back();
		pWait[ulCurGrp].pop_back();

		PARTSET *pISet = &partSet[nCurSet];
		ZeroMemory(pAbleToI, nStaNum);
		ulong nRevCnt = 0;
		//��ʼ��pAbleToI
		for (STATELIST_ITER iSta = pISet->StaSet.begin();
			iSta != pISet->StaSet.end(); ++iSta)
		{
			ulong nRevIdx = *iSta * nGrpNum + ulCurGrp;
			STATEVEC &revI = pRevTbl[nRevIdx];
			SetStateFlags(pAbleToI, revI);
			nRevCnt += revI.size();
		}

		if (nRevCnt != 0)
		{
			for (ulong j = 0; j != partSet.size(); ++j)
			{
				//ȡpartSet�е�һ������j��B(j)�д���t����������(t,a)��a(i)��
				//B��(j)Ϊt�ļ��ϣ�B��(j)ΪB(j)-B��(j)
				PARTSET *pJSet = &partSet[j];
				if (SortPartition(pAbleToI, *pJSet) == false)
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
					nr = ulong(-1);
					break;
				}
				//����������µĻ���
				PARTSET &lastPart = partSet.back();
				lastPart.StaSet.splice(lastPart.StaSet.begin(),
					pJSet->StaSet, part, pJSet->StaSet.end());

				CalcAbleTo(pRevTbl, nGrpNum, nStaNum, *pJSet);
				CalcAbleTo(pRevTbl, nGrpNum, nStaNum, lastPart);

				////����pWait
				for (byte m = 0; m < nGrpNum; ++m)
				{
					int k = partSet.size() - 1;
					std::vector<ulong> &curWait = pWait[m];
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

	if (partSet.size() < m_pDfa->size())
	{
		//Partition�е�Ԫ��Ϊһ��״̬�ļ��ϣ�������Ԫ��Ϊ����ȼ�״̬��ÿ�����Ͽ��Ժϲ�Ϊ�µ�DFA��һ��״̬
		STATEVEC sta2Part(m_pDfa->size());

		STATEID nCol = (STATEID)GetGroupCount();

		CFinalStates newFinStas;
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
					newFinStas.PushBack(nSetIdx);
					newFinStas.GetDfaIdSet(nSetIdx) =
						m_FinStas.GetDfaIdSet(*iSta);
				}
			}
			++nSetIdx;
		}
		m_FinStas.Swap(newFinStas);

		//set new DFA and modify new number
		nSetIdx = 0;
		for (std::vector<PARTSET>::iterator iPart = partSet.begin(); iPart != partSet.end(); ++iPart)
		{
			CDfaRow &curRow = tmpDfa[nSetIdx];
			CDfaRow &orgRow = (*m_pDfa)[iPart->StaSet.front()];
			for (byte iCol = 0; iCol != nCol; ++iCol)
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
		//DFA minization
		std::cout << "Minimized: " << (nStaNum - m_pDfa->size()) << std::endl;
	}
	return nr;
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
**	NAME
**	 MergeMultipleDfas::
*/
/**
**	This function merges mutiple dfas into one dfa. And mark the terminal states to 
**	distinguish which dfas the terminal state belongs to.
**
**	In order to speed up, we need one support function:DfaColGroup to group
**	the lastDfa's columns.
**
**	@param dfas		a vector contains mutiple CDfa
**	@param lastDfa	the merged dfa
**
**	@return bool
**
**	@retval true function successful
**	@retval fasle fatal error
*/

DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName)
{
	std::ofstream fout(fileName);
	fout << "digraph G {" << std::endl;
	fout << "S -> " << (ulong)newdfa.GetStartId() << std::endl;

	for(STATEID i = 0; i != newdfa.Size(); ++i)
	{
		std::map<STATEID, ulong> rowStateCnt;
		for(byte j = 0; j != newdfa.GetGroupCount(); ++j)
		{
			rowStateCnt[newdfa[i][j]]++;
		}
		STATEID maxId = 0;
		for (std::map<STATEID, ulong>::iterator j = rowStateCnt.begin(); j != rowStateCnt.end(); ++j)
		{
			if (j->second > rowStateCnt[maxId])
			{
				maxId = j->first;
			}
		}
		for(byte j = 0; j != newdfa.GetGroupCount(); ++j)
		{
			if (newdfa[i][j] != maxId)
			{
				fout << i << " -> " << (ulong)newdfa[i][j] << " [label=\"" << j << "\"];" << std::endl;
			}
			else if (maxId != (STATEID)-1)
			{
				fout << i << " -> "  << (ulong)maxId << " [label=\"" << j << "\"];" << std::endl;
			}
		}
	}
	for (STATEID i = 0; i < newdfa.Size(); ++i)
	{
		if (newdfa[i].GetFlag() & CDfaRow::TERMINAL)
		{
			fout << (ulong)i << " [peripheries=2];" << std::endl;
		}
	}
	fout << "}" << std::endl;
	fout.close();
}

DFAHDR bool MergeMultipleDfas(std::vector<CDfa> &dfas, CDfa &lastDfa)
{
	ulong dfaId = lastDfa.GetId();
	lastDfa.Clear();
	lastDfa.SetId(dfaId);
#undef max

	ulong dfasSize = dfas.size();
	STATEID nTermSta = 1;//nTermSta: terminal flag. 1: terminal, -1: non-terminal

	//group the lastDfa's columns
	byte groups[SC_DFACOLCNT];
	DfaColGroup(dfas, groups);
	lastDfa.SetGroups(groups);

	ulong colCnt = lastDfa.GetGroupCount();

	typedef std::unordered_map<std::vector<STATEID>, STATEID, TODFA_HASH> STATESETHASH;
	STATESETHASH statehash;

	ulong finFlag = 0;//terminal state flag, 1: terminal state, 0: normal state
	std::stack<std::vector<STATEID>> statesStack;

	/*
	**	use a size of (dfas.size() + 2) vector to represent a state of the merged dfa.
	**	element 0 represents the state of dfas[0], ..., element n represents the state of dfas[n].
	**	element n + 1 and element n + 2 are flags which show that whether this state is a start state or terminal state.
	*/
	std::vector<STATEID> startVec(dfasSize + 2);
	
	lastDfa.Reserve(SC_CHARSETSIZE);
	lastDfa.Resize(lastDfa.Size() + 1, colCnt);

	for(ulong i = 0; i < dfasSize; ++i)
	{
		STATEID nSta = dfas[i].GetStartId();
		if((dfas[i][nSta].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			//this is a terminal state
			finFlag = 1;
			AddTermIntoDFA(nSta, dfas[i], 0, lastDfa);
			CFinalStates &newFinSta = lastDfa.GetFinalState();
			newFinSta.PushBack(0);
			newFinSta.GetDfaIdSet(0).Append(
				dfas[i].GetFinalState().GetDfaIdSet(nSta));

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
	byte computFlag[SC_CHARSETSIZE];

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
		for(ulong curChar = 0; curChar < SC_DFACOLCNT; ++curChar)
		{
			finFlag = 0;
			//ZeroMemory(NextVec.data(), NextVec.size() * sizeof(NextVec.front()));
			byte lastDfaGroup = groups[curChar];
			if(computFlag[lastDfaGroup] == 1)
			{
				continue;
			}
			computFlag[lastDfaGroup] = 1;

			ulong flag = 0;//flag = 0: empty next state
			for(ulong i = 0; i < dfasSize; ++i)
			{
				STATEID sta = curVec[i];
				
				if(sta != (STATEID)-1)
				{
					byte curgroup = dfas[i].Char2Group((byte)curChar);
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

