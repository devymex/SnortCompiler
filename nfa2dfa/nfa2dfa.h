#pragma once
//#include "stdafx.h"

#include "../common/common.h"

#define HASHMODULO 1000000

#define EMPTY 256

#ifndef CREDFA_H_
#define CREDFA __declspec(dllimport)
#else
#define CREDFA __declspec(dllexport)
#endif

template <typename _Iter>
size_t MatchDfa(CDfa &dfaTab, _Iter _beg, _Iter _end)
{
	size_t curSta = 0;
	for(_Iter iter = _beg; iter != _end && curSta != (size_t)-1; ++iter)
	{
		curSta = dfaTab[curSta][*iter];
	}

	if(curSta == (size_t)-1 || (dfaTab[curSta].GetFlag() & CDfaRow::TERMINAL) == 0)
	{
		curSta = (size_t)-1;
	}
	return curSta;
}

CREDFA void NfaToDfa(CNfa &oneNfaTab, CDfa &dfaTab);
CREDFA size_t DfaMin(CDfa &oneDfaTab, CDfa &minDfaTab);
