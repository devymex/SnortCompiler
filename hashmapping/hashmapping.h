#pragma once
#include "../common/common.h"

#ifndef HASHMAPPING_H_
#define HASHMAPPINGSC __declspec(dllimport)
#else
#define HASHMAPPINGSC __declspec(dllexport)
#endif

struct GROUPHASH
{
	std::vector<SIGNATURE> vecSigs;
	SIGNATURE currSig;
	std::size_t dfaId;
};

HASHMAPPINGSC void Mapping();
