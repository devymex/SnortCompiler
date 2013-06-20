/**
**	@file		dfa.cpp
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

#include <hwprj\dfa.h>


DFAHDR CDfa::CDfa()
	: m_nId			(ulong(-1))
	, m_usColNum	(ulong(0))
	, m_nStartId	(STATEID(0))
{
	memset(m_pGroup, -1, sizeof(m_pGroup));
	try
	{
		m_pDfa = new std::vector<CDfaRow>;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAHDR CDfa::CDfa(const CDfa &other)
	: m_nId			(other.m_nId)
	, m_usColNum	(other.m_usColNum)
	, m_nStartId	(other.m_nStartId)
	, m_FinStas		(other.m_FinStas)
{
	try
	{
		m_pDfa = new std::vector<CDfaRow>(*other.m_pDfa);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	CopyMemory(m_pGroup, other.m_pGroup, sizeof(m_pGroup));
}

DFAHDR CDfa::~CDfa()
{
	delete m_pDfa;
}

DFAHDR CDfa& CDfa::operator=(const CDfa &other)
{
	TASSERT(other.m_pDfa != null);

	m_nId = other.m_nId;
	m_usColNum = other.m_usColNum;
	m_nStartId = other.m_nStartId;
	CopyMemory(m_pGroup, other.m_pGroup, sizeof(m_pGroup));
	try
	{
		*m_pDfa = *other.m_pDfa;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	m_FinStas = other.m_FinStas;
	return *this;
}

DFAHDR CDfaRow& CDfa::operator[](STATEID nIdx)
{
	TASSERT(nIdx < m_pDfa->size());
	return (*m_pDfa)[nIdx];
}

DFAHDR const CDfaRow& CDfa::operator[](STATEID nIdx) const
{
	TASSERT(nIdx < m_pDfa->size());
	return (*m_pDfa)[nIdx];
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
	try
	{
		m_pDfa->reserve(nSize);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAHDR void CDfa::Resize(ulong nSize, ulong nCol)
{
	try
	{
		m_pDfa->resize(nSize, CDfaRow(nCol));
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAHDR void CDfa::Clear()
{
	m_nId = ulong(-1);
	m_usColNum = ulong(0);
	m_nStartId = STATEID(0);
	memset(m_pGroup, 0xFF, SC_DFACOLCNT);
	m_pDfa->clear();
	m_FinStas.Clear();
}

DFAHDR void CDfa::PushBack(CDfaRow &sta)
{
	m_pDfa->push_back(sta);
}

DFAHDR ulong CDfa::GetId()
{
	return m_nId;
}

DFAHDR void CDfa::SetId(ulong id)
{
	m_nId = id;
}

DFAHDR ushort CDfa::GetGroupCount() const
{
	return m_usColNum;
}

DFAHDR void CDfa::SetGroups(byte *pGroup)
{
	TASSERT(pGroup != null);
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
		m_usColNum = nZeroBegPos;
		CopyMemory(m_pGroup, pGroup, SC_DFACOLCNT * sizeof(byte));
	}
	else
	{
		throw 0;
	}
}

DFAHDR byte CDfa::Char2Group(byte nIdx)
{
	return m_pGroup[nIdx];
}

DFAHDR STATEID CDfa::GetStartState() const
{
	return m_nStartId;
}

DFAHDR void CDfa::SetStartState(STATEID id)
{
	m_nStartId = id;
}

CFinalStates& CDfa::GetFinalStates()
{
	return m_FinStas;
}

const CFinalStates& CDfa::GetFinalStates() const
{
	return m_FinStas;
}

DFAHDR ulong CDfa::FromNFA(const CNfa &nfa)
{
	typedef std::unordered_map<STATEIDARY, STATEID, NSTATESET_HASH> STATESETHASH;
	//std::vector<std::pair<std::vector<ulong>, STATEID>> termStasVec;

	ulong dfaId = m_nId;
	Clear();
	m_nId = dfaId;
	
	std::vector<STATEIDARY> eClosure;
	NfaEClosure(nfa, eClosure);

	byte groups[CNfaRow::COLUMNCNT];
	GroupNfaColumns(nfa, groups);
	SetGroups(groups);

	std::stack<STATEIDARY> nfaStasStack;
	STATEIDARY &startEVec = eClosure.front();
	STATESETHASH ssh;

	try
	{
		nfaStasStack.push(startEVec);
		ssh[startEVec] = 0;
		m_pDfa->reserve(300);
		m_pDfa->push_back(CDfaRow(m_usColNum));

		CDfaRow &firstRow = m_pDfa->back();
		firstRow.AddFlags(CDfaRow::START);

		if (startEVec.back() == nfa.Size())
		{
			firstRow.AddFlags(CDfaRow::FINAL);
			m_FinStas.AddState(0).AddDfaId(m_nId);
		}

		static STATEIDARY nextNfaVec; //
		nextNfaVec.clear();
		byte compuFlag[CNfaRow::COLUMNCNT];

		for (; nfaStasStack.size() > 0; )
		{
			STATEIDARY curNfaVec = nfaStasStack.top();
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
						m_pDfa->push_back(CDfaRow(m_usColNum));
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
							lastRow.AddFlags(CDfaRow::FINAL);
							m_FinStas.AddState(nextSta).AddDfaId(m_nId);
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
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAHDR ulong CDfa::Minimize()
{
	ulong nSize = m_pDfa->size();
	ulong nCols = GetGroupCount();
	if (nSize == 0 || nSize == 0)
	{
		return ulong(-1);
	}

	//Indicate current state would be jump to the next state set through the character.
	nSize = m_pDfa->size();
	STATEIDARY *pRevTab = null;
	try
	{
		pRevTab = new STATEIDARY[nSize * nCols];
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
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}

	ulong nr = PartStates(pRevTab);

	delete []pRevTab;

	return nr;
}

DFAHDR void CDfa::MergeColumn()
{
	byte groups[SC_DFACOLCNT];
	GroupDfaColumns(*this, groups);
	std::set<byte> gpset;
	for (ushort i = 0; i < m_usColNum; ++i)
	{
		gpset.insert(groups[i]);
	}
	if (gpset.size() != m_usColNum)
	{
		std::cout << m_usColNum << ": " << m_usColNum - gpset.size() << std::endl;
		//TTHROW("gpset.size() != m_usColNum");
	}
}

DFAHDR ulong CDfa::CalcStoreSize() const
{
	ulong nSize = 0;

	//m_nId
	nSize += sizeof(m_nId);

	//m_pDfa->size()
	nSize += sizeof(byte);

	if (m_pDfa->size() == 0)
	{
		return nSize;
	}

	//group
	nSize += SC_DFACOLCNT * sizeof(byte);

	//dfa
	nSize += m_pDfa->size() * (sizeof(ulong) + m_usColNum);

	//m_nStartId
	nSize += sizeof(byte);

	//m_FinStas.GetAllDfaIdCount()
	nSize += sizeof(ulong);

	//the relationship of final state and dfa id
	nSize += m_FinStas.CountDfaIds() * (sizeof(byte) + sizeof(ulong));

	return nSize;
}

DFAHDR void CDfa::Save(byte *beg)
{
	//write dfa id
	WriteNum(beg, m_nId);

	//write number of dfa states
	WriteNum(beg, m_pDfa->size(), sizeof(byte));
	if (m_pDfa->size() == 0)
	{
		return;
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
		WriteNum(beg, (*m_pDfa)[i].GetFlags());
		for (byte j = 0; j < m_usColNum; ++j)
		{
			WriteNum(beg, (*m_pDfa)[i][j], sizeof(byte));
		}
	}
	
	//write the dfa's start state id
	WriteNum(beg, m_nStartId, sizeof(byte));

	//write the relationship between dfa's terminal state and dfa id
	WriteNum(beg, m_FinStas.CountDfaIds());
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
}

DFAHDR void CDfa::Load(byte *beg)
{
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
	m_pDfa->resize(dfaSize, CDfaRow(m_usColNum));
	ulong nFlag;
	for (ulong i = 0; i < m_pDfa->size(); ++i)
	{
		//read the flag of dfa state
		ReadNum(beg, nFlag);
		(*m_pDfa)[i].SetFlags(nFlag);
		for (byte j = 0; j < m_usColNum; ++j)
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
		m_FinStas.AddState(nStaId).AddDfaId(nDfaId);
	}
}

DFAHDR ulong CDfa::Process(byte *ByteStream, ulong len, STATEARY &StaSet)
{
	std::vector<bool> res;
	ulong nPos = 0;
	try
	{
		res.resize(m_pDfa->size(), false);
		STATEID ActiveState = m_nStartId;
		for (nPos = 0; nPos < len; ++nPos)
		{
			if ((*this)[ActiveState].GetFlags() & CDfaRow::FINAL)
			{
				res[ActiveState] = true;
			}
			ActiveState = (*this)[ActiveState][m_pGroup[*(ByteStream + nPos)]];
			if (ActiveState == STATEID(-1))
			{
				break;
			}
		}
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
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

DFAHDR void CDfa::Dump(const char *pFile)
{
	std::ofstream fout(pFile);
	if(!fout)
	{
		g_log << "file open error!" << g_log.nl;
	}

	fout << "字符和组对应关系：" << std::endl;
	for(byte i = 0; i < SC_DFACOLCNT - 1; ++i)
	{
		fout << (ulong)i << "\t" << (ulong)m_pGroup[i] << std::endl;
	}

	fout << 255 << "\t" << (ulong)m_pGroup[255] << std::endl;
	fout << "\t";
	for(byte j = 0; j != m_usColNum; ++j)
	{
		fout << (ulong)j << "\t";
	}
	fout << std::endl;
	for(ulong i = 0; i != m_pDfa->size(); ++i)
	{
		if(((*m_pDfa)[i].GetFlags() & CDfaRow::FINAL) != 0)
		{
			fout << i << ",Term\t";
		}
		else
		{
			fout << i << "\t";
		}
		for(byte j = 0; j != m_usColNum; ++j)
		{
			fout << (ulong)(*m_pDfa)[i][j] << "\t";
		}
		fout << std::endl;
	}
	fout << std::endl;
	fout.close();
}

/* 	Divide nondistinguishable states and merge equivalent states

Arguments:
  inverse table

Returns:		0 success
				-1 error 
*/

