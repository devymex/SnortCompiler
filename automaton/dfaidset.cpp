#include "stdafx.h"
#include <hwprj\dfaidset.h>

typedef DFAIDSET::iterator			DFAIDSET_ITER;
typedef DFAIDSET::const_iterator	DFAIDSET_CITER;

DFAIDSETHDR CDfaIdSet::CDfaIdSet()
{
	try
	{
		m_pSet = new DFAIDSET;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAIDSETHDR CDfaIdSet::CDfaIdSet(const CDfaIdSet &other)
{
	try
	{
		m_pSet = new DFAIDSET;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	*this = other;
}

DFAIDSETHDR CDfaIdSet::~CDfaIdSet()
{
	delete m_pSet;
}

DFAIDSETHDR CDfaIdSet& CDfaIdSet::operator = (const CDfaIdSet &other)
{
	try
	{
		*m_pSet = *other.m_pSet;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	return *this;
}

DFAIDSETHDR bool CDfaIdSet::operator == (const CDfaIdSet &other) const
{
	return *m_pSet == *other.m_pSet;
}

DFAIDSETHDR bool CDfaIdSet::operator <	(const CDfaIdSet &other) const
{
	return *m_pSet < *other.m_pSet;
}

DFAIDSETHDR ulong CDfaIdSet::Size() const
{
	return m_pSet->size();
}

DFAIDSETHDR void CDfaIdSet::Clear()
{
	m_pSet->clear();
}

DFAIDSETHDR void CDfaIdSet::Append(const CDfaIdSet &other)
{
	try
	{
		m_pSet->insert(other.m_pSet->begin(), other.m_pSet->end());
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAIDSETHDR void CDfaIdSet::AddDfaId(DFAID dfaId)
{
	try
	{
		m_pSet->insert(dfaId);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

DFAIDSETHDR void CDfaIdSet::CopyTo(CUnsignedArray &idAry) const
{
	try
	{
		for (DFAIDSET_CITER i = m_pSet->cbegin(); i != m_pSet->cend(); ++i)
		{
			idAry.PushBack(*i);
		}
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}
