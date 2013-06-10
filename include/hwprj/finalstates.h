/**
**  @file        common.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CDllArray, CDllString
**
*/

#pragma once

#include <set>
#include <unordered_map>
#include <hwprj\dllarray.h>

#ifndef FINSTAHDR_DS
#define FINSTAHDR __declspec(dllimport)
#else
#define FINSTAHDR __declspec(dllexport)
#endif


class FINSTAHDR CFinalStates
{
	typedef std::unordered_map<STATEID, std::set<ULONG>> FINSTAMAP;
	typedef FINSTAMAP::iterator FINSTAMAP_ITER;
	typedef FINSTAMAP::const_iterator FINSTAMAP_CITER;
public:
	CFinalStates();
	virtual ~CFinalStates();
	CFinalStates(const CFinalStates &other);
	CFinalStates& operator=(const CFinalStates &other);

	STATEID operator[](ULONG nIdx) const;

	ULONG Size() const;
	void Clear();
	void PushBack(STATEID nStaId);
	void PushBack(STATEID nStaId, ULONG nDfaId);
	ULONG GetDfaIdCount(STATEID nStaId) const;
	ULONG GetAllDfaIdCount() const;
	void GetDfaIds(STATEID nStaId, CDllArray &ids) const;

	std::set<ULONG>& _GetDfaIds(STATEID nStaId);
	const std::set<ULONG>& _GetDfaIds(STATEID nStaId) const;

protected:
	std::vector<STATEID> *m_pStates;
	FINSTAMAP *m_pDfaIds;
};