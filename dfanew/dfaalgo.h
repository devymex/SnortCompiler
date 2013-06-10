#include "../common/common.h"

typedef std::vector<size_t> STATESET;
class CDfaNew;

struct PARTSET
{
	STATELIST StaSet;
	std::vector<BYTE*> AbleTo;
	STATEVEC Ones;
};

struct TODFA_HASH
{
	size_t operator()(const std::vector<STATEID> &vec)
	{
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;

		size_t _Val = _FNV_offset_basis;
		for (size_t _Next = 0; _Next < vec.size(); ++_Next)
		{	
			_Val ^= vec[_Next];
			_Val *= _FNV_prime;
		}

		return (_Val);
	}
};

struct GROUPKEY
{
	std::vector<size_t> key;
	size_t hash;
	__forceinline bool operator == (const GROUPKEY &other) const
	{
		size_t nSize = key.size();
		if (nSize != other.key.size())
		{
			return false;
		}
		if (nSize == 0)
		{
			return true;
		}
		return (0 == memcmp(key.data(), other.key.data(), nSize * sizeof(size_t)));
	}
};

void Warshall(BYTE *pMat, size_t nWidth, size_t nHeight);

void NfaEClosure(const CNfa &nfa, std::vector<STATESET> &eClosure);

void GetNextEClosureSet(const CNfa &nfa, const std::vector<STATESET> &eClosure,
	const STATESET &curSet, size_t edge, STATESET &eClosureSet);

void NAvaiEdges(const CNfa &nfa, BYTE *group);

void ReleaseAbleTo(PARTSET &ps);

void CalcAbleTo(STATEVEC *pRevTbl, size_t nGrpNum, size_t nStaNum, PARTSET &ps);

template<typename _Ty>
void WriteNum(BYTE*& pBuf, _Ty _num, size_t nBytes = sizeof(_Ty))
{
	CopyMemory(pBuf, &_num, nBytes);
	pBuf += nBytes;
}

template<typename _Ty>
void ReadNum(BYTE*& pBuf, _Ty &_num, size_t nBytes = sizeof(_Ty))
{
	CopyMemory(&_num, pBuf, nBytes);
	pBuf += nBytes;
}

/*
**	This function groups the merged dfa's columns on the ground of
**	the column groups of dfas to be merged.
*/
void DfaColGroup(std::vector<CDfaNew> &dfas, BYTE* groups);

/*	this function marks the lastDfa's terminal states. */
void AddTermIntoDFA(STATEID otherSta, const CDfaNew &other,
					STATEID lastSta, CDfaNew &lastDfa);
