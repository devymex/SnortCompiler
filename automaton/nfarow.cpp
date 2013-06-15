#include "stdafx.h"
#include <hwprj\nfarow.h>


NFAHDR CNfaRow::CNfaRow(ulong nSize)
	: m_nSize(nSize), m_pDestSet(NULL)
{
	if (nSize != 0)
	{
		try
		{
			m_pDestSet = new std::vector<ulong>[m_nSize];
		}
		catch (std::exception &e)
		{
			throw CTrace(__FILE__, __LINE__, e.what());
		}
	}
}

NFAHDR CNfaRow::CNfaRow(const CNfaRow &other)
	: m_nSize(0), m_pDestSet(0)
{
	*this = other;
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
		if (nSize != 0)
		{
			try
			{
				m_pDestSet = new std::vector<ulong>[nSize];
			}
			catch (std::exception &e)
			{
				throw CTrace(__FILE__, __LINE__, e.what());
			}
		}
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
	memcpy(pOut, col.data(), col.size() * sizeof(col[0]));
}

NFAHDR void CNfaRow::AddDest(ulong nCol, ulong nDest)
{
	try
	{
		m_pDestSet[nCol].push_back(nDest);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

NFAHDR void CNfaRow::SortAll()
{
	for (ulong i = 0; i < m_nSize; ++i)
	{
		std::sort(m_pDestSet[i].begin(), m_pDestSet[i].end());
	}
}

NFAHDR CNfaRow& CNfaRow::operator=(const CNfaRow &other)
{
	try
	{
		Resize(other.m_nSize);
		for (ulong i = 0; i < m_nSize; ++i)
		{
			m_pDestSet[i] = other.m_pDestSet[i];
		}
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}

	return *this;
}
