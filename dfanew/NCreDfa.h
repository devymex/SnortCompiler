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

struct NSTATESET_HASH
{
	enum {MAX_SIZE = 20000};
	size_t operator()(const std::vector<size_t> &set)
	{
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;

		const size_t *pData = set.data();
		const size_t *pEnd = pData + set.size();

		size_t nr = 2166136261U;
		for (; pData != pEnd; ++pData)
		{
			nr ^= *pData;
			nr *= _FNV_prime;
		}
		return nr;
	}
};

DFANEWSC void NAvaiEdges(const CNfa &oneNfaTab, BYTE *group);
