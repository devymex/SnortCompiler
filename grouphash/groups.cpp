#include "stdafx.h"
#include <hwprj\groups.h>

GROUPSHDR CGroups::CGroups()
{
	m_pGroups = new GROUPVEC;
}

GROUPSHDR CGroups::CGroups(const CGroups& other)
{
	m_pGroups = new GROUPVEC;
	*this = other;
}

GROUPSHDR CGroups &CGroups::operator=(const CGroups &other)
{
	*m_pGroups = *other.m_pGroups;
	return *this;
}

GROUPSHDR CGroups::~CGroups()
{
	delete m_pGroups;
}

GROUPSHDR const ulong CGroups::Size() const
{
	return m_pGroups->size();
}

GROUPSHDR void CGroups::Resize(ulong nSize)
{
	m_pGroups->resize(nSize);
}

GROUPSHDR ONEGROUP &CGroups::operator[](ulong nIdx)
{
	return (*m_pGroups)[nIdx];
}

GROUPSHDR const ONEGROUP &CGroups::operator[](ulong nIdx) const
{
	return (*m_pGroups)[nIdx];
}

GROUPSHDR void CGroups::PushBack(ONEGROUP oneGroup)
{
	m_pGroups->push_back(oneGroup);
}

GROUPSHDR ONEGROUP& CGroups::Back()
{
	return m_pGroups->back();
}

GROUPSHDR void CGroups::Clear()
{
	m_pGroups->clear();
}

GROUPSHDR void CGroups::Erase(ulong nIdx)
{
	m_pGroups->erase(m_pGroups->begin() + nIdx);
}