ulong CDfa::PartStates(STATEIDARY *pRevTbl)
{
	ulong nGrpNum = GetGroupCount();
	ulong ulStaNum = m_pDfa->size();

	PARTSETVEC partSets;
	InitPartSet(m_FinStas, ulStaNum, partSets);

	for (PARTSETVEC_ITER i = partSets.begin(); i != partSets.end(); ++i)
	{
		CalcAbleTo(pRevTbl, nGrpNum, ulStaNum, *i);
	}

	//divide partSet using pWait in a loop
	//store index of partSet into pWait for different symbol 
	ULONGVEC pWait[256];

	//inital pWait; for same symbol, if |partSet[j]|<=|partSet[k]|，
	//then insert j into pWait, otherwise，insert k	
	InitPartWait(partSets, pWait, nGrpNum);

	byte *pAbleToI = null;
	try
	{
		pAbleToI = new byte[ulStaNum];
		partSets.reserve(1000);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}

	ulong nr = 0;
	for (; nr == 0; )
	{
		ulong ulCurGrp = (byte)-1;
		ulong nCurSet = (ulong)-1;
		//choose and remove one set's index in pWait
		ulCurGrp = FindNotEmpty(pWait, nGrpNum);
		//finish the outer loop
		if (ulCurGrp == -1)
		{
			break;
		}
		nCurSet = pWait[ulCurGrp].back();
		pWait[ulCurGrp].pop_back();

		PARTSET *pISet = &partSets[nCurSet];
		ZeroMemory(pAbleToI, ulStaNum);
		ulong nRevCnt = 0;
		for (STATELIST_ITER iSta = pISet->StaSet.begin();
			iSta != pISet->StaSet.end(); ++iSta)
		{
			ulong nRevIdx = *iSta * nGrpNum + ulCurGrp;
			STATEIDARY &revI = pRevTbl[nRevIdx];
			SetStateFlags(pAbleToI, revI);
			nRevCnt += revI.size();
		}

		if (nRevCnt != 0)
		{
			for (ulong j = 0; j != partSets.size(); ++j)
			{
				/*choose partSet[j] and divide partSet[j] for a jump character*/
				PARTSET *pJSet = &partSets[j];
				if (SortPartition(pAbleToI, *pJSet) == false)
				{
					continue;
				}

				partSets.push_back(PARTSET());

				pJSet = &partSets[j];
				pISet = &partSets[nCurSet];

				STATELIST_ITER part = pJSet->StaSet.begin();
				for (; part != pJSet->StaSet.end() &&
					pAbleToI[*part] != 0; ++part);

					if (partSets.size() > 254)
					{
						nr = ulong(-1);
						break;
					}

					PARTSET &lastPart = partSets.back();
					lastPart.StaSet.splice(lastPart.StaSet.begin(),
						pJSet->StaSet, part, pJSet->StaSet.end());
					CalcAbleTo(pRevTbl, nGrpNum, ulStaNum, *pJSet);
					CalcAbleTo(pRevTbl, nGrpNum, ulStaNum, lastPart);

					//modify pWait
					for (byte m = 0; m < nGrpNum; ++m)
					{
						int k = partSets.size() - 1;
						ULONGVEC &curWait = pWait[m];
						int aj = pJSet->Ones[m];
						int ak = lastPart.Ones[m];
						if (aj > 0 && aj <= ak)
						{
							if (std::find(curWait.begin(), curWait.end(), j) ==
								curWait.end())
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


	if (nr != ulong(-1) && partSets.size() < m_pDfa->size())
	{
		STATEIDARY sta2Part(m_pDfa->size());
		STATEID nCol = (STATEID)GetGroupCount();

		CFinalStates newFinStas;

		DFAROWARY *pNewDfa = null;
		try
		{
			pNewDfa = new DFAROWARY((STATEID)partSets.size(), CDfaRow(nCol));
		}
		catch (std::exception &e)
		{
			TTHROW(e.what());
		}

		//renumber DFA state
		BuildDfaByPart(partSets, *m_pDfa, *pNewDfa);

		//remark new start state and final states
		for (PARTSETVEC_ITER i = partSets.begin(); i != partSets.end(); ++i)
		{
			STATEID nState = STATEID(i - partSets.begin());
			for (STATELIST_ITER j = i->StaSet.begin(); j != i->StaSet.end(); ++j)
			{
				CDfaRow &curRow = (*m_pDfa)[*j];
				if (curRow.GetFlags() & CDfaRow::START)
				{
					m_nStartId = nState;
				}

				if (curRow.GetFlags() & CDfaRow::FINAL)
				{
					newFinStas.AddState(nState) = m_FinStas.GetDfaIdSet(*j);
				}
			}
		}
		m_FinStas.Swap(newFinStas);

		//replace m_pDfa
		swap(m_pDfa, pNewDfa);
		delete pNewDfa;

		g_log << "Minimized: " << (ulStaNum - m_pDfa->size()) << g_log.nl;
	}

	for (PARTSETVEC_ITER i = partSets.begin(); i != partSets.end(); ++i)
	{
		ReleaseAbleTo(*i);
	}
	return nr;
}

DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName)
{
	std::ofstream fout(fileName);
	fout << "digraph G {" << std::endl;
	fout << "S -> " << (ulong)newdfa.GetStartState() << std::endl;

	for(STATEID i = 0; i != newdfa.Size(); ++i)
	{
		std::map<STATEID, ulong> rowStateCnt;
		for(byte j = 0; j != newdfa.GetGroupCount(); ++j)
		{
			try
			{
				rowStateCnt[newdfa[i][j]]++;
			}
			catch (std::exception &e)
			{
				TTHROW(e.what());
			}
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
				fout << i << " -> " << (ulong)newdfa[i][j] << " [label=\"" << (int)j << "\"];" << std::endl;
			}
			else if (maxId != STATEID(-1))
			{
				fout << i << " -> "  << (ulong)maxId << " [label=\"" << (int)j << "\"];" << std::endl;
			}
		}
	}
	for (STATEID i = 0; i < newdfa.Size(); ++i)
	{
		if (newdfa[i].GetFlags() & CDfaRow::FINAL)
		{
			fout << (ulong)i << " [peripheries=2];" << std::endl;
		}
	}
	fout << "}" << std::endl;
	fout.close();
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
DFAHDR bool MergeMultipleDfas(CDfaArray &inputDfas, CDfa &mergedDfa)
{
	ulong dfaId = mergedDfa.GetId();
	mergedDfa.Clear();
	mergedDfa.SetId(dfaId);

	ulong dfasSize = inputDfas.Size();
	STATEID nTermSta = 1;//nTermSta: terminal flag. 1: terminal, -1: non-terminal

	//group the mergedDfa's columns
	byte groups[SC_DFACOLCNT];
	DfaColGroup(inputDfas, groups);
	mergedDfa.SetGroups(groups);

	ulong colCnt = mergedDfa.GetGroupCount();

	typedef std::unordered_map<STATEIDARY, STATEID, TODFA_HASH> STATESETHASH;
	STATESETHASH statehash;

	ulong finFlag = 0;//terminal state flag, 1: terminal state, 0: normal state
	std::stack<STATEIDARY> statesStack;

	/*
	**	use a size of (inputDfas.size() + 2) vector to represent a state of the merged dfa.
	**	element 0 represents the state of inputDfas[0], ..., element n represents the state of inputDfas[n].
	**	element n + 1 and element n + 2 are flags which show that whether this state is a start state or terminal state.
	*/
	STATEIDARY startVec;
	
	try
	{
		startVec.resize(dfasSize + 2);
		mergedDfa.Reserve(CNfaRow::COLUMNCNT);
		mergedDfa.Resize(mergedDfa.Size() + 1, colCnt);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}

	for(ulong i = 0; i < dfasSize; ++i)
	{
		STATEID nSta = inputDfas[i].GetStartState();
		if((inputDfas[i][nSta].GetFlags() & CDfaRow::FINAL) != 0)
		{
			//this is a terminal state
			finFlag = 1;
			AddTermIntoDFA(nSta, inputDfas[i], 0, mergedDfa);
			CFinalStates &newFinSta = mergedDfa.GetFinalStates();
			newFinSta.AddState(0).Append(
				inputDfas[i].GetFinalStates().GetDfaIdSet(nSta));

		}
		startVec[i] = nSta;
	}
	startVec[dfasSize] = 0;//this is start state
	if(finFlag)
	{
		startVec[dfasSize + 1] = nTermSta;
		mergedDfa[0].AddFlags(CDfaRow::START | CDfaRow::FINAL);
	}
	else
	{
		startVec[dfasSize + 1] = STATEID(-1);
		mergedDfa[0].AddFlags(CDfaRow::START);
	}

	std::vector<STATEID> NextVec;

	statehash[startVec] = 0;
	statesStack.push(startVec);
	NextVec.resize(dfasSize + 2);

	byte computFlag[CNfaRow::COLUMNCNT];

	bool br = true;
	for (; br && !statesStack.empty(); )
	{
		std::vector<STATEID> curVec = statesStack.top();//current state, size is:dfasSize + 2
		statesStack.pop();

		STATESETHASH::iterator ir = statehash.find(curVec);
		if (ir == statehash.end())
		{
			g_log << "hash Error!" << g_log.nl;
			return false;
		}
		STATEID curStaNum = ir->second;
		ZeroMemory(computFlag, sizeof(computFlag));

		//get next states
		for(ulong curChar = 0; curChar < SC_DFACOLCNT; ++curChar)
		{
			finFlag = 0;
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

				if(sta != STATEID(-1))
				{
					byte curgroup = inputDfas[i].Char2Group((byte)curChar);
					STATEID nextId = inputDfas[i][sta][curgroup];//the next state the ith dfa transforms from state curVec[i] through curChar to
					if(nextId != STATEID(-1))
					{
						flag = 1;
						if((inputDfas[i][nextId].GetFlags() & CDfaRow::FINAL) != 0)
						{
							finFlag = 1;
						}
					}
					NextVec[i] = nextId; 
				}
				else
				{
					NextVec[i] = STATEID(-1);
				}
			}
			NextVec[dfasSize] = STATEID(-1);
			if(finFlag)
			{
				NextVec[dfasSize + 1] = nTermSta;
			}
			else
			{
				NextVec[dfasSize + 1] = STATEID(-1);
			}

			if(flag)
			{
				STATESETHASH::iterator nextIt = statehash.find(NextVec);
				if(nextIt == statehash.end())
				{
					mergedDfa.Resize(mergedDfa.Size() + 1, colCnt);
					if(mergedDfa.Size() > SC_STATELIMIT)
					{
						br = false;
						break;
					}
					STATEID nextSta = (STATEID)statehash.size();
					statehash[NextVec] = nextSta;
					mergedDfa[curStaNum][lastDfaGroup] = nextSta;

					if(finFlag)
					{
						for(STATEID k = 0; k < dfasSize; ++k)
						{
							if(NextVec[k] != STATEID(-1))
							{
								if((inputDfas[k][NextVec[k]].GetFlags() &
									CDfaRow::FINAL) != 0)
								{
									AddTermIntoDFA(NextVec[k], inputDfas[k],
										nextSta, mergedDfa);
								}
							}
						}
						mergedDfa[nextSta].AddFlags(CDfaRow::FINAL);
					}
					statesStack.push(NextVec);
				}
				else
				{
					mergedDfa[curStaNum][lastDfaGroup] = nextIt->second;
				}
			}
			else
			{
				mergedDfa[curStaNum][lastDfaGroup] = STATEID(-1);
			}
		}
	}
	//mergedDfa.Minimize();
	if(mergedDfa.Size() > SC_MAXDFASIZE)
	{
		//std::cerr << "SC_MAXDFASIZE!" << std::endl;
		br = false;
	}
	return br;
}

