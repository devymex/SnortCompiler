#include "stdafx.h"
#include <hwprj\dfarow.h>

DFAHDR CDfaRow::CDfaRow(ulong col)
	: m_nFlag(NORMAL), m_nColNum(col)
{
	m_pDest = new STATEVEC;
	CHECKALLOC(m_pDest);
	m_pDest->resize(m_nColNum);
	memset(m_pDest->data(), -1, m_pDest->size() * sizeof(m_pDest->front()));
}

DFAHDR CDfaRow::~CDfaRow()
{
	delete m_pDest;
}

DFAHDR CDfaRow::CDfaRow(const CDfaRow &other)
{
	m_pDest = new STATEVEC;
	CHECKALLOC(m_pDest);
	*this = other;
}

DFAHDR CDfaRow& CDfaRow::operator=(const CDfaRow &other)
{
	m_nFlag = other.m_nFlag;
	m_nColNum = other.m_nColNum;
	*m_pDest = *other.m_pDest;
	return *this;
}

DFAHDR STATEID& CDfaRow::operator[](byte nIdx)
{
	return (*m_pDest)[nIdx];
}

DFAHDR const STATEID& CDfaRow::operator[](byte nIdx) const
{
	return (*m_pDest)[nIdx];
}

DFAHDR void CDfaRow::Fill(STATEID _Val)
{
	std::fill(m_pDest->begin(), m_pDest->end(), _Val);
}

DFAHDR void CDfaRow::SetFlag(ulong nFlag)
{
	m_nFlag = nFlag;
}

DFAHDR ulong CDfaRow::GetFlag() const
{
	return m_nFlag;
}

DFAHDR ulong CDfaRow::GetColNum() const
{
	return m_nColNum;
}
