/**
**	@file		unsary.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		impletation of the CUnsignedArray class
**
*/

#include "stdafx.h"

#include <hwprj\unsary.h>
#include <hwprj\trace.h>

UNSARYHDR CUnsignedArray::CUnsignedArray()
{
	try
	{
		m_pSet = new std::vector<ulong>;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

UNSARYHDR CUnsignedArray::~CUnsignedArray()
{
	delete m_pSet;
}

UNSARYHDR CUnsignedArray::CUnsignedArray(const CUnsignedArray &other)
{
	try
	{
		m_pSet = new std::vector<ulong>;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
	*this = other;
}

UNSARYHDR const CUnsignedArray& CUnsignedArray::operator = (const CUnsignedArray &other)
{
	try
	{
		*m_pSet = *other.m_pSet;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
	return *this;
}

UNSARYHDR ulong& CUnsignedArray::operator[](ulong nIdx)
{
	return (*m_pSet)[nIdx];
}

UNSARYHDR const ulong& CUnsignedArray::operator[](ulong nIdx) const
{
	return (*m_pSet)[nIdx];
}

UNSARYHDR bool CUnsignedArray::operator == (const CUnsignedArray &other)
{
	return *m_pSet == *other.m_pSet;
}

UNSARYHDR ulong CUnsignedArray::Size() const
{
	return m_pSet->size();
}

UNSARYHDR void CUnsignedArray::PopBack()
{
	m_pSet->pop_back();
}

UNSARYHDR void CUnsignedArray::PushBack(ulong nState)
{
	try
	{
		m_pSet->push_back(nState);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

UNSARYHDR void CUnsignedArray::Reserve(ulong nCount)
{
	try
	{
		m_pSet->reserve(nCount);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

UNSARYHDR void CUnsignedArray::Resize(ulong nSize)
{
	try
	{
		m_pSet->resize(nSize);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

UNSARYHDR ulong& CUnsignedArray::Back()
{
	return m_pSet->back();
}

UNSARYHDR void CUnsignedArray::Sort()
{
	std::sort(m_pSet->begin(), m_pSet->end());
}

UNSARYHDR void CUnsignedArray::Unique()
{
	Sort();
	m_pSet->erase(std::unique(m_pSet->begin(), m_pSet->end()), m_pSet->end());
}

UNSARYHDR void CUnsignedArray::Fill(ulong _Val)
{
	std::fill(m_pSet->begin(), m_pSet->end(), _Val);
}

UNSARYHDR void CUnsignedArray::Clear()
{
	m_pSet->clear();
}
