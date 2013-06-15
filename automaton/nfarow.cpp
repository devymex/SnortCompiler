#include "stdafx.h"
#include <hwprj\nfarow.h>

NFAHDR CNfaRow::CNfaRow()
	: m_pDestSet(NULL)
{
	try
	{
		m_pDestSet = new CUnsignedArray[COLUMNCNT];
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

NFAHDR CNfaRow::CNfaRow(const CNfaRow &other)
	: m_pDestSet(0)
{
	*this = other;
}

NFAHDR CNfaRow::~CNfaRow()
{
	delete []m_pDestSet;
}

NFAHDR void CNfaRow::SortAll()
{
	for (ulong i = 0; i < COLUMNCNT; ++i)
	{
		m_pDestSet[i].Sort();
	}
}

NFAHDR CNfaRow& CNfaRow::operator=(const CNfaRow &other)
{
	try
	{
		for (ulong i = 0; i < COLUMNCNT; ++i)
		{
			m_pDestSet[i] = other.m_pDestSet[i];
		}
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
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
