/**
**	@file		unsary.h
**
**	@author	  Lab 435, Xidian University
**
**	@brief		Impletation of the CDllString class
**
*/

#include "stdafx.h"
#include <hwprj\trace.h>
#include <hwprj\dllstring.h>

DLLSTRHDR CDllString::CDllString()
{
	m_pString = new std::string;
	CHECKALLOC(m_pString);
}

DLLSTRHDR CDllString::CDllString(const char *pStr)
{
	m_pString = new std::string(pStr);
	CHECKALLOC(m_pString);
}

DLLSTRHDR void CDllString::Append(const char* pChar)
{
	m_pString->append(pChar);;
}

DLLSTRHDR CDllString::~CDllString()
{
	delete m_pString;
}

DLLSTRHDR CDllString::CDllString(const CDllString &other)
{
	m_pString = new std::string;
	CHECKALLOC(m_pString);
	*this = other;
}

DLLSTRHDR CDllString& CDllString::operator = (const CDllString &other)
{
	*this->m_pString = *other.m_pString;
	return *this;
}
DLLSTRHDR char CDllString::operator[](ulong nIdx) const
{
	return (*m_pString)[nIdx];
}

DLLSTRHDR ulong CDllString::Size() const
{
	return m_pString->size();
}

DLLSTRHDR void CDllString::PushBack(const char nChar)
{
	m_pString->push_back(nChar);
}

DLLSTRHDR char CDllString::Back() const
{
	return m_pString->back();
}

DLLSTRHDR const char* CDllString::GetStr() const
{
	return m_pString->c_str();
}

DLLSTRHDR void CDllString::Clear()
{
	m_pString->clear();
}

DLLSTRHDR bool CDllString::Empty() const
{
	return m_pString->empty();
}
