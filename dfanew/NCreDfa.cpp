#include "stdafx.h"
#include "NCreDfa.h"
#include "dfanew.h"

struct NFAELEM
{
	size_t* pBuf;
	size_t nSize;
	__forceinline bool operator ==(const NFAELEM &other) const
	{
		if (nSize != other.nSize)
		{
			return false;
		}
		if (nSize > 0)
		{
			return (0 == memcmp(pBuf, other.pBuf, nSize));
		}
		return true;
	}
};

struct NFACOLUMN
{
	std::vector<NFAELEM> elems;
	size_t nTotal;
	size_t nHash;
	bool operator==(const NFACOLUMN &other) const
	{
		if (nTotal != other.nTotal)
		{
			return false;
		}
		return (elems == other.elems);	
	}
};

struct NFACOLUMNHASH
{
	size_t operator()(const NFACOLUMN &column)
	{
		return column.nHash;
	}
};

void CalcKeyHash(NFACOLUMN &column)
{
	const size_t _FNV_offset_basis = 2166136261U;
	const size_t _FNV_prime = 16777619U;
	column.nHash = _FNV_offset_basis;
	column.nTotal = 0;
	for (std::vector<NFAELEM>::const_iterator i = column.elems.cbegin();
		i != column.elems.cend(); ++i)
	{
		size_t nSize = i->nSize;
		column.nTotal += nSize;
		column.nHash ^= nSize;
		column.nHash *= _FNV_prime;
		for (size_t j = 0; j < nSize; ++j)
		{	
			column.nHash ^= i->pBuf[j];
			column.nHash *= _FNV_prime;
		}
	}
}

typedef std::unordered_map<NFACOLUMN, STATEID, NFACOLUMNHASH> GROUPHASH;

void NAvaiEdges(const CNfa &nfa, STATEID *group)
{
	static NFACOLUMN keys[CHARSETSIZE];

	size_t nSize = nfa.Size();
	for (size_t col = 0; col < CHARSETSIZE; ++col)
	{
		NFACOLUMN &curCol = keys[col];
		curCol.elems.resize(nSize);
		for (size_t i = 0; i < nSize; ++i)
		{
			const CNfaRow &curRow = nfa[i];
			NFAELEM &curElem = curCol.elems[i];
			curElem.pBuf = (size_t*)curRow.GetCol(col);
			curElem.nSize = curRow.DestCnt(col);
		}
		CalcKeyHash(curCol);
	}

	GROUPHASH gpHash;
	gpHash.rehash(991);
	for(size_t c = 0; c < DFACOLSIZE; ++c)
	{
		NFACOLUMN &curCol = keys[c];
		GROUPHASH::iterator colIt = gpHash.find(curCol);
		if(colIt == gpHash.end())
		{
			STATEID curId = gpHash.size();
			gpHash[curCol] = curId;
			group[c] = curId;
		}
		else
		{
			group[c] = colIt->second;
		}
	}
}
