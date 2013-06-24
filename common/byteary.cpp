/*!
* @file			byteary.cpp
* @author		Lab 435, Xidian University
* @brief		实现了CByteArray的成员函数
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#include "stdafx.h"
#include <hwprj\trace.h>
#include <hwprj\byteary.h>

BYTEARYHDR CByteArray::CByteArray()
	: m_pAry(null)
{
	try
	{
		m_pAry = new DLLBYTEARY;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

BYTEARYHDR CByteArray::CByteArray(const CByteArray &other)
	: m_pAry(null)
{
	try
	{
		m_pAry = new DLLBYTEARY(*other.m_pAry);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

BYTEARYHDR CByteArray::~CByteArray()
{
	delete m_pAry;
}

BYTEARYHDR CByteArray& CByteArray::operator = (const CByteArray &other)
{
	if (this != &other)
	{
		*m_pAry = *other.m_pAry;
	}
	return *this;
}

BYTEARYHDR byte& CByteArray::operator [] (ulong ulIdx)
{
	TASSERT(ulIdx < m_pAry->size());
	return (*m_pAry)[ulIdx];
}

BYTEARYHDR const byte& CByteArray::operator [] (ulong ulIdx) const
{
	TASSERT(ulIdx < m_pAry->size());
	return (*m_pAry)[ulIdx];
}

BYTEARYHDR ulong CByteArray::Size() const
{
	return m_pAry->size();
}

BYTEARYHDR bool CByteArray::Empty() const
{
	return m_pAry->empty();
}

BYTEARYHDR void CByteArray::Clear()
{
	m_pAry->clear();
}

BYTEARYHDR void CByteArray::PushBack(byte byVal)
{
	try
	{
		m_pAry->push_back(byVal);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

BYTEARYHDR void CByteArray::PopBack()
{
	TASSERT(!m_pAry->empty());
	m_pAry->pop_back();
}

BYTEARYHDR void CByteArray::Resize(ulong ulSize)
{
	try
	{
		m_pAry->resize(ulSize);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

BYTEARYHDR void CByteArray::Reserve(ulong ulCap)
{
	try
	{
		m_pAry->reserve(ulCap);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

BYTEARYHDR byte* CByteArray::Data()
{
	TASSERT(!m_pAry->empty());
	return m_pAry->data();
}

BYTEARYHDR const byte* CByteArray::Data() const
{
	TASSERT(!m_pAry->empty());
	return m_pAry->data();
}
