#pragma once
#include <map>
#include <vector>
#include <unordered_map>
#include <hwprj\grouping.h>

#ifndef HASHMAPHDR_DS
#define HASHMAPHDR __declspec(dllimport)
#else
#define HASHMAPHDR __declspec(dllexport)
#endif

typedef std::map<SIGNATURE, std::vector<ULONG>> SIGNATUREMAP;
typedef std::map<ULONG, std::vector<SIGNATURE>> IDMAP;
typedef std::unordered_map<ULONG, std::vector<ULONG>> RESULTMAP;

struct HASHNODE
{
	HASHNODE(const SIGNATURE &sig, const ULONG nDfaId) : m_sig(sig), m_nDfaId(nDfaId) {}
	SIGNATURE m_sig;
	ULONG m_nDfaId;
};

typedef std::map<ULONG, std::vector<HASHNODE>> HASHRES;

struct GROUPHASH
{
	std::vector<SIGNATURE> vecSigs;
	SIGNATURE currSig;
	ULONG mergeDfaId;
	std::vector<ULONG> vecDfaIds;
};

HASHMAPHDR ULONG hash(const SIGNATURE &oneSig);

HASHMAPHDR void HashMapping(CGROUPRes &groupRes, HASHRES &HashResMap);
