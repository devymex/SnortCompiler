#include "stdafx.h"
#include <hwprj\dfarow.h>

DFAHDR CDfaRow::CDfaRow(ULONG col)
	: m_nFlag(NORMAL), m_nColNum(col)
{
	m_pDest = new STATEVEC;
	m_pDest->resize(m_nColNum);
	Fill(STATEID(-1));
}

DFAHDR CDfaRow::~CDfaRow()
{
	delete m_pDest;
}

DFAHDR CDfaRow::CDfaRow(const CDfaRow &other)
{
	m_pDest = new STATEVEC;
	*this = other;
}

DFAHDR CDfaRow& CDfaRow::operator=(const CDfaRow &other)
{
	m_nFlag = other.m_nFlag;
	m_nColNum = other.m_nColNum;
	*m_pDest = *other.m_pDest;
	return *this;
}

DFAHDR STATEID& CDfaRow::operator[](BYTE nIdx)
{
	return (*m_pDest)[nIdx];
}

DFAHDR const STATEID& CDfaRow::operator[](BYTE nIdx) const
{
	return (*m_pDest)[nIdx];
}

DFAHDR void CDfaRow::Fill(STATEID _Val)
{
	std::fill(m_pDest->begin(), m_pDest->end(), _Val);
}

DFAHDR void CDfaRow::SetFlag(ULONG nFlag)
{
	m_nFlag = nFlag;
}

DFAHDR ULONG CDfaRow::GetFlag() const
{
	return m_nFlag;
}

DFAHDR ULONG CDfaRow::GetColNum() const
{
	return m_nColNum;
}
