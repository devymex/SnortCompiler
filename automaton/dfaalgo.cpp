/**
**	@file		dfaalgo.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Common classes declaration
**
**	Include CUnsignedArray, CDllString
**
*/

#include "stdafx.h"
#include <hwprj\dfa.h>
#include <hwprj\nfa.h>
#include "dfaalgo.h"

void Warshall(byte *pMat, ulong nWidth, ulong nHeight)
{
	for (ulong k = 0; k < nHeight; ++k)
	{
		for (ulong i = 0; i < nHeight; ++i)
		{
			ulong row = i * nWidth;
			if (pMat[row + k])
			{
				__m128i *pBeg1 = (__m128i*)(pMat + row);
				__m128i *pBeg2 = (__m128i*)(pMat + k * nWidth);
				ulong nInts = nWidth >> 4;
				for (ulong j = 0; j < nInts; ++j)
				{
					*pBeg1 = _mm_or_si128(*pBeg1, *pBeg2);
					++pBeg1;
					++pBeg2;
				}
			}
		}
	}
}

void NfaEClosure(const CNfa &nfa, std::vector<STATEIDARY> &eClosure)
{
	TASSERT(nfa.Size() < 65535);

	ushort nNfaSize = ushort(nfa.Size());
	ushort nMatHeight = nNfaSize + 1;
	ushort nMatWidth = nMatHeight;
	if (nMatWidth % 16 != 0)
	{
		nMatWidth = (nMatWidth / 16) * 16 + 16;
	}
	byte *pMat = (byte*)_aligned_malloc(nMatWidth * nMatHeight, 128);
	if (pMat == null)
	{
		throw CTrace(__FILE__, __LINE__, "Out of memory");
	}
	memset(pMat, 0, nMatWidth * nMatHeight);

	for (ulong i = 0; i < nNfaSize; ++i)
	{
		const CNfaRow &row = nfa[i];
		ulong nCnt = row[SC_DFACOLCNT].Size();
		const ulong *pDest = row[SC_DFACOLCNT].Data();
		for (ulong k = 0; k < nCnt; ++k)
		{
			pMat[i * nMatWidth + pDest[k]] = 1;
		}
		pMat[i * nMatWidth + i] = 1;
	}

	Warshall(pMat, nMatWidth, nMatHeight);

	try
	{
		eClosure.resize(nNfaSize + 1);
		for (ulong i = 0; i < nNfaSize; ++i)
		{
			for (ulong j = 0; j < nMatWidth; ++j)
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
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

void GetNextEClosureSet(const CNfa &nfa, const std::vector<STATEIDARY> &eClosure,
						const STATEIDARY &curSet, ulong edge, STATEIDARY &eClosureSet)
{
	TASSERT(edge < CNfaRow::COLUMNCNT);
	TASSERT(nfa.Size() < 65535);

	static STATEIDARY nextSet; //
	nextSet.clear();
	try
	{
		nextSet.reserve(1000);
		ulong nSize = nfa.Size();
		for(STATEIDARY::const_iterator i = curSet.begin(); i != curSet.end(); ++i)
		{
			STATEID nCurSta = STATEID(*i);
			if(nCurSta != nSize)
			{
				const CNfaRow &row = nfa[nCurSta];
				ulong nCurCnt = nextSet.size();
				ulong nAddCnt = row[edge].Size();
				if (nAddCnt != 0)
				{
					nextSet.resize(nCurCnt + nAddCnt);

					STATEID *pDest = nextSet.data() + nCurCnt;
					const ulong *pStates = row[edge].Data();

					for (ulong k = 0; k < nAddCnt; ++k)
					{
						pDest[k] = STATEID(pStates[k]);
					}
				}
			}
		}
		std::sort(nextSet.begin(), nextSet.end());
		nextSet.erase(std::unique(nextSet.begin(), nextSet.end()), nextSet.end());

		for (STATEIDARY::const_iterator i = nextSet.cbegin(); i != nextSet.cend(); ++i)
		{
			const STATEIDARY &ci = eClosure[*i];
			eClosureSet.insert(eClosureSet.end(), ci.cbegin(), ci.cend());
		}
		std::sort(eClosureSet.begin(), eClosureSet.end());
		eClosureSet.erase(std::unique(eClosureSet.begin(),
			eClosureSet.end()), eClosureSet.end());
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

ulong CalcFNV(const std::vector<ulong> &ary)
{
	const ulong _FNV_offset_basis = 2166136261U;
	const ulong _FNV_prime = 16777619U;

	ulong hash = _FNV_offset_basis;
	for (std::vector<ulong>::const_iterator i = ary.cbegin(); i != ary.cend(); ++i)
	{
		hash ^= *i;
		hash *= _FNV_prime;
	}
	return hash;
}

void CalcDfaColumnKeys(const CDfa &dfa, COLUMNKEY *pColumns)
{
	ulong nSize = dfa.Size();
	ulong nCols = dfa.GetGroupCount();
	for (ulong i = 0; i < nSize; ++i)
	{
		const CDfaRow &curRow = dfa[STATEID(i)];
		for (ulong j = 0; j < nCols; ++j)
		{
			pColumns[j].key.push_back(curRow[byte(j)]);
		}
	}
	for (ulong i = 0; i < nCols; ++i)
	{
		pColumns[i].hash = CalcFNV(pColumns[i].key);
	}
}

void CalcNfaColumnKeys(const CNfa &nfa, COLUMNKEY *pColumns)
{
	ulong nSize = nfa.Size();
	ulong zeroCnts[CNfaRow::COLUMNCNT] = {0};
	for (ulong i = 0; i < nSize; ++i)
	{
		const CNfaRow &curRow = nfa[i];
		for (ulong j = 0; j < CNfaRow::COLUMNCNT; ++j)
		{
			std::vector<ulong> &curCol = pColumns[j].key;
			ulong ulElemSize = curRow[j].Size();
			const ulong *pElem = curRow[j].Data();
			if (ulElemSize != 0)
			{
				if (zeroCnts[j] > 0)
				{
					curCol.resize(curCol.size() + zeroCnts[j], ulong(-1));
					zeroCnts[j] = 0;
				}
				curCol.push_back(ulong(-1));
				for (ulong k = 0; k < ulElemSize; ++k)
				{
					curCol.push_back(pElem[k]);
				}
			}
			else
			{
				++zeroCnts[j];
			}
		}
	}
	for (ulong i = 0; i < CNfaRow::COLUMNCNT; ++i)
	{
		std::vector<ulong> &curCol = pColumns[i].key;
		ulong nCurSize = curCol.size();
		curCol.resize(nCurSize + zeroCnts[i]);
		memset(curCol.data() + nCurSize, -1, zeroCnts[i] * sizeof(curCol[0]));
		pColumns[i].hash = CalcFNV(pColumns[i].key);
	}
}


void ColumnHashGroup(const COLUMNKEY *pColumns, ulong ulColCnt, byte *pGroups)
{
	typedef std::unordered_map<COLUMNKEY, byte, COLUMNKEYHASH> COLUMNHASHMAP;
	static COLUMNHASHMAP colHash;
	colHash.clear();
	for(ulong i = 0; i < ulColCnt; ++i)
	{
		const COLUMNKEY &curCol = pColumns[i];
		COLUMNHASHMAP::iterator identical = colHash.find(curCol);
		if(identical == colHash.end())
		{
			byte curId = (byte)colHash.size();
			try
			{
				colHash[curCol] = curId;
			}
			catch (std::exception &e)
			{
				TTHROW(e.what());
			}
			pGroups[i] = curId;
		}
		else
		{
			pGroups[i] = identical->second;
		}
	}
}

void GroupDfaColumns(const CDfa &dfa, byte *pGroups)
{
	static COLUMNKEY columns[SC_DFACOLCNT];
	ulong ulColCnt = dfa.GetGroupCount();
	for (ulong i = 0; i < ulColCnt; ++i)
	{
		columns[i].key.clear();
	}

	CalcDfaColumnKeys(dfa, columns);
	ColumnHashGroup(columns, ulColCnt, pGroups);
}

void GroupNfaColumns(const CNfa &nfa, byte *pGroups)
{
	static COLUMNKEY columns[CNfaRow::COLUMNCNT];
	for (ulong i = 0; i < CNfaRow::COLUMNCNT; ++i)
	{
		columns[i].key.clear();
	}
	CalcNfaColumnKeys(nfa, columns);
	ColumnHashGroup(columns, CNfaRow::COLUMNCNT, pGroups);
}

void ReleaseAbleTo(PARTSET &ps)
{
	if (!ps.AbleTo.empty())
	{
		delete []ps.AbleTo.front();
	}
	ps.AbleTo.clear();
	ps.Ones.clear();
}

void CalcAbleTo(STATEIDARY *pRevTbl, ulong nGrpNum, ulong nStaNum, PARTSET &ps)
{
	//清空AbleTo
	ReleaseAbleTo(ps);

	byte *pBuf = null;
	try
	{
		pBuf = new byte[nStaNum * nGrpNum];
		ZeroMemory(pBuf, nStaNum * nGrpNum);
		ps.AbleTo.resize(nGrpNum, 0);
		ps.Ones.resize(nGrpNum, 0);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	//计算AbleTo的值，每产生一个新的或者更新PARTSET对象计算一次
	for (ulong j = 0; j < nGrpNum; ++j)
	{
		byte *pAbleTo = pBuf + j * nStaNum;
		/*AbleTo[j] = {state|state belong to parSet &&
		pRevTbl[state][j] is not null} */
		ps.AbleTo[j] = pAbleTo;
		//遍历PARTSET中的每个状态t，若存在δ(-1)(t,j)≠Φ，AbleTo[t]标记为1
		for (STATELIST_ITER k = ps.StaSet.begin(); k != ps.StaSet.end(); ++k)
		{
			bool br = !(pRevTbl[*k * nGrpNum + j].empty());
			if (br == true && pAbleTo[*k] == 0)
			{
				pAbleTo[*k] = (byte)br;
				++ps.Ones[j];
			}
		}
	}
}

/*
**	This function groups the merged dfa's columns on the ground of
**	the column groups of dfas to be merged.
**
**	We use a hash function to group.
**	
**	First of all, merge the dfas's column groups into a table of n*256, n means the size of dfas
**	Then hash the table columns to get the merged dfa's colums group.
*/
void DfaColGroup(CDfaArray &dfas, byte* groups)
{
	struct GROUPKEYHASH
	{
		inline ulong operator ()(const GROUPKEY &column)
		{
			return column.hash;
		}
	};
	typedef std::unordered_map<GROUPKEY, byte, GROUPKEYHASH> GROUPMAP;

	const ulong _FNV_offset_basis = 2166136261U;
	const ulong _FNV_prime = 16777619U;

	GROUPKEY colum[SC_DFACOLCNT];

	for(ulong c = 0; c < SC_DFACOLCNT; ++c)
	{
		colum[c].hash = _FNV_offset_basis;
	}

	try
	{
		for(ulong i = 0; i < dfas.Size(); ++i)
		{
			for(ulong c = 0; c < SC_DFACOLCNT; ++c)
			{
				ulong group = dfas[i].Char2Group(byte(c));
				colum[c].val.push_back(group);
				colum[c].hash ^= group;
				colum[c].hash *= _FNV_prime;
			}
		}

		GROUPMAP groupMap;
		for(ulong c = 0; c < SC_DFACOLCNT; ++c)
		{
			GROUPKEY &curCol = colum[c];
			GROUPMAP::iterator it = groupMap.find(curCol);
			if(it == groupMap.end())
			{
				ulong curId = groupMap.size();
				groups[c] = byte(curId);
				groupMap[curCol] = byte(curId);
			}
			else
			{
				groups[c] = it->second;
			}
		}
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

/*
**	this function marks the lastDfa's terminal states.
**	
**	@param otherSta		the state to be found in other
**	@param other		 one dfa to be merged
**	@param lastSta	  the state to be marked
**	@param lastDfa	  the merged dfa
**
**	@return a integer
**
**	@retval 0 function successful
**	@retval -1 fatal error
*/
void AddTermIntoDFA(STATEID otherSta, const CDfa &other,
					STATEID lastSta, CDfa &lastDfa)
{
	const CFinalStates &orgFinStas = other.GetFinalStates();
	CFinalStates &newFinStas = lastDfa.GetFinalStates();
	newFinStas.AddState(lastSta).Append(orgFinStas.GetDfaIdSet(otherSta));
}

void SetStateFlags(byte *pFlags, STATEIDARY states)
{
	for (STATEIDARY_ITER i = states.begin(); i != states.end(); ++i)
	{
		pFlags[*i] = 1;
	}
}

bool SortPartition(const byte *pAbleTo, PARTSET &partSet)
{
	try
	{
		STATELIST_ITER t = partSet.StaSet.begin();
		//将满足条件的t存在集合j的前段，不满足的存在集合j的后段
		//满足条件的t标记为1，先滤去pAbleToI中前段为1的值
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
		//将后段中出现的值为1的插入至前段
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
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
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
	try
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
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

void BuildDfaByPart(const PARTSETVEC &partSets, const DFAROWARY &oldDfa,
					DFAROWARY &newDfa)
{
	STATEIDARY old2New(oldDfa.size());
	for (PARTSETVEC_CITER i = partSets.cbegin(); i != partSets.cend(); ++i)
	{
		STATEID nSetIdx = STATEID(i - partSets.cbegin());
		for (STATELIST_CITER j = i->StaSet.cbegin(); j != i->StaSet.cend(); ++j)
		{
			old2New[*j] = nSetIdx;
		}
	}

	ulong ulColNum = oldDfa[0].Size();
	for (PARTSETVEC_CITER i = partSets.begin(); i != partSets.end(); ++i)
	{
		CDfaRow &newRow = newDfa[i - partSets.cbegin()];
		const CDfaRow &oldRow = oldDfa[i->StaSet.front()];
		for (ulong j = 0; j != ulColNum; ++j)
		{
			STATEID nCur = oldRow[byte(j)], nDest = STATEID(-1);
			if (nCur != STATEID(-1))
			{
				nDest = old2New[nCur];
			}
			newRow[byte(j)] = nDest;
		}
		//set a state attribute
		newRow.SetFlags(oldRow.GetFlags());
	}
}

void InitPartSet(const CFinalStates &finStas, ulong ulStaNum,
				 std::vector<PARTSET> &partSets)
{
	try
	{
		partSets.clear();
		CDfaIdSet *pTerm2Dfa = null;
		//distinguish final state belonging to different DFA
		pTerm2Dfa = new CDfaIdSet[ulStaNum];

		for (ulong i = 0; i < finStas.Size(); ++i)
		{
			STATEID nFinStaId = finStas[i];
			pTerm2Dfa[nFinStaId] = finStas.GetDfaIdSet(nFinStaId);
		}

		/*distinguish between final states and normal states，if map's first is null，
		then the PARTSET is the set of normal states,otherwise, it is final states.
		initBSet is unordered*/
		std::map<CDfaIdSet, PARTSET> initBSet;
		for (STATEID i = 0; i < ulStaNum; ++i)
		{
			CDfaIdSet &curDfaSet = pTerm2Dfa[i];
			initBSet[curDfaSet].StaSet.push_back(i);
		}
		delete []pTerm2Dfa;

		//set initBSet orderly and store in vector, among them, the last is normal states
		for (std::map<CDfaIdSet, PARTSET>::iterator i = initBSet.begin();
			i != initBSet.end(); ++i)
		{
			if (i->first.Size() != 0)
			{
				partSets.push_back(i->second);
			}
		}
		partSets.push_back(initBSet[CDfaIdSet()]);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}
