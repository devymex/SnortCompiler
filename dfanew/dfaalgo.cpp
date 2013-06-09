#include "stdafx.h"
#include "dfanew.h"
#include "dfaalgo.h"

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
		size_t nCnt = row.DestCnt(DFACOLSIZE);
		const size_t *pDest = row.GetCol(DFACOLSIZE);
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

	static STATESET nextSet; //
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

void NAvaiEdges(const CNfa &nfa, BYTE *group)
{
	struct COLUMNKEY
	{
		std::vector<size_t> key;
		size_t hash;
		__forceinline bool operator == (const COLUMNKEY &other) const
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
	struct COLUMNKEYHASH
	{
		__forceinline size_t operator ()(const COLUMNKEY &column)
		{
			return column.hash;
		}
	};
	typedef std::unordered_map<COLUMNKEY, BYTE, COLUMNKEYHASH> COLUMNHASHMAP;
	const size_t _FNV_offset_basis = 2166136261U;
	const size_t _FNV_prime = 16777619U;

	static COLUMNKEY columns[CHARSETSIZE]; //
	size_t zeroCnts[CHARSETSIZE] = {0};

	size_t nSize = nfa.Size();
	for (size_t i = 0; i < CHARSETSIZE; ++i)
	{
		columns[i].key.clear();
		columns[i].hash = _FNV_offset_basis;
	}
	for (size_t i = 0; i < nSize; ++i)
	{
		const CNfaRow &curRow = nfa[i];
		for (size_t j = 0; j < CHARSETSIZE; ++j)
		{
			std::vector<size_t> &curCol = columns[j].key;
			const size_t *pData = curRow.GetCol(j);
			size_t nAddSize = curRow.DestCnt(j);
			if (nAddSize != 0)
			{
				if (zeroCnts[j] > 0)
				{
					curCol.resize(curCol.size() + zeroCnts[j], 0);
					zeroCnts[j] = 0;
				}
				curCol.push_back(nAddSize);
			}
			else
			{
				++zeroCnts[j];
			}

			size_t &hash = columns[j].hash;
			hash ^= nAddSize;
			hash *= _FNV_prime;
			for (size_t k = 0; k < nAddSize; ++k)
			{
				curCol.push_back(pData[k]);
				hash ^= pData[k];
				hash *= _FNV_prime;
			}
		}
	}
	for (size_t i = 0; i < CHARSETSIZE; ++i)
	{
		std::vector<size_t> &curCol = columns[i].key;
		size_t nCurSize = curCol.size();
		curCol.resize(nCurSize + zeroCnts[i]);
		memset(curCol.data() + nCurSize, 0, zeroCnts[i] * sizeof(size_t));
	}

	static COLUMNHASHMAP colHash; //
	colHash.clear();
	for(size_t i = 0; i < DFACOLSIZE; ++i)
	{
		COLUMNKEY &curCol = columns[i];
		COLUMNHASHMAP::iterator same = colHash.find(curCol);
		if(same == colHash.end())
		{
			BYTE curId = (BYTE)colHash.size();
			colHash[curCol] = curId;
			group[i] = curId;
		}
		else
		{
			group[i] = same->second;
		}
	}
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

void CalcAbleTo(STATEVEC *pRevTbl, size_t nGrpNum, size_t nStaNum, PARTSET &ps)
{
	//清空AbleTo
	ReleaseAbleTo(ps);

	BYTE *pBuf = new BYTE[nStaNum * nGrpNum];
	ZeroMemory(pBuf, nStaNum * nGrpNum);
	ps.AbleTo.resize(nGrpNum, 0);
	ps.Ones.resize(nGrpNum, 0);
	//计算AbleTo的值，每产生一个新的或者更新PARTSET对象计算一次
	for (size_t j = 0; j < nGrpNum; ++j)
	{
		BYTE *pAbleTo = pBuf + j * nStaNum;
		ps.AbleTo[j] = pAbleTo;
		//遍历PARTSET中的每个状态t，若存在δ(-1)(t,j)≠Φ，AbleTo[t]标记为1
		for (STATELIST_ITER k = ps.StaSet.begin(); k != ps.StaSet.end(); ++k)
		{
			BOOL br = !(pRevTbl[*k * nGrpNum + j].empty());
			if (br == TRUE && pAbleTo[*k] == 0)
			{
				pAbleTo[*k] = (BYTE)br;
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
void DfaColGroup(std::vector<CDfaNew> &dfas, BYTE* groups)
{
	struct GROUPKEYHASH
	{
		__forceinline size_t operator ()(const GROUPKEY &column)
		{
			return column.hash;
		}
	};

	const size_t _FNV_offset_basis = 2166136261U;
	const size_t _FNV_prime = 16777619U;

	GROUPKEY colum[CHARSETSIZE];
	typedef std::unordered_map<GROUPKEY, BYTE, GROUPKEYHASH> GROUPMAP;;
	GROUPMAP groupMap;

	for(size_t c = 0; c < DFACOLSIZE; ++c)
	{
		colum[c].key.clear();
		colum[c].hash = _FNV_offset_basis;
	}

	for(size_t i = 0; i < dfas.size(); ++i)
	{
		for(size_t c = 0; c < DFACOLSIZE; ++c)
		{
			size_t group = dfas[i].Char2Group(BYTE(c));
			colum[c].key.push_back(group);
			colum[c].hash ^= group;
			colum[c].hash *= _FNV_prime;
		}
	}

	for(size_t c = 0; c < DFACOLSIZE; ++c)
	{
		GROUPKEY &curCol = colum[c];
		GROUPMAP::iterator it = groupMap.find(curCol);
		if(it == groupMap.end())
		{
			size_t curId = groupMap.size();
			groups[c] = BYTE(curId);
			groupMap[curCol] = BYTE(curId);
		}
		else
		{
			groups[c] = it->second;
		}
	}
}

/*
**	this function marks the lastDfa's terminal states.
**	
**  @param otherSta	   the state to be found in other
**  @param other       one dfa to be merged
**	@param lastSta     the state to be marked
**	@param lastDfa     the merged dfa
**
**  @return a integer
**
**  @retval 0 function successful
**  @retval -1 fatal error
*/
void AddTermIntoDFA(STATEID otherSta, const CDfaNew &other,
					STATEID lastSta, CDfaNew &lastDfa)
{
	const CFinalStates &orgFinStas = other.GetFinalState();
	CFinalStates &newFinStas = lastDfa.GetFinalState();
	newFinStas.PushBack(lastSta);
	const std::set<size_t> &finSet = orgFinStas._GetDfaIds(otherSta);
	newFinStas._GetDfaIds(lastSta).insert(finSet.begin(), finSet.end());
}
