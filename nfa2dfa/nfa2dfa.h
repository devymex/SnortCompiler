#pragma once
//#include "stdafx.h"

#include "../common/common.h"

#define HASHMODULO 1000000

#define SC_STATELIMIT 254

#define EMPTY 256

#ifndef CREDFA_H_
#define CREDFA __declspec(dllimport)
#else
#define CREDFA __declspec(dllexport)
#endif

template <typename _Iter>
STATEID MatchDfa(CDfa &dfaTab, _Iter _beg, _Iter _end)
{
	STATEID curSta = 0;
	for(_Iter iter = _beg; iter != _end && curSta != (STATEID)-1; ++iter)
	{
		STATEID group = dfaTab.GetGroup(*iter);
		curSta = dfaTab[curSta][group];
	}

	if(curSta == (STATEID)-1 || (dfaTab[curSta].GetFlag() & CDfaRow::TERMINAL) == 0)
	{
		curSta = (STATEID)-1;
	}
	return curSta;
}

CREDFA size_t NfaToDfa(CNfa &oneNfaTab,CDfa &dfaTab, bool combine = false);
CREDFA size_t DfaMin(CDfa &oneDfaTab, CDfa &minDfaTab);
