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
#include <hwprj\common.h>
#include <hwprj\groupres.h>

#ifndef HASHMAPHDR_DS
#define HASHMAPHDR __declspec(dllimport)
#else
#define HASHMAPHDR __declspec(dllexport)
#endif

struct HASHNODE
{
	SIGNATURE m_sig;
	ulong m_nDfaId;
};

typedef std::map<ulong, std::vector<HASHNODE>> HASHRES;

/*	函数名：HashMapping
**	参数：groupRes-分组结果，HashResMap-哈希映射的结果
**	功能：每个分组均有能够代表该组的Signature，为Signature建立哈希表
*/
HASHMAPHDR void HashMapping(CGroupRes &groupRes, HASHRES &HashResMap);
