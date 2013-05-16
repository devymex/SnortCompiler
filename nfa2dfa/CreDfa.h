#pragma once
//#include "stdafx.h"

#include "../common/common.h"

//����ע�����ڲ���
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

typedef std::list<STATEID> STALIST;
typedef std::list<STATEID>::iterator STALIST_ITER;
typedef std::list<std::list<STATEID>> SETLIST;
typedef std::list<std::list<STATEID>>::iterator SETLIST_ITER;


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

void printNfa(CNfa oneNfaTab);

void printDfa(CDfa dfaTab);

void RemoveUnreachable(const std::vector<STATEID> *Tab, const STALIST &begs, const size_t &col, std::vector<STATEID> &reachable);

void MergeReachable(const CDfa &oneDfaTab, std::vector<STATEID> &reachable, CDfa &tmpDfa);

void PartitionNonDisState(CDfa &tmpDfa, std::vector<STATEID> *pRevTbl, SETLIST &pSets);

void MergeNonDisStates(CDfa &tmpDfa, SETLIST &Partition, CDfa &minDfaTab);
