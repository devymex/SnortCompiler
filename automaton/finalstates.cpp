#include "stdafx.h"
#include <hwprj\finalstates.h>

typedef FINSTAMAP::iterator			FINSTAMAP_ITER;
typedef FINSTAMAP::const_iterator	FINSTAMAP_CITER;

DFAIDSETHDR CFinalStates::CFinalStates()
	: m_pStates(null), m_pDfaIds(null)
{
	try
	{
		m_pStates = new STATEVEC;
		m_pDfaIds = new FINSTAMAP;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAIDSETHDR CFinalStates::~CFinalStates()
{
	delete m_pStates;
	delete m_pDfaIds;
}

DFAIDSETHDR CFinalStates::CFinalStates(const CFinalStates &other)
	: m_pStates(null), m_pDfaIds(null)
{
	try
	{
		m_pStates = new STATEVEC(*other.m_pStates);
		m_pDfaIds = new FINSTAMAP(*other.m_pDfaIds);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAIDSETHDR CFinalStates& CFinalStates::operator = (const CFinalStates &other)
{
	TASSERT(other.m_pDfaIds != null);
	TASSERT(other.m_pStates != null);
	if (this != &other)
	{
		try
		{
			*m_pStates = *other.m_pStates;
			*m_pDfaIds = *other.m_pDfaIds;
		}
		catch (std::exception &e)
		{
			TTHROW(e.what());
		}
	}
	return *this;
}

DFAIDSETHDR STATEID CFinalStates::operator[](ulong nIdx) const
{
	TASSERT(nIdx < m_pStates->size());
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

DFAIDSETHDR CDfaIdSet& CFinalStates::AddState(STATEID nStaId)
{
	CDfaIdSet *pFound = null;
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		try
		{
			pFound = &(*m_pDfaIds)[nStaId];
			(*m_pStates).push_back(nStaId);
		}
		catch (std::exception &e)
		{
			TTHROW(e.what());
		}
	}
	else
	{
		pFound = &(iter->second);
	}
	return *pFound;
}

DFAIDSETHDR void CFinalStates::Swap(CFinalStates &other)
{
	TASSERT(other.m_pDfaIds != null);
	TASSERT(other.m_pStates != null);
	std::swap(m_pStates, other.m_pStates);
	std::swap(m_pDfaIds, other.m_pDfaIds);
}

DFAIDSETHDR CDfaIdSet& CFinalStates::GetDfaIdSet(STATEID nStaId)
{
	FINSTAMAP_ITER iterMe = m_pDfaIds->find(nStaId);
	if (iterMe == m_pDfaIds->end())
	{
		TTHROW(TI_NOTFOUND);
	}
	return iterMe->second;
}

DFAIDSETHDR const CDfaIdSet& CFinalStates::GetDfaIdSet(STATEID nStaId) const
{
	FINSTAMAP_CITER iterMe = m_pDfaIds->find(nStaId);
	if (iterMe == m_pDfaIds->end())
	{
		TTHROW(TI_NOTFOUND);
	}
	return iterMe->second;
}

DFAIDSETHDR ulong CFinalStates::CountDfaIds() const
{
	ulong nSum = 0;
	for (FINSTAMAP_CITER i = m_pDfaIds->cbegin(); i != m_pDfaIds->end(); ++i)
	{
		nSum += i->second.Size();
	}
	return nSum;
}
