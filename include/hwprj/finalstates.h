/**
**	@file		finalstates.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CFinalStates class
**
** store the mapping between STATEID and DFAID
**
*/

#pragma once

#include <hwprj\unsary.h>

#include <set>

#ifndef FINSTAHDR_DS
#define FINSTAHDR __declspec(dllimport)
class STATEVEC;
class FINSTAMAP;
#else
#define FINSTAHDR __declspec(dllexport)
#endif

class FINSTAHDR CFinalStates
{

public:
	CFinalStates();
	CFinalStates(const CFinalStates &other);
	virtual ~CFinalStates();

	CFinalStates&	operator =	(const CFinalStates &other);
	STATEID			operator []	(ulong nIdx) const;

	ulong	Size() const;
	void	Clear();
	void	PushBack(STATEID nStaId);
	void	PushBack(STATEID nStaId, DFAID nDfaId);
	ulong	GetDfaIdCount(STATEID nStaId) const;
	ulong	GetAllDfaIdCount() const;
	void	GetDfaIds(STATEID nStaId, CUnsignedArray &ids) const;

	std::set<ulong>&		_GetDfaIds(STATEID nStaId);
	const std::set<ulong>&	_GetDfaIds(STATEID nStaId) const;

protected:
	STATEVEC	*m_pStates;
	FINSTAMAP	*m_pDfaIds;
};
