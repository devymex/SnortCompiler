

#pragma once

#include "stdafx.h"

#ifndef	MULTILEVELHASH_DS
#define MULTILEVELHASH __declspec(dllimport)
#else
#define MULTILEVELHASH __declspec(dllexport)
#endif


typedef struct 
{
	ulong sig;
	ulong dfaId;

}SIGDFAID;


typedef std::vector<SIGDFAID> SIGDFAIDARRAY;
typedef std::vector<SIGDFAIDARRAY> HASHTABLE;
typedef std::vector<HASHTABLE> MLEVELHASH;


class MULTILEVELHASH CMultiLevelHash
{
public:

	CMultiLevelHash();

	void Init(const CGroupRes &groupRes);

	void MatchOneSig(const ulong &sig, CUnsignedArray &matchResultArr);

	int GetMin(int i);

	virtual ~CMultiLevelHash();

private:

	static const ulong hashDeepth = 4;

	ulong dfaSize;

	MLEVELHASH mLevelHash;

	CUnsignedArray minSig;
};


