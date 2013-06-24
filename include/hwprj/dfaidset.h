/*!
* @file			dfaidset.h
* @author		Lab 435, Xidian University
* @brief		定义了CDfaIdSet类，用以存储每一个DFA相应的ID
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

#include <hwprj\unsary.h>

#ifndef DFAIDSETHDR_DS
class DFAIDSET;
#define DFAIDSETHDR __declspec(dllimport)
#else
#define DFAIDSETHDR __declspec(dllexport)
#endif

/*!
* @addtogroup groupAutomaton
* @{
*/

/*!
* @brief		实现CDfaIdSet数据结构及相关算法。
* @remark		存储每一个CDfa对应的ID。实现的算法包括：
*				添加ID、复制ID集合等。
*/
class DFAIDSETHDR CDfaIdSet
{
public:
	/// @brief		构造函数。
	CDfaIdSet();

	/// @brief		拷贝构造函数。
	CDfaIdSet(const CDfaIdSet &other);

	/// @brief		析构函数。
	virtual ~CDfaIdSet();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个CDfaIdSet对象。
	/// @return		返回自身对象的引用。
	CDfaIdSet& operator =	(const CDfaIdSet &other);

	/// @brief		重载的 '==' 运算符，判断自身对象是否与所指定的对象相同。
	/// @param[in]	other 另一个CDfaIdSet对象。
	/// @retval		true 两个CDfaIdSet相等。
	/// @retval		false 两个CDfaIdSet不相等。
	/// @remark		将会执行逐元素的比较。
	bool operator == (const CDfaIdSet &other) const;

	/// @brief		重载的 '<' 运算符，判断自身对象是否比所指定的对象小。
	/// @param[in]	other 另一个CDfaIdSet对象。
	/// @return		当前CDfaIdSet比另一个CDfaIdSet小时返回true，否则返回false。
	/// @remark		将会执行逐元素的比较。若二者长短不同，且比较至较短数组的末尾之后，
	///				较长的数组为大。
	bool operator <	(const CDfaIdSet &other) const;

	/// @brief		得到CDfaIdSet当前的大小。
	/// @return		CDfaIdSet当前的大小（常量）。
	ulong Size() const;

	/// @brief		清空CDfaIdSet。
	void Clear();

	/// @brief		将另一个DfaIdSet连接到当前的DfaIdSet之后。
	/// @param[in]	other 另一个DfaIdSet。
	void Append(const CDfaIdSet &other);

	/// @brief		添加DFA ID。
	/// @param[in]	dfaId DFA编号。
	void AddDfaId(DFAID dfaId);

	/// @brief		将DfaIdSet中的value复制给CUnsignedArray类型的数组。
	/// @param[out]	idAry CUnsignedArray类型的数组。
	void CopyTo(CUnsignedArray &idAry) const;

private:
	/// @brief		私有成员，仅供内部使用。
	DFAIDSET* m_pSet;
};

/*!
@}
*/
