#include "../common/common.h"

typedef std::vector<size_t> STATESET;

struct PARTSET
{
	STATELIST StaSet;
	std::vector<BYTE*> AbleTo;
	STATEVEC Ones;
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
