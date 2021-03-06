#include "stdafx.h"
#include <hwprj\strary.h>
#include <hwprj\trace.h>

CStringArray::CStringArray()
{
	try
	{
		m_pStrAry = new DLLSTRINGARY;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

CStringArray::CStringArray(const CStringArray &other)
{
	try
	{
		m_pStrAry = new DLLSTRINGARY(*other.m_pStrAry);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

CStringArray::~CStringArray()
{
	delete []m_pStrAry;
}

CStringArray& CStringArray::operator = (const CStringArray &other)
{
	try
	{
		*m_pStrAry = *other.m_pStrAry;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
	return *this;
}

CDllString& CStringArray::operator [] (ulong ulIdx)
{
	TASSERT(ulIdx < m_pStrAry->size());
	return (*m_pStrAry)[ulIdx];
}

const CDllString& CStringArray::operator [] (ulong ulIdx) const
{
	TASSERT(ulIdx < m_pStrAry->size());
	return (*m_pStrAry)[ulIdx];
}

// Overided operators

ulong CStringArray::Size() const
{
	return m_pStrAry->size();
}

void CStringArray::Clear()
{
	m_pStrAry->clear();
}

void CStringArray::PushBack(const CDllString &str)
{
	try
	{
		m_pStrAry->push_back(str);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}
