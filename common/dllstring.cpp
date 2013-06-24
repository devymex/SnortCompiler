/*!
* @file			unsary.h
* @author		Lab 435, Xidian University
* @brief		实现了CDllString类的成员函数
*/

#include "stdafx.h"
#include <hwprj\dllstring.h>
#include <hwprj\trace.h>

DLLSTRHDR CDllString::CDllString()
{
	try
	{
		m_pString = new std::string;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DLLSTRHDR CDllString::CDllString(const char *pStr)
{
	TASSERT(pStr != null);
	try
	{
		m_pString = new std::string(pStr);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DLLSTRHDR CDllString::CDllString(const CDllString &other)
{
	try
	{
		m_pString = new std::string(*other.m_pString);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DLLSTRHDR CDllString::~CDllString()
{
	delete m_pString;
}

DLLSTRHDR CDllString& CDllString::operator = (const CDllString &other)
{
	if (this != &other)
	{
		try
		{
			*m_pString = *other.m_pString;
		}
		catch (std::exception &e)
		{
			TTHROW(e.what());
		}
	}
	return *this;
}

DLLSTRHDR char& CDllString::operator[](ulong nIdx) const
{
	return (*m_pString)[nIdx];
}

DLLSTRHDR ulong CDllString::Size() const
{
	return m_pString->size();
}

DLLSTRHDR void CDllString::PushBack(char nChar)
{
	try
	{
		m_pString->push_back(nChar);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DLLSTRHDR void CDllString::Append(const CDllString &other)
{
	m_pString->append(other.Data());
}

DLLSTRHDR void CDllString::Insert(ulong ulIdx, char nChar)
{
	m_pString->insert(m_pString->begin() + ulIdx, nChar);
}

DLLSTRHDR void CDllString::Insert(ulong ulIdx, pcstr pChar)
{
	m_pString->insert(m_pString->begin() + ulIdx, pChar, pChar + strlen(pChar));
}

DLLSTRHDR void CDllString::Assign(const char* pStr)
{
	m_pString->assign(pStr);
}

DLLSTRHDR char CDllString::Back() const
{
	return m_pString->back();
}

DLLSTRHDR void CDllString::Clear()
{
	m_pString->clear();
}

DLLSTRHDR void CDllString::Erase(ulong ulIdx)
{
	m_pString->erase(m_pString->begin() + ulIdx);
}

DLLSTRHDR bool CDllString::Empty() const
{
	return m_pString->empty();
}

DLLSTRHDR const char* CDllString::Data() const
{
	TASSERT(!m_pString->empty());
	return m_pString->c_str();
}

