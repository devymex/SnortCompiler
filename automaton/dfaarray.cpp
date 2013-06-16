#include "stdafx.h"
#include <hwprj\dfaarray.h>

DFAARYHDR CDfaArray::CDfaArray()
{
	try
	{
		m_pdfaTbl = new CDFAVEC;	
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAARYHDR CDfaArray::CDfaArray(const CDfaArray& other)
	: m_pdfaTbl(null)
{
	try
	{
		m_pdfaTbl = new CDFAVEC(*other.m_pdfaTbl);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAARYHDR CDfaArray::~CDfaArray()
{
	delete m_pdfaTbl;
}

DFAARYHDR CDfaArray &CDfaArray::operator=(const CDfaArray &other)
{
	TASSERT(other.m_pdfaTbl != null);
	try
	{
		*m_pdfaTbl = *other.m_pdfaTbl;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	return *this;
}

DFAARYHDR CDfa& CDfaArray::operator[](ulong ulIdx)
{
	TASSERT(ulIdx < m_pdfaTbl->size());
	return (*m_pdfaTbl)[ulIdx];
}

DFAARYHDR const CDfa& CDfaArray::operator[](ulong ulIdx) const
{
	TASSERT(ulIdx < m_pdfaTbl->size());
	return (*m_pdfaTbl)[ulIdx];
}

DFAARYHDR void CDfaArray::Reserve(ulong ulCount)
{
	try
	{
		m_pdfaTbl->reserve(ulCount);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAARYHDR void CDfaArray::Resize(ulong ulSize)
{
	try
	{
		m_pdfaTbl->resize(ulSize);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAARYHDR ulong CDfaArray::Size() const
{
	return m_pdfaTbl->size();
}

DFAARYHDR void CDfaArray::PushBack(const CDfa &dfa)
{
	try
	{
		m_pdfaTbl->push_back(dfa);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAARYHDR void CDfaArray::Clear()
{
	m_pdfaTbl->clear();
}
