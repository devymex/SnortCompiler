#include "stdafx.h"
#include <hwprj\groups.h>

GROUPSHDR CGroups::CGroups()
{
	m_pGroups = new GROUPVEC;
}

GROUPSHDR CGroups::CGroups(const CGroups& other)
{
	TASSERT(other.m_pGroups != null);
	try
	{
		m_pGroups = new GROUPVEC(*other.m_pGroups);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

GROUPSHDR CGroups::~CGroups()
{
	delete m_pGroups;
}

GROUPSHDR ulong CGroups::Size() const
{
	return m_pGroups->size();
}

GROUPSHDR void CGroups::Resize(ulong nSize)
{
	try
	{
		m_pGroups->resize(nSize);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

GROUPSHDR CGroups &CGroups::operator=(const CGroups &other)
{
	TASSERT(other.m_pGroups != null);
	if (this != &other)
	{
		try
		{
			*m_pGroups = *other.m_pGroups;
		}
		catch (std::exception &e)
		{
			TTHROW(e.what());
		}
	}
	return *this;
}

GROUPSHDR ONEGROUP &CGroups::operator[](ulong nIdx)
{
	TASSERT(nIdx < m_pGroups->size());
	return (*m_pGroups)[nIdx];
}

GROUPSHDR const ONEGROUP &CGroups::operator[](ulong nIdx) const
{
	TASSERT(nIdx < m_pGroups->size());
	return (*m_pGroups)[nIdx];
}

GROUPSHDR void CGroups::PushBack(ONEGROUP oneGroup)
{
	m_pGroups->push_back(oneGroup);
}

GROUPSHDR ONEGROUP& CGroups::Back()
{
	TASSERT(!m_pGroups->empty());
	return m_pGroups->back();
}

GROUPSHDR void CGroups::Clear()
{
	m_pGroups->clear();
}

GROUPSHDR void CGroups::Erase(ulong nIdx)
{
	TASSERT(nIdx < m_pGroups->size());
	m_pGroups->erase(m_pGroups->begin() + nIdx);
}
