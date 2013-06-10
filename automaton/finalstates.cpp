#include "stdafx.h"
#include <hwprj/finalstates.h>

FINSTAHDR_DS CFinalStates::CFinalStates()
{
	m_pStates = new std::vector<STATEID>;
	m_pDfaIds = new std::unordered_map<STATEID, std::set<ULONG>>;
}

FINSTAHDR_DS CFinalStates::~CFinalStates()
{
	delete m_pStates;
	delete m_pDfaIds;
}

FINSTAHDR_DS CFinalStates::CFinalStates(const CFinalStates &other)
{
	m_pStates = new std::vector<STATEID>;
	m_pDfaIds = new std::unordered_map<STATEID, std::set<ULONG>>;
	*this = other;
}

FINSTAHDR_DS CFinalStates& CFinalStates::operator=(const CFinalStates &other)
{
	*m_pStates = *other.m_pStates;
	*m_pDfaIds = *other.m_pDfaIds;
	return *this;
}

FINSTAHDR_DS STATEID CFinalStates::operator[](ULONG nIdx) const
{
	return (*m_pStates)[nIdx];
}

FINSTAHDR_DS ULONG CFinalStates::Size() const
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

FINSTAHDR_DS void CFinalStates::PushBack(STATEID nStaId, ULONG nDfaId)
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

FINSTAHDR_DS void CFinalStates::GetDfaIds(STATEID nStaId, CDllArray &ids) const
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		throw 0;
	}
	const std::set<ULONG> &curSet = iter->second;
	for (std::set<ULONG>::const_iterator i = curSet.begin();
		i != curSet.end(); ++i)
	{
		if (*i > 10000)
		{
			system("pause");
		}
		ids.PushBack(*i);
	}
}

FINSTAHDR_DS std::set<ULONG>& CFinalStates::_GetDfaIds(STATEID nStaId)
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		throw 0;
	}
	return iter->second;
}

FINSTAHDR_DS const std::set<ULONG>& CFinalStates::_GetDfaIds(STATEID nStaId) const
{
	FINSTAMAP_ITER iter = m_pDfaIds->find(nStaId);
	if (iter == m_pDfaIds->end())
	{
		throw 0;
	}
	return iter->second;
}

FINSTAHDR_DS ULONG CFinalStates::GetDfaIdCount(STATEID nStaId) const
{
	return (*m_pDfaIds)[nStaId].size();
}

FINSTAHDR_DS ULONG CFinalStates::GetAllDfaIdCount() const
{
	ULONG nSum = 0;
	for (FINSTAMAP_CITER i = m_pDfaIds->cbegin(); i != m_pDfaIds->end(); ++i)
	{
		nSum += i->second.size();
	}
	return nSum;
}