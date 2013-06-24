/*!
* @file			dfaalgo.h
* @author		Lab 435, Xidian University
* @brief		DFA的算法的函数声明
*/

#include <hwprj\common.h>
#include <hwprj\finalstates.h>
#include <hwprj\dfaarray.h>

class CDfa;
class CNfa;
typedef DFAROWARY::iterator			DFAROWARY_ITER;
typedef DFAROWARY::const_iterator	DFAROWARY_CITER;

/*!
* @brief 表示一个状态的化分的数据结构，仅供最小化算法使用。
*/
struct PARTSET
{
	STATELIST StaSet;
	std::vector<byte*> AbleTo;
	STATEIDARY Ones;
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
struct COLUMNKEY
{
	std::vector<ulong> key;
	ulong hash;
	inline bool operator == (const COLUMNKEY &other) const
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

struct COLUMNKEYHASH
{
	inline ulong operator ()(const COLUMNKEY &column)
	{
		return column.hash;
	}
};

struct GROUPKEY
{
	std::vector<ulong> val;
	ulong hash;
};

inline bool operator == (const GROUPKEY &key1, const GROUPKEY &key2)
{
	size_t nSize = key1.val.size();
	if (nSize != key2.val.size())
	{
		return false;
	}
	if (nSize == 0)
	{
		return true;
	}
	return (0 == memcmp(key1.val.data(), key2.val.data(),
		nSize * sizeof(key1.val[0])));
}

struct NSTATESET_HASH
{
	enum {MAX_SIZE = 20000};
	ulong operator()(const STATEIDARY &set)
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

void NfaEClosure(const CNfa &nfa, std::vector<STATEIDARY> &eClosure);

void GetNextEClosureSet(const CNfa &nfa, const std::vector<STATEIDARY> &eClosure,
	const STATEIDARY &curSet, ulong edge, STATEIDARY &eClosureSet);

void GroupNfaColumns(const CNfa &nfa, byte *pGroups);

void GroupDfaColumns(const CDfa &dfa, byte *pGroups);

void ReleaseAbleTo(PARTSET &ps);

void CalcAbleTo(STATEIDARY *pRevTbl, ulong nGrpNum, ulong nStaNum, PARTSET &ps);

//void DfaColGroup(std::vector<CDfa> &dfas, byte* groups);
/*!
* This function groups the merged dfa's columns on the ground of
* the column groups of dfas to be merged.
*/
void DfaColGroup(CDfaArray &dfas, byte* groups);

/*!	this function marks the lastDfa's terminal states. */
void AddTermIntoDFA(STATEID otherSta, const CDfa &other,
					STATEID lastSta, CDfa &lastDfa);

void SetStateFlags(byte *pFlags, STATEIDARY states);

bool SortPartition(const byte *pAbleTo, PARTSET &partSet);

ulong FindNotEmpty(const std::vector<ulong> *pVecAry, ulong nCnt);

void InitPartWait(const std::vector<PARTSET> &partSet,
				  std::vector<ulong> *pWait, ulong ulGrpNum);

void BuildDfaByPart(const PARTSETVEC &partSets, const DFAROWARY &oldDfa,
						 DFAROWARY &newDfa);

void InitPartSet(const CFinalStates &finStas, ulong ulStaNum,
				 std::vector<PARTSET> &partSets);
