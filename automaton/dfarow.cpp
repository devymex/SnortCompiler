#include "stdafx.h"
#include <hwprj\dfarow.h>

DFAHDR CDfaRow::CDfaRow(ulong ulColCnt)
	: m_nFlags(CDfaRow::NORMAL)
{
	try
	{
		m_pDest = new STATEVEC;
		m_pDest->resize(ulColCnt);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	memset(m_pDest->data(), -1, m_pDest->size() * sizeof(m_pDest->front()));
}

DFAHDR CDfaRow::~CDfaRow()
{
	delete m_pDest;
}

DFAHDR CDfaRow::CDfaRow(const CDfaRow &other)
{
	try
	{
		m_pDest = new STATEVEC;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	*this = other;
}

DFAHDR CDfaRow& CDfaRow::operator=(const CDfaRow &other)
{
	TASSERT(other.m_pDest != null);
	try
	{
		*m_pDest = *other.m_pDest;
		m_nFlags = other.m_nFlags;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	return *this;
}

DFAHDR STATEID& CDfaRow::operator[](byte nIdx)
{
	TASSERT(nIdx < m_pDest->size());
	return (*m_pDest)[nIdx];
}

DFAHDR const STATEID& CDfaRow::operator[](byte nIdx) const
{
	TASSERT(nIdx < m_pDest->size());
	return (*m_pDest)[nIdx];
}

DFAHDR ulong CDfaRow::Size() const
{
	return m_pDest->size();
}

DFAHDR void CDfaRow::Fill(STATEID nState)
{
	std::fill(m_pDest->begin(), m_pDest->end(), nState);
}

void CDfaRow::AddFlags(STATEFLAG nFlags)
{
	m_nFlags |= nFlags;
}

DFAHDR CDfaRow::STATEFLAG CDfaRow::GetFlags() const
{
	return m_nFlags;
}

DFAHDR void CDfaRow::SetFlags(STATEFLAG nFlag)
{
	m_nFlags = nFlag;
}
