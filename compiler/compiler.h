#include "../common/common.h"
#include <windows.h>

#ifndef COMPILER_H_
#define COMPILER __declspec(dllimport)
#else
#define COMPILER __declspec(dllexport)
#endif

class COMPILER SNORTIDDFAIDS
{
	size_t m_nSid;
	CVectorNumber m_dfaIds;
};

class COMPILER CRes
{
	std::vector<CDfa> m_dfaTbl;
	std::vector<SNORTIDDFAIDS> m_sidDfaIds;
	CVectorNumber m_errorIds;
	CVectorNumber m_exceedIds;
	CVectorNumber m_hasbyteIds;
	CVectorNumber m_hasnotIds;
	CVectorNumber m_emptyIds;
};

COMPILER void compile(LPCTSTR filename, RES &result);