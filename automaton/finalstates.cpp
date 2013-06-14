#include "stdafx.h"
#include <hwprj\finalstates.h>

typedef FINSTAMAP::iterator								FINSTAMAP_ITER;
typedef FINSTAMAP::const_iterator						FINSTAMAP_CITER;

DFAIDSETHDR CFinalStates::CFinalStates()
{
	m_pStates = new STATEVEC;
	CHECKALLOC(m_pStates);
	m_pDfaIds = new FINSTAMAP;
	CHECKALLOC(m_pDfaIds);
}

DFAIDSETHDR CFinalStates::~CFinalStates()
{
	delete m_pStates;
	delete m_pDfaIds;
}

DFAIDSETHDR CFinalStates::CFinalStates(const CFinalStates &other)
{
	m_pStates = new STATEVEC;
	CHECKALLOC(m_pStates);
	m_pDfaIds = new FINSTAMAP;
	CHECKALLOC(m_pDfaIds);
	*this = other;
}

DFAIDSETHDR CFinalStates& CFinalStates::operator=(const CFinalStates &other)
{
	*m_pStates = *other.m_pStates;
	*m_pDfaIds = *other.m_pDfaIds;
	return *this;
}

DFAIDSETHDR STATEID CFinalStates::operator[](ulong nIdx) const
{
	return (*m_pStates)[nIdx];
}

DFAIDSETHDR ulong CFinalStates::Size() const
{
	return m_pStates->size();
}

DFAIDSETHDR void CFinalStates::Clear()
{
	m_pDfaIds->clear();
	m_pStates->clear();
}

DFAIDSETHDR void CFinalStates::PushBack(STATEID nStaId)
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		(*m_pDfaIds)[nStaId];
		(*m_pStates).push_back(nStaId);
	}
}

DFAIDSETHDR void CFinalStates::PushBack(STATEID nStaId, DFAID nDfaId)
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		(*m_pDfaIds)[nStaId].Add(nDfaId);
		(*m_pStates).push_back(nStaId);
	}
	else
	{
		iter->second.Add(nDfaId);
	}
}

DFAIDSETHDR ulong CFinalStates::GetDfaIdCount(STATEID nStaId) const
{
	return (*m_pDfaIds)[nStaId].Size();
}

DFAIDSETHDR ulong CFinalStates::GetAllDfaIdCount() const
{
	ulong nSum = 0;
	for (FINSTAMAP_CITER i = m_pDfaIds->cbegin(); i != m_pDfaIds->end(); ++i)
	{
		nSum += i->second.Size();
	}
	return nSum;
}

DFAIDSETHDR void CFinalStates::Swap(CFinalStates &other)
{
	std::swap(m_pStates, other.m_pStates);
	std::swap(m_pDfaIds, other.m_pDfaIds);
}

DFAIDSETHDR CDfaIdSet& CFinalStates::GetDfaIdSet(STATEID nStaId)
{
	FINSTAMAP_ITER iterMe = m_pDfaIds->find(nStaId);
	if (iterMe == m_pDfaIds->end())
	{
		throw 0;
	}
	return iterMe->second;
}

DFAIDSETHDR const CDfaIdSet& CFinalStates::GetDfaIdSet(STATEID nStaId) const
{
	FINSTAMAP_CITER iterMe = m_pDfaIds->find(nStaId);
	if (iterMe == m_pDfaIds->end())
	{
		throw 0;
	}
	return iterMe->second;
}
