#pragma once
#include <map>
#include <vector>
#include <unordered_map>
#include "../common/common.h"
#include "../grouping/grouping.h"

#ifndef HASHMAPPING_H_
#define HASHMAPPINGSC __declspec(dllimport)
#else
#define HASHMAPPINGSC __declspec(dllexport)
#endif

typedef std::map<SIGNATURE, std::vector<size_t>> SIGNATUREMAP;
typedef std::map<size_t, std::vector<SIGNATURE>> IDMAP;
typedef std::unordered_map<size_t, std::vector<size_t>> RESULTMAP;

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
	std::vector<size_t> vecDfaIds;
};

HASHMAPPINGSC size_t hash(const SIGNATURE &oneSig);

HASHMAPPINGSC void HashMapping(CGROUPRes &groupRes, HASHRES &HashResMap);
