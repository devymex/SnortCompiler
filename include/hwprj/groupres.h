/*!
* @file			groupres.h
* @author		Lab 435, Xidian University
* @brief		定义了CGroupRes类，记录分组结果数据结果及相关算法
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\dfaarray.h>
#include <hwprj\siddfaids.h>
#include <hwprj\groups.h>

#ifndef GROUPRESHDR_DS
#define GROUPRESHDR __declspec(dllimport)
#else
#define GROUPRESHDR __declspec(dllexport)
#endif

class GROUPRESHDR CGroupRes
{
protected:
	CDfaArray m_dfaTbl;
	CSidDfaIds m_sidDfaIds; 
	CGroups m_groups;
	ulong nBucketCnt;
public:
	/// @brief		获取DFA集合
	CDfaArray &GetDfaTable();

	/// @brief		获取DFA集合（常量）
	const CDfaArray &GetDfaTable() const;

	/// @brief		获取规则sid与DfaId的对应关系
	CSidDfaIds &GetSidDfaIds();

	/// @brief		获取规则sid与DfaId的对应关系（常量）
	const CSidDfaIds &GetSidDfaIds() const;

	/// @brief		获取分组结果
	CGroups & GetGroups();

	/// @brief		获取分组结果（常量）
	const CGroups &GetGroups() const;

	/// @brief		获取hash槽数
	ulong &GetBucketCnt();

	/// @brief		获取hash槽数（常量）
	const ulong &GetBucketCnt() const;

	/// @brief		设置hash槽数,便于把hash槽数写入文件
	/// @param		nBucketSize hash槽数
	void SetBucketCnt(ulong nBucketSize);
	
	/// @brief		将分组结果输出文件
	/// @param		filename 输出文件路径
	ulong WriteToFile(const char *filename);

	/// @brief		将写入文件中的分组结果读入
	/// @param		filename 输入文件路径
	ulong ReadFromFile(const char *filename);
};