/*!
* @file			dfaarray.h
* @author		Lab 435, Xidian University
* @brief		定义了CDfaArray类，用以存储多个CDfa，即DFA数组
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

#include <hwprj\dfa.h>

#ifndef DFAARYHDR_DS
class CDFAVEC;
#define DFAARYHDR __declspec(dllimport)
#else
#define DFAARYHDR __declspec(dllexport)
#endif

/*!
* @addtogroup groupAutomaton
* @{
*/

/*!
* @brief		封装了std::vector<class CDfa>对象，表示DFA数组的数据结构。
* @remark		数组中每一个元素都是一个CDfa对象。
*/
class DFAARYHDR CDfaArray
{
public:
	/// @brief		构造函数。
	CDfaArray();

	/// @brief		拷贝构造函数。
	CDfaArray(const CDfaArray& other);

	/// @brief		析构函数。
	virtual ~CDfaArray();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个DFA数组对象。
	/// @return		返回自身对象的引用。
	CDfaArray& operator = (const CDfaArray &other);

	/// @brief		重载的 '[]' 运算符，取得指定位置的CDfa类对象的引用。
	/// @param[in]	nIdx 指定位置的下标。
	/// @return		返回指定位置的CDfa类对象的引用。
	CDfa& operator [] (ulong ulIdx);

	/// @brief		重载的 '[]' 运算符，取得指定位置的CDfa类对象的引用。
	/// @param[in]	nIdx 指定位置的下标。
	/// @return		返回指定位置的CDfa类对象的引用（常量）。
	const CDfa& operator [] (ulong ulIdx) const;

	/// @brief		获得DFA数组当前的个数。
	/// @return		DFA数组当前的个数（常量）。
	ulong Size() const;

	/// @brief		清空DFA数组。
	void Clear();

	/// @brief		为存储DFA数组预留内存空间。
	/// @param[in]	ulCount 指定的预留的空间。
	void Reserve(ulong ulCount);

	/// @brief		更改DFA数组的大小。
	/// @param[in]	ulSize 更改的尺寸。
	/// @remark		如果ulSize大于或等于当前尺寸，则不会更改现有元素；
	///				否则会删除多出部分的元素。
	void Resize(ulong ulSize);

	/// @brief		将指定的DFA添加为DFA数组的最后一个元素。
	/// @param[in]	dfa 指定的DFA。
	void PushBack(const CDfa &dfa);

private:
	/// @brief		私有成员，仅供内部使用。
	CDFAVEC *m_pdfaTbl;
};

/*!
@}
*/
