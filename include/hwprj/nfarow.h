/*!
* @file			nfarow.h
* @author		Lab 435, Xidian University
* @brief		Declaration of CNfaRow
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\unsary.h>

#ifndef NFAHDR_DS
#define NFAHDR __declspec(dllimport)
#else
#define NFAHDR __declspec(dllexport)
#endif

/*!
* @brief		封装了std::vector<CUnsignedArray>对象，表示NFA表中一行的数据结构。
* @remark		NFA表中每一行对应一个状态。行中每个元素对应一个字符，存储一组状态编号
*				的动态数组，表示该行对应的状态，经过当前字符可以跳转到的一组状态。
*/
class NFAHDR CNfaRow
{
public:
	/// @brief		NFA表的列数
	static const ulong COLUMNCNT = 260;

	/// @brief		构造函数。
	CNfaRow();

	/// @brief		拷贝构造函数。
	CNfaRow(const CNfaRow &other);

	/// @brief		析构函数。
	virtual ~CNfaRow();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		返回自身对象的引用。
	CNfaRow& operator = (const CNfaRow &other);

	/// @brief		重载的 '[]' 运算符，取得指定位置的状态集数组的引用。
	/// @param[in]	nIdx 指定位置的下标。
	/// @return		返回指定位置的状态集数组的引用。
	CUnsignedArray& operator [] (ulong ulCol);


	/// @brief		重载的 '[]' 运算符，取得指定位置的状态集数组的引用（常量）。
	/// @param[in]	nIdx 指定位置的下标。
	/// @return		返回指定位置的状态集数组的引用（常量）。
	const CUnsignedArray& operator [] (ulong ulCol) const;
	
	/// @brief		将NFA表中的每一个状态集数组进行排序。
	/// @todo		是否需要增加Unique函数？
	void SortAll();

private:
	/// @brief		私有成员，仅供内部使用。
	CUnsignedArray* m_pDestSet;
};
