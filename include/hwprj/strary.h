
/*!
* @file		dllstring.h
**
* @author		Lab 435, Xidian University
**
* @brief			Common string class
**
* Definition of the CDllString class
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\dllstring.h>

#ifndef DLLSTRHDR_DS
class DLLSTRINGARY;
#endif

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

protected:
	/// @brief		私有成员，仅供内部使用
	DLLSTRINGARY*		m_pStrAry;
};
