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

#include <hwprj\common.h>

typedef std::vector<ULONG> STATESET;
class CDfa;

struct PARTSET
{
	STATELIST StaSet;
	std::vector<BYTE*> AbleTo;
	STATEVEC Ones;
};

struct TODFA_HASH
{
	ULONG operator()(const std::vector<STATEID> &vec)
	{
		const ULONG _FNV_offset_basis = 2166136261U;
		const ULONG _FNV_prime = 16777619U;

		ULONG _Val = _FNV_offset_basis;
		for (ULONG _Next = 0; _Next < vec.size(); ++_Next)
		{	
			_Val ^= vec[_Next];
			_Val *= _FNV_prime;
		}

		return (_Val);
	}
};

struct GROUPKEY
{
	std::vector<ULONG> key;
	ULONG hash;
	__forceinline bool operator == (const GROUPKEY &other) const
	{
		ULONG nSize = key.size();
		if (nSize != other.key.size())
		{
			return false;
		}
		if (nSize == 0)
		{
			return true;
		}
		return (0 == memcmp(key.data(), other.key.data(), nSize * sizeof(ULONG)));
	}
};

void Warshall(BYTE *pMat, ULONG nWidth, ULONG nHeight);

void NfaEClosure(const CNfa &nfa, std::vector<STATESET> &eClosure);

void GetNextEClosureSet(const CNfa &nfa, const std::vector<STATESET> &eClosure,
	const STATESET &curSet, ULONG edge, STATESET &eClosureSet);

void NAvaiEdges(const CNfa &nfa, BYTE *group);

void ReleaseAbleTo(PARTSET &ps);

void CalcAbleTo(STATEVEC *pRevTbl, ULONG nGrpNum, ULONG nStaNum, PARTSET &ps);

template<typename _Ty>
void WriteNum(BYTE*& pBuf, _Ty _num, ULONG nBytes = sizeof(_Ty))
{
	CopyMemory(pBuf, &_num, nBytes);
	pBuf += nBytes;
}

template<typename _Ty>
void ReadNum(BYTE*& pBuf, _Ty &_num, ULONG nBytes = sizeof(_Ty))
{
	CopyMemory(&_num, pBuf, nBytes);
	pBuf += nBytes;
}

/*
**	This function groups the merged dfa's columns on the ground of
**	the column groups of dfas to be merged.
*/
void DfaColGroup(std::vector<CDfa> &dfas, BYTE* groups);

/*	this function marks the lastDfa's terminal states. */
void AddTermIntoDFA(STATEID otherSta, const CDfa &other,
					STATEID lastSta, CDfa &lastDfa);
