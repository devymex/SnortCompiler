/*
**	@file		dfaidset.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CDfaIdSet class
**
**	Store the corresponding ID for each CDfa
**
*/

#pragma once

#include <hwprj\unsary.h>

#ifndef DFAIDSETHDR_DS
class DFAIDSET;
#define DFAIDSETHDR __declspec(dllimport)
#else
#define DFAIDSETHDR __declspec(dllexport)
#endif

class DFAIDSETHDR CDfaIdSet
{
public:
	CDfaIdSet();
	CDfaIdSet(const CDfaIdSet &other);
	virtual ~CDfaIdSet();

	CDfaIdSet&	operator =	(const CDfaIdSet &other);
	bool		operator == (const CDfaIdSet &other) const;
	bool		operator <	(const CDfaIdSet &other) const;

	ulong		Size() const;
	void		Clear();
	/*	函数名：Append
	**	参数：另一个DfaIdSet
	**	功能：将另一个DfaIdSet连接到当前的DfaIdSet
	*/
	void		Append(const CDfaIdSet &other);
	/*	函数名：AddDfaId
	**	参数：dfaId-DFA编号
	**	功能：添加DFA ID
	*/
	void		AddDfaId(DFAID dfaId);
	/*	函数名：CopyTo
	**	参数：idAry-DfaIdSet的数组
	**	功能：将DfaIdSet中的数值复制给DfaIdSet的数组
	*/
	void		CopyTo(CUnsignedArray &idAry) const;

private:
	DFAIDSET*	m_pSet;
};
