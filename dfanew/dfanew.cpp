#include "stdafx.h"
#include "dfanew.h"

DFANEWSC CDfanew::CDfanew()
{
	m_pDfa = new std::vector<CDfaRow>;
}

DFANEWSC CDfanew::~CDfanew()
{
	delete m_pDfa;
}

DFANEWSC CDfanew::CDfanew(const CDfanew &other)
{
	m_pDfa = new std::vector<CDfaRow>;
	*this = other;
}

DFANEWSC CDfanew& CDfanew::operator=(const CDfanew &other)
{
	Clear();
	m_nId = other.m_nId;
	m_nColNum = other.m_nColNum;
	m_StartId = other.m_StartId;
	CopyMemory(m_pGroup, other.m_pGroup, DFACOLSIZE * sizeof(BYTE));
	*m_pDfa = *other.m_pDfa;
	return *this;
}

DFANEWSC STATEID CDfanew::Size() const
{
	return (STATEID)m_pDfa->size();
}

DFANEWSC CDfaRow& CDfanew::operator[](STATEID index)
{
	return (*m_pDfa)[index];
}

DFANEWSC const CDfaRow& CDfanew::operator[](STATEID index) const
{
	return (*m_pDfa)[index];
}

DFANEWSC void CDfanew::Init(BYTE *pGroup)
{
	
}

DFANEWSC void CDfanew::Clear()
{
	
}

DFANEWSC size_t CDfanew::FromNFA(CNfa &nfa, NFALOG *nfalog, size_t Count)
{
	return 0;
}

DFANEWSC size_t CDfanew::Minimize()
{
	return 0;
}

DFANEWSC size_t CDfanew::GetGroupCount() const
{
	return m_nColNum;
}

DFANEWSC BYTE CDfanew::Char2Group(BYTE nIdx)
{
	return m_pGroup[nIdx];
}

DFANEWSC const BYTE* CDfanew::GetGroup() const
{
	return m_pGroup;
}

DFANEWSC STATEID CDfanew::GetStartId() const
{
	return m_StartId;
}

DFANEWSC void CDfanew::SetId(size_t id)
{
	m_nId = id;
}

DFANEWSC size_t CDfanew::GetId()
{
	return m_nId;
}

DFANEWSC size_t CDfanew::Process(BYTE *ByteStream, size_t len, CStateSet &StaSet)
{
	return 0;
}

DFANEWSC void CDfanew::GetAcceptedId(STATEID id, CVectorNumber &dfaIds)
{
	
}
