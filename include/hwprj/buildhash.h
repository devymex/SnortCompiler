/*
**	@file		buildhash.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of GROUPHASH class
**
**	Store mapping between grouping result and hash table
**	Merge CDfa
**
*/

#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <hwprj\common.h>
#include <hwprj\groupres.h>

#ifndef HASHMAPHDR_DS
#define HASHMAPHDR __declspec(dllimport)
#else
#define HASHMAPHDR __declspec(dllexport)
#endif

typedef std::map<SIGNATURE, std::vector<ulong>> SIGNATUREMAP;
typedef std::map<ulong, std::vector<SIGNATURE>> IDMAP;
typedef std::unordered_map<ulong, std::vector<ulong>> RESULTMAP;

struct HASHNODE
{
	HASHNODE(const SIGNATURE &sig, const ulong nDfaId) : m_sig(sig), m_nDfaId(nDfaId) {}
	SIGNATURE m_sig;
	ulong m_nDfaId;
};

typedef std::map<ulong, std::vector<HASHNODE>> HASHRES;

struct GROUPHASH
{
	std::vector<SIGNATURE> vecSigs;
	SIGNATURE currSig;
	ulong mergeDfaId;
	std::vector<ulong> vecDfaIds;
};

HASHMAPHDR ulong hash(const SIGNATURE &oneSig);

HASHMAPHDR void HashMapping(CGroupRes &groupRes, HASHRES &HashResMap);
