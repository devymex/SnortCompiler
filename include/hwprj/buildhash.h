/*!
* @file			buildhash.h
* @author		Lab 435, Xidian University
* @brief		定义了GROUPHASH类，存储分组结果和哈希表的映射关系。
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
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

/*! @addtogroup groupGrouphash
*  @{
*/

/*!
* @brief		hash表的结点信息。
* @remark		包括采用的分组采用的Signature和分组合并后的DFA。
*/

struct HASHNODE
{
	SIGNATURE m_sig;
	ulong m_nDfaId;
};

/*!
* @brief		封装hash函数类。
*/

struct SIGHASH
{
	ulong nBucketCnt;
	ulong operator()(const SIGNATURE &oneSig)
	{
		//const ulong _FNV_offset_basis = 2166136261U;
		//const ulong _FNV_prime = 16777619U;

		//ulong _Val = _FNV_offset_basis;
		//_Val ^= oneSig;
		//_Val *= _FNV_prime;

		//return (_Val % nBucketCnt);

		return (oneSig % nBucketCnt);
	}
};


HASHMAPHDR extern SIGHASH hash;

typedef std::map<ulong, std::vector<HASHNODE>> HASHRES;

/// @brief		每个分组均有能够代表该组的Signature(s)，为每个分组选择
///				代表该组的Signature，建立哈希表
/// @param[in]	groupRes 分组结果
/// @param[in]	HashResMap 哈希映射的结果
HASHMAPHDR void HashMapping(CGroupRes &groupRes, HASHRES &HashResMap);

/*!
@}
*/
