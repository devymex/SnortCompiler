#include "stdafx.h"
#include "NCreDfa.h"
#include "dfanew.h"

struct ELEMS
{
	size_t* pBuf;
	size_t nSize;
	bool operator ==(const ELEMS &other) const
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

typedef std::vector<ELEMS> COLKEY;

struct COLHASH
{
	size_t operator()(const COLKEY &key)
	{
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;
		size_t _Val = _FNV_offset_basis;
		for (COLKEY::const_iterator i = key.cbegin(); i != key.cend(); ++i)
		{
			_Val ^= i->nSize;
			_Val *= _FNV_prime;
			for (size_t j = 0; j < i->nSize; ++j)
			{	
				_Val ^= i->pBuf[j];
				_Val *= _FNV_prime;
			}
		}
		return _Val;
	}
};

typedef std::unordered_map<COLKEY, STATEID, COLHASH> GROUPHASH;

void NAvaiEdges(const CNfa &nfa, STATEID *group)
{
	std::vector<COLKEY> keys(CHARSETSIZE);

	size_t nSize = nfa.Size();
	for (size_t col = 0; col < CHARSETSIZE; ++col)
	{
		COLKEY &curCol = keys[col];
		curCol.resize(nSize);
		for (size_t i = 0; i < nSize; ++i)
		{
			const CNfaRow &curRow = nfa[i];
			ELEMS &curElem = curCol[i];
			curElem.pBuf = (size_t*)curRow.GetCol(col);
			curElem.nSize = curRow.DestCnt(col);
		}
	}

	GROUPHASH gpHash;
	for(size_t c = 0; c < DFACOLSIZE; ++c)
	{
		COLKEY &curKey = keys[c];
		GROUPHASH::iterator colIt = gpHash.find(curKey);
		if(colIt == gpHash.end())
		{
			STATEID curId = gpHash.size();
			gpHash[curKey] = curId;
			group[c] = curId;
		}
		else
		{
			group[c] = colIt->second;
		}
	}
}
