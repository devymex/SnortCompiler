#include "stdafx.h"
#include <hwprj\nfarow.h>


NFAHDR CNfaRow::CNfaRow(ULONG nSize)
	: m_nSize(nSize)
{
	m_pDestSet = new std::vector<ULONG>[m_nSize];
}

NFAHDR CNfaRow::~CNfaRow()
{
	delete []m_pDestSet;
}

NFAHDR ULONG CNfaRow::Size() const
{
	return m_nSize;
}

NFAHDR void CNfaRow::Resize(ULONG nSize)
{
	if (m_nSize != nSize)
	{
		delete []m_pDestSet;
		m_pDestSet = new std::vector<ULONG>[nSize];
		m_nSize = nSize;
	}
}

NFAHDR ULONG CNfaRow::DestCnt(ULONG nCol) const
{
	return m_pDestSet[nCol].size();
}

NFAHDR ULONG& CNfaRow::GetDest(ULONG nCol, ULONG nIdx)
{
	return m_pDestSet[nCol][nIdx];
}

NFAHDR const ULONG& CNfaRow::GetDest(ULONG nCol, ULONG nIdx) const
{
	return m_pDestSet[nCol][nIdx];
}

NFAHDR ULONG* CNfaRow::GetCol(ULONG nCol)
{
	return m_pDestSet[nCol].data();
}

NFAHDR const ULONG* CNfaRow::GetCol(ULONG nCol) const
{
	return m_pDestSet[nCol].data();
}

NFAHDR void CNfaRow::CopyCol(ULONG nCol, ULONG *pOut) const
{
	std::vector<ULONG> &col = m_pDestSet[nCol];
	memcpy(pOut, col.data(), col.size() * sizeof(ULONG));
}

NFAHDR void CNfaRow::AddDest(ULONG nCol, ULONG nDest)
{
	m_pDestSet[nCol].push_back(nDest);
}

NFAHDR void CNfaRow::SortAll()
{
	for (ULONG i = 0; i < m_nSize; ++i)
	{
		std::sort(m_pDestSet[i].begin(), m_pDestSet[i].end());
	}
}

NFAHDR CNfaRow::CNfaRow(const CNfaRow &other)
	: m_nSize(0), m_pDestSet(0)
{
	*this = other;
}

NFAHDR CNfaRow& CNfaRow::operator=(const CNfaRow &other)
{
	Resize(other.m_nSize);
	for (ULONG i = 0; i < m_nSize; ++i)
	{
		m_pDestSet[i] = other.m_pDestSet[i];
	}

	return *this;
}
