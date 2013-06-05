#pragma once
#include <map>
#include <vector>
#include "../common/common.h"
#include "../grouping/grouping.h"

#ifndef HASHMAPPING_H_
#define HASHMAPPINGSC __declspec(dllimport)
#else
#define HASHMAPPINGSC __declspec(dllexport)
#endif

typedef std::map<SIGNATURE, std::vector<size_t>> SIGNATUREMAP;
typedef std::map<size_t, std::vector<SIGNATURE>> IDMAP;
typedef std::map<size_t, std::vector<size_t>> RESULTMAP;

struct HASHNODE
{
	HASHNODE(const SIGNATURE &sig, const size_t nDfaId) : m_sig(sig), m_nDfaId(nDfaId) {}
	SIGNATURE m_sig;
	std::size_t m_nDfaId;
};

typedef std::map<size_t, std::vector<HASHNODE>> HASHRES;

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

HASHMAPPINGSC size_t hash(const SIGNATURE &oneSig);

HASHMAPPINGSC void HashMapping(const CGROUPRes &groupRes, HASHRES &HashResMap);
