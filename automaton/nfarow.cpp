#include "stdafx.h"
#include <hwprj\nfarow.h>

NFAHDR CNfaRow::CNfaRow()
	: m_pDestSet(null)
{
	try
	{
		m_pDestSet = new CUnsignedArray[COLUMNCNT];
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

NFAHDR CNfaRow::CNfaRow(const CNfaRow &other)
	: m_pDestSet(null)
{
	try
	{
		m_pDestSet = new CUnsignedArray[COLUMNCNT];
		for (ulong i = 0; i < COLUMNCNT; ++i)
		{
			m_pDestSet[i] = other.m_pDestSet[i];
		}
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

NFAHDR CNfaRow::~CNfaRow()
{
	delete []m_pDestSet;
}

NFAHDR CNfaRow& CNfaRow::operator=(const CNfaRow &other)
{
	TASSERT(other.m_pDestSet != null);
	try
	{
		for (ulong i = 0; i < COLUMNCNT; ++i)
		{
			m_pDestSet[i] = other.m_pDestSet[i];
		}
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}

	return *this;
}

NFAHDR CUnsignedArray& CNfaRow::operator[](ulong ulCol)
{
	TASSERT(ulCol < COLUMNCNT);
	return m_pDestSet[ulCol];
}

NFAHDR const CUnsignedArray& CNfaRow::operator[](ulong ulCol) const
{
	TASSERT(ulCol < COLUMNCNT);
	return m_pDestSet[ulCol];
}

NFAHDR void CNfaRow::SortAll()
{
	for (ulong i = 0; i < COLUMNCNT; ++i)
	{
		m_pDestSet[i].Sort();
	}
}
