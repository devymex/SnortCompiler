#include "stdafx.h"
#include "common.h"

RULEOPTION::~RULEOPTION()
{
}

COMMONSC CStateSet::CStateSet()
{
	m_pSet = new std::vector<size_t>;
}

COMMONSC CStateSet::~CStateSet()
{
	delete m_pSet;
}

COMMONSC CStateSet::CStateSet(const CStateSet &other)
{
	m_pSet = new std::vector<size_t>;
	*this = other;
}

COMMONSC const CStateSet& CStateSet::operator = (const CStateSet &other)
{
	*m_pSet = *other.m_pSet;
	return *this;
}

COMMONSC size_t& CStateSet::operator[](size_t nIdx)
{
	return (*m_pSet)[nIdx];
}

COMMONSC const size_t& CStateSet::operator[](size_t nIdx) const
{
	return (*m_pSet)[nIdx];
}

COMMONSC bool CStateSet::operator == (const CStateSet &other)
{
	return *m_pSet == *other.m_pSet;
}

COMMONSC const size_t CStateSet::Size() const
{
	return m_pSet->size();
}

COMMONSC void CStateSet::PopBack()
{
	m_pSet->pop_back();
}

COMMONSC void CStateSet::PushBack(size_t nState)
{
	m_pSet->push_back(nState);
}

COMMONSC void CStateSet::Reserve(size_t nCount)
{
	m_pSet->reserve(nCount);
}

COMMONSC void CStateSet::Resize(size_t nSize)
{
	m_pSet->resize(nSize);
}

COMMONSC size_t& CStateSet::Back()
{
	return m_pSet->back();
}

COMMONSC void CStateSet::Sort()
{
	std::sort(m_pSet->begin(), m_pSet->end());
}

COMMONSC void CStateSet::Unique()
{
	Sort();
	m_pSet->erase(std::unique(m_pSet->begin(), m_pSet->end()), m_pSet->end());
}

COMMONSC CNfaRow::CNfaRow()
{
}

COMMONSC CNfaRow::~CNfaRow()
{
}

COMMONSC CStateSet& CNfaRow::operator[](size_t nChar)
{
	return m_pDestSet[nChar];
}

COMMONSC const CStateSet& CNfaRow::operator[](size_t nChar) const
{
	return m_pDestSet[nChar];
}

COMMONSC CNfaRow::CNfaRow(const CNfaRow &other)
{
	*this = other;
}

COMMONSC CNfaRow& CNfaRow::operator=(const CNfaRow &other)
{
	for (size_t i = 0; i < CHARSETSIZE; ++i)
	{
		m_pDestSet[i] = other.m_pDestSet[i];
	}

	return *this;
}

COMMONSC CNfa::CNfa()
{
	m_pNfa = new std::vector<CNfaRow>;
}

COMMONSC CNfa::~CNfa()
{
	delete m_pNfa;
}

COMMONSC size_t CNfa::GetRowNum(void)
{
	return m_pNfa->size();
}

COMMONSC void CNfa::Reserve(size_t _Count)
{
	m_pNfa->reserve(_Count);
}

COMMONSC void CNfa::Resize(size_t _Newsize)
{
	m_pNfa->resize(_Newsize);
}

COMMONSC size_t CNfa::Size() const
{
	return m_pNfa->size();
}

COMMONSC CNfaRow& CNfa::Back()
{
	return m_pNfa->back();
}

COMMONSC CNfaRow& CNfa::operator[](size_t index)
{
	return (*m_pNfa)[index];
}

COMMONSC const CNfaRow& CNfa::operator[](size_t index) const
{
	return (*m_pNfa)[index];
}

COMMONSC CNfa::CNfa(const CNfa &other)
{
	m_pNfa = new std::vector<CNfaRow>;
	*this = other;
}
COMMONSC CNfa& CNfa::operator=(const CNfa &other)
{
	*this->m_pNfa = *other.m_pNfa;
	return *this;
}

COMMONSC void CNfa::PopBack()
{
	m_pNfa->pop_back();
}

COMMONSC void CNfa::PushBack(const CNfaRow &row)
{
	m_pNfa->push_back(row);
}

COMMONSC CDfaRow::CDfaRow()
	: m_nFlag(NORMAL)
{
	std::fill(m_pDest, m_pDest + CHARSETSIZE, size_t(-1));
}

COMMONSC CDfaRow::~CDfaRow()
{
	
}
COMMONSC CDfaRow::CDfaRow(const CDfaRow &other)
{
	*this = other;
}

COMMONSC CDfaRow& CDfaRow::operator=(const CDfaRow &other)
{
	CopyMemory(m_pDest, other.m_pDest, CHARSETSIZE * sizeof(size_t));
	m_nFlag = other.m_nFlag;
	return *this;
}

