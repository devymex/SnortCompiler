/*!
* @file		dfarow.h
**
* @author		Lab 435, Xidian University
**
* @brief			Declaration of the CDfaRow class
**
* Store a row for CDfaRow. Array of std::vector<ulong>. Each element of the
* array stand by a column that indicates the jumping character.
* Each number in vector is the next state would be jump to by the character.
**
*/

#pragma once

#include <hwprj\common.h>


#ifndef DFAHDR_DS
class STATEIDARY;
#define DFAHDR __declspec(dllimport)
#else
#define DFAHDR __declspec(dllexport)
#endif

class DFAHDR CDfaRow
{
public:
	//标记该状态/行的属性：NORMAL、START、FINAL
	typedef byte STATEFLAG;
	static const STATEFLAG NORMAL	= 0;
	static const STATEFLAG FINAL	= (1 << 1);

	explicit CDfaRow(ulong ulColCnt);
	CDfaRow(const CDfaRow &other);
	virtual ~CDfaRow();

	CDfaRow& operator =	(const CDfaRow &other);
	const STATEID& operator []	(byte nIdx) const;
	STATEID& operator []	(byte nIdx);

	ulong			Size() const;
	void Fill(STATEID nState);

	/// @brief		增加状态属性
	/// @param nFlags 属性取值
	void AddFlags(STATEFLAG nFlags);

	/// @brief		设置状态属性
	/// @param	nFlags 属性取值
	void SetFlags(STATEFLAG nFlags);

	/// @brief		获取状态属性
	STATEFLAG		GetFlags() const;

protected:
	/// @brief		私有成员，仅供内部使用
	STATEIDARY*		m_pDest;

	/// @brief		私有成员，仅供内部使用
	STATEFLAG		m_nFlags;
};

