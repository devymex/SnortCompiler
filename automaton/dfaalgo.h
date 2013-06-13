/*
**	@file		dfaalgo.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Functions declaration of dfa's algorithms
**
**	Includes empty closure etc.
**
*/

#include <hwprj\common.h>
#include <hwprj\finalstates.h>

class CDfa;
class CNfa;
typedef DFAROWARY::iterator			DFAROWARY_ITER;
typedef DFAROWARY::const_iterator	DFAROWARY_CITER;

struct PARTSET
{
	STATELIST StaSet;
	std::vector<byte*> AbleTo;
	STATEVEC Ones;
};

typedef std::vector<PARTSET>		PARTSETVEC;
typedef PARTSETVEC::iterator		PARTSETVEC_ITER;
typedef PARTSETVEC::const_iterator	PARTSETVEC_CITER;

struct TODFA_HASH
{
	ulong operator()(const std::vector<STATEID> &vec)
	{
		const ulong _FNV_offset_basis = 2166136261U;
		const ulong _FNV_prime = 16777619U;

		ulong _Val = _FNV_offset_basis;
		for (ulong _Next = 0; _Next < vec.size(); ++_Next)
		{	
			_Val ^= vec[_Next];
			_Val *= _FNV_prime;
		}

		return (_Val);
	}
};

struct GROUPKEY
{
	std::vector<ulong> key;
	ulong hash;
	__forceinline bool operator == (const GROUPKEY &other) const
	{
		ulong nSize = key.size();
		if (nSize != other.key.size())
		{
			return false;
		}
		if (nSize == 0)
		{
			return true;
		}
		return (0 == memcmp(key.data(), other.key.data(), nSize * sizeof(ulong)));
	}
};

struct NSTATESET_HASH
{
	enum {MAX_SIZE = 20000};
	ulong operator()(const STATEVEC &set)
	{
		const ulong _FNV_offset_basis = 2166136261U;
		const ulong _FNV_prime = 16777619U;

		const STATEID *pData = set.data();
		const STATEID *pEnd = pData + set.size();

		ulong nr = _FNV_offset_basis;
		for (; pData != pEnd; ++pData)
		{
			nr ^= *pData;
			nr *= _FNV_prime;
		}
		return nr;
	}
};

template<typename _Ty>
void WriteNum(byte*& pBuf, _Ty _num, ulong nBytes = sizeof(_Ty))
{
	CopyMemory(pBuf, &_num, nBytes);
	pBuf += nBytes;
}

template<typename _Ty>
void ReadNum(byte*& pBuf, _Ty &_num, ulong nBytes = sizeof(_Ty))
{
	CopyMemory(&_num, pBuf, nBytes);
	pBuf += nBytes;
}

void Warshall(byte *pMat, ulong nWidth, ulong nHeight);

void NfaEClosure(const CNfa &nfa, std::vector<STATEVEC> &eClosure);

void GetNextEClosureSet(const CNfa &nfa, const std::vector<STATEVEC> &eClosure,
	const STATEVEC &curSet, ulong edge, STATEVEC &eClosureSet);

void NAvaiEdges(const CNfa &nfa, byte *group);

void ReleaseAbleTo(PARTSET &ps);

void CalcAbleTo(STATEVEC *pRevTbl, ulong nGrpNum, ulong nStaNum, PARTSET &ps);

/*
**	This function groups the merged dfa's columns on the ground of
**	the column groups of dfas to be merged.
*/
void DfaColGroup(std::vector<CDfa> &dfas, byte* groups);

/*	this function marks the lastDfa's terminal states. */
void AddTermIntoDFA(STATEID otherSta, const CDfa &other,
					STATEID lastSta, CDfa &lastDfa);

void SetStateFlags(byte *pFlags, STATEVEC states);

bool SortPartition(const byte *pAbleTo, PARTSET &partSet);

ulong FindNotEmpty(const std::vector<ulong> *pVecAry, ulong nCnt);

void InitPartWait(const std::vector<PARTSET> &partSet,
				  std::vector<ulong> *pWait, ulong ulGrpNum);

void BuildDfaByPart(const PARTSETVEC &partSets, const DFAROWARY &oldDfa,
						 DFAROWARY &newDfa);

void InitPartSet(const CFinalStates &finStas, ulong ulStaNum,
				 std::vector<PARTSET> &partSets);
