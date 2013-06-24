/*!
* @file			groups.h
* @author		Lab 435, Xidian University
* @brief		定义了CGroups类
*/

#pragma once

#include <hwprj\unsary.h>

#ifndef GROUPSHDR_DS
#define GROUPSHDR __declspec(dllimport)
#else
#define GROUPSHDR __declspec(dllexport)
#endif

#ifndef GROUPSHDR_DS
class GROUPVEC;
#endif

/*!
* @brief		分为一组的DFA信息。
* @remark		该组包括哪些DFA、这些DFA的公共Signature、当前采用哪个Signature代表该分组、合并后的DFA。
*/

struct ONEGROUP
{
	CUnsignedArray	DfaIds;
	CUnsignedArray	ComSigs;
	SIGNATURE		currSig;
	ulong			mergeDfaId;
};

/*!
* @brief		存储DFA的所有分组情况。
* @remark		包括哪些DFA分为一组、这些DFA的公共Signature、当前采用哪个Signature代表该分组、合并后的DFA。
*/

class GROUPSHDR CGroups
{
public:
	/// @brief		构造函数。
	CGroups();

	/// @brief		拷贝构造函数。
	CGroups(const CGroups& other);

	/// @brief		析构函数。
	virtual ~CGroups();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个CGroups对象。
	/// @return		返回自身对象的引用。
	CGroups& operator=(const CGroups &other);

	/// @brief		重载的 '[]' 运算符，取得指定位置的值的引用。
	/// @param[in]	nIdx 指定位置的下标
	/// @return		返回指定位置元素的引用。
	ONEGROUP& operator[](ulong nIdx);

	/// @brief		重载的 '[]' 运算符，取得指定位置的值的引用。
	/// @param[in]	nIdx 指定位置的下标
	/// @return		返回指定位置元素的引用（常量）。
	const ONEGROUP& operator[](ulong nIdx) const;

	/// @brief		得到CGroups数组当前的大小。
	/// @return		CGroups数组当前的大小。
	ulong Size() const;

	/// @brief		更改CGroups数组的大小。
	/// @param[in]	uSize 更改的尺寸。
	/// @remark		如果ulSize大于或等于当前数组尺寸，则不会更改现有元素；
	///				否则会删除多出部分的元素。
	void Resize(ulong nSize);

	/// @brief		将指定的一个分组结果添加为CGroups数组的最后一个元素。
	/// @param		oneGroup 指定的一个分组结果。
	void PushBack(ONEGROUP oneGroup);

	/// @brief		得到CGroups数组最后一个元素的引用。
	/// @return		最后一个元素的引用。
	ONEGROUP& Back();

	/// @brief		清空CGroups数组的所有元素。
	void Clear();

	/// @brief		删除CGroups数组中下标索引所对应的元素。
	/// @param[in]	nIdx 数组下标索引。
	void Erase(ulong nIdx);

protected:
	/// @brief		私有成员，仅供内部使用
	GROUPVEC* m_pGroups;
};

/*!
@}
*/
