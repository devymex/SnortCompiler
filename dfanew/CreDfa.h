#pragma once
//#include "stdafx.h"

#include "../common/common.h"
#include "dfanew.h"

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
			}
		}
		return nr % MAX_SIZE;
	}
};

void AvaiEdges(CNfa &oneNfaTab, BYTE *group);

void NextNfaSet(const CNfa &oneNfaTab, const std::vector<size_t> &curNfaVec, size_t edge,
				std::vector<size_t> &nextENfaVec, char &finFlag);

void EClosure(const CNfa &oneNfaTab, const std::vector<size_t> &curNfaVec, 
			  std::vector<size_t> &eNfaVec, char &finFlag);

void RemoveUnreachable(const std::vector<STATEID> *Tab, const STALIST &begs, 
					   const size_t &col, std::vector<BYTE> &reachable);

void PartitionNonDisState(const size_t &groupnum, const size_t &size, std::vector<STATEID> *pRevTbl, SETLIST &pSets);

