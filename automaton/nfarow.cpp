#include "stdafx.h"
#include <hwprj\nfarow.h>


NFAHDR CNfaRow::CNfaRow(ulong nSize)
	: m_nSize(nSize)
{
	m_pDestSet = new std::vector<ulong>[m_nSize];
	CHECKALLOC(m_pDestSet);
}

NFAHDR CNfaRow::~CNfaRow()
{
	delete []m_pDestSet;
}

NFAHDR ulong CNfaRow::Size() const
{
	return m_nSize;
}

NFAHDR void CNfaRow::Resize(ulong nSize)
{
	if (m_nSize != nSize)
	{
		delete []m_pDestSet;
		m_pDestSet = new std::vector<ulong>[nSize];
		CHECKALLOC(m_pDestSet);
		m_nSize = nSize;
	}
}

NFAHDR ulong CNfaRow::DestCnt(ulong nCol) const
{
	return m_pDestSet[nCol].size();
}

NFAHDR ulong& CNfaRow::GetDest(ulong nCol, ulong nIdx)
{
	return m_pDestSet[nCol][nIdx];
}

NFAHDR const ulong& CNfaRow::GetDest(ulong nCol, ulong nIdx) const
{
	return m_pDestSet[nCol][nIdx];
}

NFAHDR ulong* CNfaRow::GetCol(ulong nCol)
{
	return m_pDestSet[nCol].data();
}

NFAHDR const ulong* CNfaRow::GetCol(ulong nCol) const
{
	return m_pDestSet[nCol].data();
}

NFAHDR void CNfaRow::CopyCol(ulong nCol, ulong *pOut) const
{
	std::vector<ulong> &col = m_pDestSet[nCol];
	memcpy(pOut, col.data(), col.size() * sizeof(ulong));
}

NFAHDR void CNfaRow::AddDest(ulong nCol, ulong nDest)
{
	m_pDestSet[nCol].push_back(nDest);
}

NFAHDR void CNfaRow::SortAll()
{
	for (ulong i = 0; i < m_nSize; ++i)
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
	for (ulong i = 0; i < m_nSize; ++i)
	{
		m_pDestSet[i] = other.m_pDestSet[i];
	}

	return *this;
}
