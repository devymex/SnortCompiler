#include "stdafx.h"
#include <hwprj\dfaidset.h>

typedef DFAIDSET::iterator			DFAIDSET_ITER;
typedef DFAIDSET::const_iterator	DFAIDSET_CITER;

DFAIDSETHDR CDfaIdSet::CDfaIdSet()
{
	m_pSet = new DFAIDSET;
}

DFAIDSETHDR CDfaIdSet::CDfaIdSet(const CDfaIdSet &other)
{
	m_pSet = new DFAIDSET;
	*this = other;
}

DFAIDSETHDR CDfaIdSet::~CDfaIdSet()
{
	delete m_pSet;
}

DFAIDSETHDR const CDfaIdSet& CDfaIdSet::operator = (const CDfaIdSet &other)
{
	*m_pSet = *other.m_pSet;
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
	m_pSet->insert(other.m_pSet->begin(), other.m_pSet->end());
}

DFAIDSETHDR void CDfaIdSet::Add(DFAID dfaId)
{
	m_pSet->insert(dfaId);
}

DFAIDSETHDR void CDfaIdSet::CopyTo(CUnsignedArray &idAry) const
{
	for (DFAIDSET_CITER i = m_pSet->cbegin(); i != m_pSet->cend(); ++i)
	{
		idAry.PushBack(*i);
	}
}
