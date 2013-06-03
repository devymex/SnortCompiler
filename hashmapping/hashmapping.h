#pragma once
#include <map>
#include <vector>
#include "../common/common.h"

#ifndef HASHMAPPING_H_
#define HASHMAPPINGSC __declspec(dllimport)
#else
#define HASHMAPPINGSC __declspec(dllexport)
#endif

typedef std::map<SIGNATURE, std::vector<size_t>> SIGNATUREMAP;
typedef std::map<size_t, std::vector<SIGNATURE>> IDMAP;
typedef std::map<size_t, std::vector<size_t>> RESULTMAP;

struct GROUPHASH
{
	std::vector<SIGNATURE> vecSigs;
	SIGNATURE currSig;
	std::size_t mergeDfaId;
};

struct COMP
{
	bool operator()(const GROUPHASH &g1, const GROUPHASH &g2)
	{
		return g1.vecSigs.size() < g2.vecSigs.size();
	}
};

HASHMAPPINGSC void HashMapping();
