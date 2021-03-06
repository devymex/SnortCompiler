/*!
* @file			groupres.h
* @author		Lab 435, Xidian University
* @brief		定义了CGroupRes类及其导出符号。
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
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

/*! @addtogroup groupGrouphash
*  @{
*/

/*!
* @brief		保存DFA及分组情况等信息。
* @remark		包括所有的DFA、规则处理信息、分组信息、hash槽数。
*/

class GROUPRESHDR CGroupRes
{
public:
	/// @brief		获取DFA集合
	CDfaArray &GetDfaTable();

	/// @brief		获取规则sid与DfaId的对应关系
	CSidDfaIds &GetSidDfaIds();

	/// @brief		获取分组结果
	CGroups & GetGroups();

	const CDfaArray &GetDfaTable() const;
	const CSidDfaIds &GetSidDfaIds() const;
	const CGroups &GetGroups() const;

	/// @brief		获取hash槽数
	ulong &GetBucketCnt();
	const ulong &GetBucketCnt() const;

	/// @brief		设置hash槽数,便于把hash槽数写入文件
	/// @param[in]	nBucketSize hash槽数
	void SetBucketCnt(ulong nBucketSize);
	
	/// @brief		将分组结果输出文件
	/// @param[in]	filename 输入包含文件路径的文件名
	ulong WriteToFile(const char *filename);

	/// @brief		将写入文件中的分组结果读入
	/// @param[in]	filename 输入包含文件路径的文件名
	ulong ReadFromFile(const char *filename);
private:
	CDfaArray m_dfaTbl;
	CSidDfaIds m_sidDfaIds; 
	CGroups m_groups;
	ulong nBucketCnt;
};

/*!
@}
*/
