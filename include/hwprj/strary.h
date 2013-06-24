/*!
* @file			dllstring.h
* @author		Lab 435, Xidian University
* @brief		定义了CStringArray类及其导出符号
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\dllstring.h>

#ifndef DLLSTRHDR_DS
class DLLSTRINGARY;
#endif

/*!
* @addtogroup groupCommon
* @{
*/

/*!
* @brief	封装了std::vector<CDllString>对象，提供字符串动态数组的数据结构。
*/
class DLLSTRHDR CStringArray
{
public:
	/// @brief		构造函数。
	CStringArray();
	CStringArray(const CStringArray &other);
	virtual ~CStringArray();

	CStringArray& operator = (const CStringArray &other);
	CDllString& operator[] (ulong ulIdx);
	const CDllString& operator[] (ulong ulIdx) const;

	ulong Size() const;
	void Clear();
	void PushBack(const CDllString &str);

private:
	/// @brief		私有成员，仅供内部使用
	DLLSTRINGARY*		m_pStrAry;
};

/*!
@}
*/
