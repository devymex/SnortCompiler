/*
**	@file		finalstates.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CFinalStates class
**
**	Store the mapping between STATEID and DFAID
**
*/

#pragma once

#include <hwprj\dfaidset.h>

#ifndef DFAIDSETHDR_DS
class STATEVEC;
class FINSTAMAP;
#endif

class DFAIDSETHDR CFinalStates
{
public:
	CFinalStates();
	CFinalStates(const CFinalStates &other);
	virtual ~CFinalStates();

	CFinalStates&		operator =	(const CFinalStates &other);
	STATEID				operator []	(ulong nIdx) const;

	ulong				Size() const;
	void				Clear();

	/*	函数名：AddState
	**	参数：nStaId-状态编号
	**	功能：终态集合中增加DFA终态的编号
	*/
	CDfaIdSet&			AddState(STATEID nStaId);
	/*	函数名：Swap
	**	参数：other-DFA终态集合
	**	功能：将另一个DFA的终态集合替换当前DFA的终态集合
	*/
	void				Swap(CFinalStates &other);

	/*	函数名：GetDfaIdSet
	**	参数：nStaId-状态编号
	**	功能：获取DFA终态对应的DfaId集合
	*/
	CDfaIdSet&			GetDfaIdSet(STATEID nStaId);
	const CDfaIdSet&	GetDfaIdSet(STATEID nStaId) const;
	/*	函数名：CountDfaIds
	**	功能：计算DFA终态对应的DfaId的个数
	*/
	ulong				CountDfaIds() const;

protected:
	STATEVEC	*m_pStates;
	FINSTAMAP	*m_pDfaIds;
};
