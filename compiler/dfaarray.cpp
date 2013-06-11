#include "stdafx.h"
#include <hwprj\dfaarray.h>


DFAARYHDR CDfaArray::CDfaArray()
{
	m_pdfaTbl = new std::vector<CDfa>;	
}

DFAARYHDR CDfaArray::CDfaArray(const CDfaArray& other)
{
	m_pdfaTbl = new std::vector<CDfa>;
	*this = other;
}

DFAARYHDR const CDfaArray &CDfaArray::operator=(const CDfaArray &other)
{
	*m_pdfaTbl = *other.m_pdfaTbl;
	return *this;
}

DFAARYHDR CDfaArray::~CDfaArray()
{
	delete m_pdfaTbl;
}

DFAARYHDR CDfa& CDfaArray::operator[](ulong index)
{
	return (*m_pdfaTbl)[index];
}

DFAARYHDR const CDfa& CDfaArray::operator[](ulong index) const
{
	return (*m_pdfaTbl)[index];
}

DFAARYHDR void CDfaArray::Reserve(ulong nCount)
{
	m_pdfaTbl->reserve(nCount);
}

DFAARYHDR void CDfaArray::Resize(ulong nSize)
{
	m_pdfaTbl->resize(nSize);
}

DFAARYHDR const ulong CDfaArray::Size() const
{
	return m_pdfaTbl->size();
}

DFAARYHDR void CDfaArray::PushBack(const CDfa &dfa)
{
	m_pdfaTbl->push_back(dfa);
}

DFAARYHDR void CDfaArray::Clear()
{
	m_pdfaTbl->clear();
}
