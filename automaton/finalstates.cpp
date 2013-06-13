#include "stdafx.h"
#include <hwprj\finalstates.h>

typedef FINSTAMAP::iterator								FINSTAMAP_ITER;
typedef FINSTAMAP::const_iterator						FINSTAMAP_CITER;

FINSTAHDR_DS CFinalStates::CFinalStates()
{
	m_pStates = new STATEVEC;
	m_pDfaIds = new FINSTAMAP;
}

FINSTAHDR_DS CFinalStates::~CFinalStates()
{
	delete m_pStates;
	delete m_pDfaIds;
}

FINSTAHDR_DS CFinalStates::CFinalStates(const CFinalStates &other)
{
	m_pStates = new STATEVEC;
	m_pDfaIds = new FINSTAMAP;
	*this = other;
}

FINSTAHDR_DS CFinalStates& CFinalStates::operator=(const CFinalStates &other)
{
	*m_pStates = *other.m_pStates;
	*m_pDfaIds = *other.m_pDfaIds;
	return *this;
}

FINSTAHDR_DS STATEID CFinalStates::operator[](ulong nIdx) const
{
	return (*m_pStates)[nIdx];
}

FINSTAHDR_DS ulong CFinalStates::Size() const
{
	return m_pStates->size();
}

FINSTAHDR_DS void CFinalStates::Clear()
{
	m_pDfaIds->clear();
	m_pStates->clear();
}

FINSTAHDR_DS void CFinalStates::PushBack(STATEID nStaId)
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		(*m_pDfaIds)[nStaId];
		(*m_pStates).push_back(nStaId);
	}
}

FINSTAHDR_DS void CFinalStates::PushBack(STATEID nStaId, DFAID nDfaId)
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		(*m_pDfaIds)[nStaId].insert(nDfaId);
		(*m_pStates).push_back(nStaId);
	}
	else
	{
		iter->second.insert(nDfaId);
	}
}

FINSTAHDR_DS void CFinalStates::GetDfaIds(STATEID nStaId, CUnsignedArray &ids) const
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		throw 0;
	}
	const std::set<ulong> &curSet = iter->second;
	for (std::set<ulong>::const_iterator i = curSet.begin();
		i != curSet.end(); ++i)
	{
		if (*i > 10000)
		{
			system("pause");
		}
		ids.PushBack(*i);
	}
}

FINSTAHDR_DS std::set<ulong>& CFinalStates::_GetDfaIds(STATEID nStaId)
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		throw 0;
	}
	return iter->second;
}

FINSTAHDR_DS const std::set<ulong>& CFinalStates::_GetDfaIds(STATEID nStaId) const
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		throw 0;
	}
	return iter->second;
}

FINSTAHDR_DS ulong CFinalStates::GetDfaIdCount(STATEID nStaId) const
{
	return (*m_pDfaIds)[nStaId].size();
}

FINSTAHDR_DS ulong CFinalStates::GetAllDfaIdCount() const
{
	ulong nSum = 0;
	for (FINSTAMAP_CITER i = m_pDfaIds->cbegin(); i != m_pDfaIds->end(); ++i)
	{
		nSum += i->second.size();
	}
	return nSum;
}