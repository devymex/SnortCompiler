#include "stdafx.h"
#include "NCreDfa.h"
#include "dfanew.h"

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

DFANEWSC void NAvaiEdges(const CNfa &nfa, BYTE *group)
{
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

	COLUMNKEY columns[CHARSETSIZE];
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
			size_t nCurSize = curCol.size();
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

	COLUMNHASHMAP colHash;
	for(size_t i = 0; i < DFACOLSIZE; ++i)
	{
		COLUMNKEY &curCol = columns[i];
		COLUMNHASHMAP::iterator same = colHash.find(curCol);
		if(same == colHash.end())
		{
			BYTE curId = colHash.size();
			colHash[curCol] = curId;
			group[i] = curId;
		}
		else
		{
			group[i] = same->second;
		}
	}
}
