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
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

DFAARYHDR CDfaArray::CDfaArray(const CDfaArray& other)
{
	try
	{
		m_pdfaTbl = new CDFAVEC;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
	*this = other;
}

DFAARYHDR const CDfaArray &CDfaArray::operator=(const CDfaArray &other)
{
	try
	{
		*m_pdfaTbl = *other.m_pdfaTbl;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
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
	try
	{
		m_pdfaTbl->reserve(nCount);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

DFAARYHDR void CDfaArray::Resize(ulong nSize)
{
	try
	{
		m_pdfaTbl->resize(nSize);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

DFAARYHDR const ulong CDfaArray::Size() const
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
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

DFAARYHDR void CDfaArray::Clear()
{
	m_pdfaTbl->clear();
}