COMMONSC size_t& CDfaRow::operator[](size_t index)
{
	return m_pDest[index];
}

COMMONSC void CDfaRow::SetFlag(size_t nFlag)
{
	m_nFlag = nFlag;
}

COMMONSC size_t CDfaRow::GetFlag()
{
	return m_nFlag;
}

COMMONSC CDfa::CDfa()
{
	m_pDfa = new std::vector<CDfaRow>;
}

COMMONSC CDfa::~CDfa()
{
	delete m_pDfa;
}

COMMONSC CDfa::CDfa(const CDfa &other)
{
	m_pDfa = new std::vector<CDfaRow>;
	*this = other;
}

COMMONSC CDfa& CDfa::operator=(const CDfa &other)
{
	*m_pDfa = *other.m_pDfa;
	return *this;
}

COMMONSC void CDfa::Reserve(size_t _Count)
{
	m_pDfa->reserve(_Count);
}

COMMONSC void CDfa::Resize(size_t _Newsize)
{
	m_pDfa->resize(_Newsize);
}

COMMONSC size_t CDfa::Size() const
{
	return m_pDfa->size();
}


COMMONSC CDfaRow &CDfa::Back()
{
	return m_pDfa->back();
}

COMMONSC CNfaChain::CNfaChain()
{
	m_pChain = new std::vector<CNfa>;
}

COMMONSC CNfaChain::~CNfaChain()
{
	delete m_pChain;
}

COMMONSC CNfaChain::CNfaChain(const CNfaChain &other)
{
	m_pChain = new std::vector<CNfa>;
	*this = other;
}

COMMONSC const CNfaChain& CNfaChain::operator = (const CNfaChain &other)
{
	*m_pChain = *other.m_pChain;
	return *this;
}

COMMONSC size_t CNfaChain::Size() const
{
	return m_pChain->size();
}
COMMONSC CNfa& CNfaChain::Back()
{
	return m_pChain->back();
}

COMMONSC void CNfaChain::PushBack(CNfa &cnfa)
{
	m_pChain->push_back(cnfa);
}

COMMONSC CNfa& CNfaChain::operator[](size_t nIdx)
{
	return (*m_pChain)[nIdx];
}

COMMONSC const CNfa& CNfaChain::operator[](size_t nIdx) const
{
	return (*m_pChain)[nIdx];
}


COMMONSC CDfaRow& CDfa::operator[](size_t index)
{
	return (*m_pDfa)[index];
}

COMMONSC CNfaTree::CNfaTree()
{
	m_pTree = new std::vector<CNfaChain>;
}

COMMONSC CNfaTree::~CNfaTree()
{
	delete m_pTree;
}

COMMONSC CNfaTree::CNfaTree(const CNfaTree &other)
{
	m_pTree = new std::vector<CNfaChain>;
	*this = other;
}

COMMONSC const CNfaTree& CNfaTree::operator = (const CNfaTree &other)
{
	*m_pTree = *other.m_pTree;
	return *this;
}

COMMONSC size_t CNfaTree::Size() const
{
	return m_pTree->size();
}

COMMONSC CNfaChain& CNfaTree::Back()
{
	return m_pTree->back();
}
COMMONSC void CNfaTree::PushBack(CNfaChain &cnfachain)
{
	m_pTree->push_back(cnfachain);
}
CNfaChain& CNfaTree::operator[](size_t nIdx)
{
	return (*m_pTree)[nIdx];
}

const CNfaChain& CNfaTree::operator[](size_t nIdx) const
{
	return (*m_pTree)[nIdx];
}

COMMONSC CSnortRule::CSnortRule()
	: m_nSid(0)
{
	m_pOptions = new std::vector<RULEOPTION*>;
}

COMMONSC CSnortRule::CSnortRule(const CSnortRule &other)
{
	m_pOptions = new std::vector<RULEOPTION*>;
	*this = other;
}
COMMONSC CSnortRule::~CSnortRule()
{
	delete m_pOptions;
}

COMMONSC void CSnortRule::SetSid(size_t sid)
{
	m_nSid = sid;
}

COMMONSC size_t CSnortRule::GetSid() const
{
	return m_nSid;
}

COMMONSC void CSnortRule::SetFlag(size_t flag)
{
	m_nFlag = flag;
}

COMMONSC size_t CSnortRule::GetFlag() const
{
	return m_nFlag;
}

COMMONSC void CSnortRule::PushBack(RULEOPTION* ruleoption)
{
	m_pOptions->push_back(ruleoption);
}
COMMONSC void CSnortRule::PopBack()
{
	m_pOptions->pop_back();
}

COMMONSC size_t CSnortRule::Size() const
{
	return m_pOptions->size();
}

COMMONSC RULEOPTION*& CSnortRule::operator[](size_t nIdx) const
{
	return (*m_pOptions)[nIdx];
}
