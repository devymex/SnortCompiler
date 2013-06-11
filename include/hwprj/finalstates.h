/**
**  @file        finalstates.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CUnsignedArray, CDllString
**
*/

#pragma once

#include <set>
#include <unordered_map>
#include <hwprj\unsary.h>

#ifndef FINSTAHDR_DS
#define FINSTAHDR __declspec(dllimport)
class STATEVEC;
#else
#define FINSTAHDR __declspec(dllexport)
#endif


class FINSTAHDR CFinalStates
{
	typedef std::unordered_map<STATEID, std::set<ulong>>	FINSTAMAP;
	typedef FINSTAMAP::iterator								FINSTAMAP_ITER;
	typedef FINSTAMAP::const_iterator						FINSTAMAP_CITER;

public:
	CFinalStates();
	CFinalStates(const CFinalStates &other);
	virtual ~CFinalStates();

	CFinalStates&	operator =	(const CFinalStates &other);
	STATEID			operator []	(ulong nIdx) const;

	ulong	Size() const;
	void	Clear();
	void	PushBack(STATEID nStaId);
	void	PushBack(STATEID nStaId, ulong nDfaId);
	ulong	GetDfaIdCount(STATEID nStaId) const;
	ulong	GetAllDfaIdCount() const;
	void	GetDfaIds(STATEID nStaId, CUnsignedArray &ids) const;

	std::set<ulong>&		_GetDfaIds(STATEID nStaId);
	const std::set<ulong>&	_GetDfaIds(STATEID nStaId) const;

protected:
	STATEVEC	*m_pStates;
	FINSTAMAP	*m_pDfaIds;
};