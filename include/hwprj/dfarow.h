/*!
* @file			dfarow.h
* @author		Lab 435, Xidian University
* @brief		定义了CDfaRow类，用以存储DFA的一个状态。
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
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

/*!
* @brief		定义了DFA表中一个状态所对应的一行数据的数据结构。
* @remark		封装了std::vector<STATEID>对象。DFA跳转表中每一行对应一个状态。
*				行中每个元素对应一个字符，存储一个状态编号，表示该行对应的状态，
*				经过当前字符可以跳转到的下一个状态。
*/
class DFAHDR CDfaRow
{
public:
	typedef byte STATEFLAG;

	/// @brief		若DFA中的某个状态为常规状态，则标记为NORMAL。
	static const STATEFLAG NORMAL	= 0;			

	/// @brief		若DFA中的某个状态为终止状态，则标记为FINAL。
	static const STATEFLAG FINAL	= (1 << 0);		

	/// @brief		构造函数。
	/// @param[in]	ulColCnt DFA跳转字符集的长度。
	explicit CDfaRow(ulong ulColCnt);

	/// @brief		拷贝构造函数。
	CDfaRow(const CDfaRow &other);

	/// @brief		析构函数。
	virtual ~CDfaRow();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个CDfaRow对象。
	/// @return		返回自身对象的引用。
	CDfaRow& operator =	(const CDfaRow &other);

	/// @brief		重载的 '[]' 运算符，取得指定位置的DFA状态编号的引用。
	/// @param[in]	nIdx 指定位置的下标。
	/// @return		返回指定位置的DFA状态编号的引用（常量）。
	const STATEID& operator [] (byte nIdx) const;

	/// @brief		重载的 '[]' 运算符，取得指定位置的DFA状态编号的引用。
	/// @param[in]	nIdx 指定位置的下标。
	/// @return		返回指定位置的DFA状态编号的引用。
	STATEID& operator [] (byte nIdx);

	/// @brief		得到CDfaRow当前的大小。
	/// @return		CDfaRow当前的大小（常量）。
	ulong Size() const;

	/// @brief		将CDfaRow中的所有元素赋值为指定DFA状态。
	/// @param[in]	nState 指定的DFA状态。
	void Fill(STATEID nState);

	/// @brief		增加DFA状态的属性。
	/// @param[in]	nFlags 属性取值。
	void AddFlags(STATEFLAG nFlags);

	/// @brief		设置DFA状态的属性。
	/// @param[in]	nFlags 属性取值。
	void SetFlags(STATEFLAG nFlags);

	/// @brief		获取DFA状态的属性。
	STATEFLAG GetFlags() const;

	/////////////////////////////////////
	bool operator==(CDfaRow &rhs);
	void PushBack(ushort n);
private:
	/// @brief		私有成员，仅供内部使用。
	STATEIDARY* m_pDest;

	/// @brief		私有成员，仅供内部使用。
	STATEFLAG m_nFlags;
};

/*!
@}
*/