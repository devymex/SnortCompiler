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

	/// @brief		拷贝构造函数。
	CStringArray(const CStringArray &other);

	/// @brief		析构函数。
	virtual ~CStringArray();

	/// @brief		重载的赋值运算符。
	/// @param[in]	other 另一个CStringArray对象。
	CStringArray& operator = (const CStringArray &other);

	/// @brief		返回指定位置的字符串。
	/// @param[in]	ulIdx 指定的位置。
	/// @return		一个CDllString对象的引用。
	CDllString& operator [] (ulong ulIdx);

	/// @brief		返回指定位置的字符串（常量）。
	/// @param[in]	ulIdx 指定的位置。
	/// @return		一个CDllString对象的引用。
	const CDllString& operator [] (ulong ulIdx) const;

	/// @brief		获得所包含的字符串的数量。
	/// @return		字符串的个数。
	ulong Size() const;

	/// @brief		删除所包含的所有字符串。
	void Clear();

	/// @brief		将指定的字符串对象添加到数组。
	/// @param[in]	str 待添加进入的字符串对象。
	void PushBack(const CDllString &str);

private:
	/// @brief		私有成员，仅供内部使用。
	DLLSTRINGARY* m_pStrAry;
};

/*!
@}
*/
