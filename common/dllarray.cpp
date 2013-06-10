#include "stdafx.h"
#include <hwprj\dllarray.h>

UNSARYHDR CDllArray::CDllArray()
{
	m_pSet = new std::vector<ULONG>;
}

UNSARYHDR CDllArray::~CDllArray()
{
	delete m_pSet;
}

UNSARYHDR CDllArray::CDllArray(const CDllArray &other)
{
	m_pSet = new std::vector<ULONG>;
	*this = other;
}

UNSARYHDR const CDllArray& CDllArray::operator = (const CDllArray &other)
{
	*m_pSet = *other.m_pSet;
	return *this;
}

UNSARYHDR ULONG& CDllArray::operator[](ULONG nIdx)
{
	return (*m_pSet)[nIdx];
}

UNSARYHDR const ULONG& CDllArray::operator[](ULONG nIdx) const
{
	return (*m_pSet)[nIdx];
}

UNSARYHDR bool CDllArray::operator == (const CDllArray &other)
{
	return *m_pSet == *other.m_pSet;
}

UNSARYHDR ULONG CDllArray::Size() const
{
	return m_pSet->size();
}

UNSARYHDR void CDllArray::PopBack()
{
	m_pSet->pop_back();
}

UNSARYHDR void CDllArray::PushBack(ULONG nState)
{
	m_pSet->push_back(nState);
}

UNSARYHDR void CDllArray::Reserve(ULONG nCount)
{
	m_pSet->reserve(nCount);
}

UNSARYHDR void CDllArray::Resize(ULONG nSize)
{
	m_pSet->resize(nSize);
}

UNSARYHDR ULONG& CDllArray::Back()
{
	return m_pSet->back();
}

UNSARYHDR void CDllArray::Sort()
{
	std::sort(m_pSet->begin(), m_pSet->end());
}

UNSARYHDR void CDllArray::Unique()
{
	Sort();
	m_pSet->erase(std::unique(m_pSet->begin(), m_pSet->end()), m_pSet->end());
}

UNSARYHDR void CDllArray::Fill(ULONG _Val)
{
	std::fill(m_pSet->begin(), m_pSet->end(), _Val);
}

UNSARYHDR void CDllArray::Clear()
{
	m_pSet->clear();
}
