/*!
* @file			dfarow.h
* @author		Lab 435, Xidian University
* @brief		定义了CDfaRow类，用以存储DFA的一个状态
* @remark		CDfaRow为一个一维数组，数组中的每个元素所在列表示
*				DFA字符集中的一个跳转字符，数组中的每个元素表示当前DFA状态
*				经过跳转字符到达的下一个状态的编号。
*/

#pragma once

#include <hwprj\common.h>


#ifndef DFAHDR_DS
class STATEIDARY;
#define DFAHDR __declspec(dllimport)
#else
#define DFAHDR __declspec(dllexport)
#endif

/*!
* @addtogroup groupAutomaton
* @{
*/

class DFAHDR CDfaRow
{
public:
	typedef byte STATEFLAG;

	/// @brief 标记该行所对应的状态的类型
	static const STATEFLAG NORMAL	= 0;			/// 一般状态
	static const STATEFLAG FINAL	= (1 << 1);		/// 终态

	/// @brief		构造函数。
	/// @param[in]	ulColCnt DFA字符集的个数。
	explicit CDfaRow(ulong ulColCnt);

	/// @brief		拷贝构造函数。
	/// @param[in]	other 另一个CDfaRow对象。
	CDfaRow(const CDfaRow &other);

	/// @brief		析构函数。
	virtual ~CDfaRow();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个CDfaRow对象。
	/// @return		返回自身对象的引用。
	CDfaRow& operator =	(const CDfaRow &other);

	/// @brief		重载的 '[]' 运算符，取得指定位置的值的引用。
	/// @param[in]	nIdx 指定位置的下标
	/// @return		返回指定位置元素的引用。
	const STATEID& operator [] (byte nIdx) const;

	/// @brief		重载的 '[]' 运算符，取得指定位置的值的引用。
	/// @param[in]	nIdx 指定位置的下标
	/// @return		返回指定位置元素的引用（常量）。
	STATEID& operator [] (byte nIdx);

	/// @brief		得到CDfaRow当前的大小。
	/// @return		CDfaRow当前的大小（常量）。
	ulong Size() const;

	/// @brief		将CDfaRow中的所有元素赋值为指定状态
	/// @param[in]	nState 指定的状态。
	void Fill(STATEID nState);

	/// @brief		增加状态属性
	/// @param[in]	nFlags 属性取值
	void AddFlags(STATEFLAG nFlags);

	/// @brief		设置状态属性
	/// @param[in]	nFlags 属性取值
	void SetFlags(STATEFLAG nFlags);

	/// @brief		获取状态属性
	STATEFLAG GetFlags() const;

private:
	/// @brief		私有成员，仅供内部使用
	STATEIDARY* m_pDest;

	/// @brief		私有成员，仅供内部使用
	STATEFLAG m_nFlags;
};

/*!
@}
*/