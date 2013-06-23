/*!
* @file		finalstates.h
**
* @author		Lab 435, Xidian University
**
* @brief			Declaration of the CFinalStates class
**
* Store the mapping between STATEID and DFAID
**
*/

#pragma once

#include <hwprj\dfaidset.h>

#ifndef DFAIDSETHDR_DS
class STATEIDARY;
class FINSTAMAP;
#endif

class DFAIDSETHDR CFinalStates
{
public:
	CFinalStates();
	CFinalStates(const CFinalStates &other);
	virtual ~CFinalStates();

	CFinalStates& operator = (const CFinalStates &other);
	STATEID operator [] (ulong nIdx) const;

	ulong Size() const;
	void Clear();

	/// @param	nStaId 状态编号。
	/// @brief		终态集合中增加DFA终态的编号。
	CDfaIdSet& AddState(STATEID nStaId);

	/// @brief		将另一个DFA的终态集合替换当前DFA的终态集合。
	/// @param other DFA终态集合。
	void Swap(CFinalStates &other);

	/// @brief		获取DFA终态对应的DfaId集合。
	/// @param nStaId 状态编号。
	CDfaIdSet& GetDfaIdSet(STATEID nStaId);
	const CDfaIdSet& GetDfaIdSet(STATEID nStaId) const;

	/// @brief		计算DFA终态对应的DfaId的个数。
	ulong CountDfaIds() const;

protected:
	STATEIDARY* m_pStates;
	FINSTAMAP* m_pDfaIds;
};
