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

	/// @param	nStaId ״̬��š�
	/// @brief		��̬����������DFA��̬�ı�š�
	CDfaIdSet& AddState(STATEID nStaId);

	/// @brief		����һ��DFA����̬�����滻��ǰDFA����̬���ϡ�
	/// @param other DFA��̬���ϡ�
	void Swap(CFinalStates &other);

	/// @brief		��ȡDFA��̬��Ӧ��DfaId���ϡ�
	/// @param nStaId ״̬��š�
	CDfaIdSet& GetDfaIdSet(STATEID nStaId);
	const CDfaIdSet& GetDfaIdSet(STATEID nStaId) const;

	/// @brief		����DFA��̬��Ӧ��DfaId�ĸ�����
	ulong CountDfaIds() const;

protected:
	STATEIDARY* m_pStates;
	FINSTAMAP* m_pDfaIds;
};
