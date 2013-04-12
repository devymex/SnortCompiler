#pragma once
//#include "stdafx.h"

#include "../SnortCommon/snortcommon.h"

#define HASHMODULO 1000000

#define EMPTY 256


//以下注释用于测试
//enum STATEFLAG
//{
//	NORMAL   = 1 << 0,
//	START    = 1 << 1,
//	TERMINAL = 1 << 2
//};
//
//
//struct DFAROW
//{
//	size_t nSize;
//	unsigned long nFlag;
//	size_t destState[CHARSETSIZE];
//};

struct STATESET_HASH
{
	enum {MAX_SIZE = 20000};
	size_t operator()(const std::vector<size_t> &set)
	{
		size_t nr = set.size() + 1;
		if (set.size() > 0)
		{
			std::vector<size_t>::const_reverse_iterator rbeg = set.crbegin();
			nr *= (*rbeg++) + 1;
			if (set.size() > 1)
			{
				nr *= (*rbeg++) + 1;
			//	if (set.size() > 2)
			//	{
			//		nr *= (*rbeg++) + 1;
			//	}
			}
		}
		return nr % MAX_SIZE;
	}
};

//typedef std::map<std::vector<size_t>, size_t> NFATODFAMAP;

//typedef std::map<unsigned int, size_t> NFATODFAMAP;
//


typedef std::vector<size_t> VS;
typedef std::vector<VS> DVS;

void ReadFile(std::string &infilename, std::vector<std::string> &vecPcres, std::vector<CNfa> &Tables);

void NfaToDfa(CNfa &oneNfaTab, CDfa &dfaTab);

void NextNfaSet(const CNfa &oneNfaTab, const std::vector<size_t> &curNfaVec,
				size_t edge, std::vector<size_t> &nextENfaVec, char &finFlag);

void EClosure(const CNfa &oneNfaTab, const std::vector<size_t> &curNfaVec, std::vector<size_t> &eNfaVec, char &finFlag);

void printNfa(CNfa oneNfaTab);

void printDfa(CDfa dfaTab);

void PartitionNonDisState(CDfa &dfa, DVS &partition_states, VS final_states);

void MergeNonDisStates(CDfa &dfa, DVS &partition_states, CDfa &newdfa);

void AvaiEdges(CNfa &oneNfaTab, std::vector<size_t> &avaiVec);

template <typename _Iter>
size_t MatchDfa(CDfa &dfaTab, _Iter _beg, _Iter _end)
{
	size_t curSta = 0;
	for(_Iter iter = _beg; iter != _end && curSta != (size_t)-1; ++iter)
	{
		curSta = dfaTab[curSta].destState[*iter];
	}

	if(curSta == (size_t)-1 || (dfaTab[curSta].nFlag & TERMINAL) == 0)
	{
		curSta = (size_t)-1;
	}
	return curSta;
}

struct COMPARE
{
	bool operator()(VS &x, VS &y)
	{
		 return x.front() < y.front();
	}
};
