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

	CDfaIdSet&			AddState(STATEID nStaId);
	void				Swap(CFinalStates &other);

	CDfaIdSet&			GetDfaIdSet(STATEID nStaId);
	const CDfaIdSet&	GetDfaIdSet(STATEID nStaId) const;
	ulong				CountDfaIds() const;

protected:
	STATEVEC	*m_pStates;
	FINSTAMAP	*m_pDfaIds;
};
