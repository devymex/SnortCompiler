#pragma once
//#include "stdafx.h"

#include "../common/common.h"

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

void AvaiEdges(CNfa &oneNfaTab, size_t *group);

void NextNfaSet(const CNfa &oneNfaTab, const std::vector<size_t> &curNfaVec,
				size_t edge, std::vector<size_t> &nextENfaVec, char &finFlag);

void EClosure(const CNfa &oneNfaTab, const std::vector<size_t> &curNfaVec, std::vector<size_t> &eNfaVec, char &finFlag);

void printNfa(CNfa oneNfaTab);

void printDfa(CDfa dfaTab);
