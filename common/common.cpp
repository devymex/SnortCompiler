#include "stdafx.h"
#include "common.h"

RULEOPTION::RULEOPTION()
{
	m_pPattern = new std::string;
}

RULEOPTION::RULEOPTION(const RULEOPTION &other)
{
	m_pPattern = new std::string;
	*this = other;
}

RULEOPTION::~RULEOPTION()
{
	delete m_pPattern;
}

const RULEOPTION& RULEOPTION::operator=(const RULEOPTION &other)
{
	*m_pPattern = *other.m_pPattern;
	nFlags = other.nFlags;
	return *this;
}

size_t RULEOPTION::GetPattern(LPSTR lpStr, size_t nLen) const
{
	if (lpStr == NULL || nLen == 0)
	{
		return m_pPattern->length();
	}
	if (nLen > m_pPattern->length())
	{
		nLen = m_pPattern->length();
	}
	CopyMemory(lpStr, &(*m_pPattern)[0], nLen);
	return nLen;
}

void RULEOPTION::SetPattern(LPCSTR lpStr)
{
	*m_pPattern = lpStr;
}

COMMONSC CVectorNumber::CVectorNumber()
{
	m_pSet = new std::vector<size_t>;
}

COMMONSC CVectorNumber::~CVectorNumber()
{
	delete m_pSet;
}

COMMONSC CVectorNumber::CVectorNumber(const CVectorNumber &other)
{
	m_pSet = new std::vector<size_t>;
	*this = other;
}

COMMONSC const CVectorNumber& CVectorNumber::operator = (const CVectorNumber &other)
{
	*m_pSet = *other.m_pSet;
	return *this;
}

COMMONSC size_t& CVectorNumber::operator[](size_t nIdx)
{
	return (*m_pSet)[nIdx];
}

COMMONSC const size_t& CVectorNumber::operator[](size_t nIdx) const
{
	return (*m_pSet)[nIdx];
}

COMMONSC bool CVectorNumber::operator == (const CVectorNumber &other)
{
	return *m_pSet == *other.m_pSet;
}

COMMONSC const size_t CVectorNumber::Size() const
{
	return m_pSet->size();
}

COMMONSC void CVectorNumber::PopBack()
{
	m_pSet->pop_back();
}

COMMONSC void CVectorNumber::PushBack(size_t nState)
{
	m_pSet->push_back(nState);
}

COMMONSC void CVectorNumber::Reserve(size_t nCount)
{
	m_pSet->reserve(nCount);
}

COMMONSC void CVectorNumber::Resize(size_t nSize)
{
	m_pSet->resize(nSize);
}

COMMONSC size_t& CVectorNumber::Back()
{
	return m_pSet->back();
}

COMMONSC void CVectorNumber::Sort()
{
	std::sort(m_pSet->begin(), m_pSet->end());
}

COMMONSC void CVectorNumber::Unique()
{
	Sort();
	m_pSet->erase(std::unique(m_pSet->begin(), m_pSet->end()), m_pSet->end());
}

COMMONSC void CVectorNumber::Fill(size_t _Val)
{
	std::fill(m_pSet->begin(), m_pSet->end(), _Val);
}

COMMONSC void CVectorNumber::Clear()
{
	m_pSet->clear();
}

COMMONSC CNfaRow::CNfaRow(size_t nSize)
	: m_nSize(nSize)
{
	m_pDestSet = new std::vector<size_t>[m_nSize];
}

COMMONSC CNfaRow::~CNfaRow()
{
	delete []m_pDestSet;
}

COMMONSC size_t CNfaRow::Size() const
{
	return m_nSize;
}

COMMONSC void CNfaRow::Resize(size_t nSize)
{
	if (m_nSize != nSize)
	{
		delete []m_pDestSet;
		m_pDestSet = new std::vector<size_t>[nSize];
		m_nSize = nSize;
	}
}

COMMONSC size_t CNfaRow::DestCnt(size_t nCol) const
{
	return m_pDestSet[nCol].size();
}

COMMONSC size_t& CNfaRow::GetDest(size_t nCol, size_t nIdx)
{
	return m_pDestSet[nCol][nIdx];
}

COMMONSC const size_t& CNfaRow::GetDest(size_t nCol, size_t nIdx) const
{
	return m_pDestSet[nCol][nIdx];
}

COMMONSC size_t* CNfaRow::GetCol(size_t nCol)
{
	return m_pDestSet[nCol].data();
}

COMMONSC const size_t* CNfaRow::GetCol(size_t nCol) const
{
	return m_pDestSet[nCol].data();
}

COMMONSC void CNfaRow::CopyCol(size_t nCol, size_t *pOut) const
{
	std::vector<size_t> &col = m_pDestSet[nCol];
	memcpy(pOut, col.data(), col.size() * sizeof(size_t));
}

COMMONSC void CNfaRow::AddDest(size_t nCol, size_t nDest)
{
	m_pDestSet[nCol].push_back(nDest);
}

COMMONSC void CNfaRow::SortAll()
{
	for (size_t i = 0; i < m_nSize; ++i)
	{
		std::sort(m_pDestSet[i].begin(), m_pDestSet[i].end());
	}
}

COMMONSC CNfaRow::CNfaRow(const CNfaRow &other)
	: m_nSize(0), m_pDestSet(0)
{
	*this = other;
}

COMMONSC CNfaRow& CNfaRow::operator=(const CNfaRow &other)
{
	Resize(other.m_nSize);
	for (size_t i = 0; i < m_nSize; ++i)
	{
		m_pDestSet[i] = other.m_pDestSet[i];
	}

	return *this;
}

COMMONSC CNfa::CNfa()
{
	m_pNfa = new std::vector<CNfaRow>;
	m_pPcre = new std::string;
	m_DfaTerms = new std::vector<DFATERMS>;
}

COMMONSC CNfa::~CNfa()
{
	delete m_pNfa;
	delete m_pPcre;
	delete m_DfaTerms;
}

//COMMONSC size_t CNfa::GetRowNum(void)
//{
// return m_pNfa->size();
//}

COMMONSC void CNfa::Reserve(size_t _Count)
{
	m_pNfa->reserve(_Count);
}

COMMONSC void CNfa::Shrink()
{
//	m_pNfa->shrink_to_fit();
}

COMMONSC void CNfa::Resize(size_t _Newsize)
{
	//CTimer t;
	m_pNfa->resize(_Newsize);
	for (std::vector<CNfaRow>::iterator i = m_pNfa->begin(); i != m_pNfa->end(); ++i)
	{
		i->Resize(CHARSETSIZE);
	}
	//g_dTimer += t.Reset();
}

COMMONSC size_t CNfa::Size() const
{
	return m_pNfa->size();
}

COMMONSC void CNfa::Clear()
{
	m_pNfa->clear();
	m_pPcre->clear();
	m_DfaTerms->clear();
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
	m_pPcre = new std::string;
	*this = other;
}
COMMONSC CNfa& CNfa::operator=(const CNfa &other)
{
	*this->m_pNfa = *other.m_pNfa;
	*m_pPcre = *other.m_pPcre;
	*m_DfaTerms = *other.m_DfaTerms;
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

COMMONSC void CNfa::SetPcre(const char* lpPcre)
{
	*m_pPcre = lpPcre;
}

COMMONSC const char* CNfa::GetPcre() const
{
	return m_pPcre->c_str();
}
COMMONSC  void CNfa::PushDfaTerms(DFATERMS dfaTerms)
{
	m_DfaTerms->push_back(dfaTerms);
}

COMMONSC size_t CNfa::GetDfaTermsNum()
{
	return m_DfaTerms->size();
}

COMMONSC CNfa::DFATERMS CNfa::GetDfaTerms(size_t num)
{
	return (*m_DfaTerms)[num];
}

COMMONSC void CNfa::SortAll()
{
	for(std::vector<CNfaRow>::iterator i = m_pNfa->begin(); i != m_pNfa->end(); ++i)
	{
		i->SortAll();
	}
}

COMMONSC CDfaRow::CDfaRow(size_t col)
	: m_nFlag(NORMAL), m_nColNum(col)
{
	m_pDest = new std::vector<STATEID>;
	m_pDest->resize(m_nColNum);
	Fill(STATEID(-1));
}

COMMONSC void CDfaRow::Fill(STATEID _Val)
{
	std::fill(m_pDest->begin(), m_pDest->end(), _Val);
}

COMMONSC CDfaRow::~CDfaRow()
{
	delete m_pDest;
}
COMMONSC CDfaRow::CDfaRow(const CDfaRow &other)
{
	m_pDest = new std::vector<STATEID>;
	*this = other;
}

COMMONSC CDfaRow& CDfaRow::operator=(const CDfaRow &other)
{
	m_nFlag = other.m_nFlag;
	m_nColNum = other.m_nColNum;
	*m_pDest = *other.m_pDest;
	return *this;
}

COMMONSC STATEID& CDfaRow::operator[](STATEID index)
{
	return (*m_pDest)[index];
}

COMMONSC const STATEID& CDfaRow::operator[](STATEID index) const
{
	return (*m_pDest)[index];
}

COMMONSC void CDfaRow::SetFlag(size_t nFlag)
{
	m_nFlag = nFlag;
}

COMMONSC size_t CDfaRow::GetFlag() const
{
	return m_nFlag;
}

COMMONSC size_t CDfaRow::GetColNum() const
{
	return m_nColNum;
}

COMMONSC CCString::CCString()
{
	m_pString = new std::string;
}

COMMONSC CCString::CCString(const char *pStr)
{
	m_pString = new std::string(pStr);
}

COMMONSC void CCString::Append(const char* pChar)
{
	m_pString->append(pChar);;
}

COMMONSC CCString::~CCString()
{
	delete m_pString;
}
	
COMMONSC CCString::CCString(const CCString &other)
{
	m_pString = new std::string;
	*this = other;
}

COMMONSC CCString& CCString::operator = (const CCString &other)
{
	*this->m_pString = *other.m_pString;
	return *this;
}
COMMONSC char CCString::operator[](size_t nIdx) const
{
	return (*m_pString)[nIdx];
}
	
COMMONSC const size_t CCString::Size() const
{
	return m_pString->size();
}

COMMONSC void CCString::PushBack(const char nChar)
{
	m_pString->push_back(nChar);
}
COMMONSC char CCString::Back() const
{
	return m_pString->back();
}

COMMONSC const char* CCString::GetString()
{
	return m_pString->c_str();
}

COMMONSC const char* CCString::C_Str()
{
	return m_pString->c_str();
}

COMMONSC void CCString::Clear()
{
	m_pString->clear();
}
COMMONSC bool CCString::Empty()
{
	return m_pString->empty();
}

COMMONSC CRegChain::CRegChain()
{
	m_pRegList = new std::vector<CCString>;
	m_pSigList = new std::vector<SIGNATURE>;
}

COMMONSC CRegChain::~CRegChain()
{
	delete m_pRegList;
	delete m_pSigList;
}

COMMONSC CRegChain::CRegChain(const CRegChain &other)
{
	m_pRegList = new std::vector<CCString>;
	m_pSigList = new std::vector<SIGNATURE>;
	*this = other;
}

COMMONSC size_t CRegChain::Size() const
{
	return m_pRegList->size();
}
	
COMMONSC CCString& CRegChain::Back() const
{
	return m_pRegList->back();
}
COMMONSC void CRegChain::PushBack(CCString &pcreStr)
{
	m_pRegList->push_back(pcreStr);
}
COMMONSC CCString& CRegChain::operator[](size_t nIdx)
{
	return (*m_pRegList)[nIdx];
}
COMMONSC const CRegChain& CRegChain::operator = (const CRegChain &other)
{
	*this->m_pRegList = *other.m_pRegList;
	*this->m_pSigList = *other.m_pSigList;
	return *this;
}

COMMONSC void CRegChain::Resize(size_t nSize)
{
	m_pRegList->resize(nSize);
}

COMMONSC size_t CRegChain::GetSigCnt() const
{
	return m_pSigList->size();
}

COMMONSC void CRegChain::PushBackSig(SIGNATURE &signature)
{
	m_pSigList->push_back(signature);
}

COMMONSC SIGNATURE& CRegChain::GetSig(size_t nIdx) const
{
	return (*m_pSigList)[nIdx];
}

COMMONSC void CRegChain::Unique()
{
	std::sort(m_pSigList->begin(), m_pSigList->end());
	m_pSigList->erase(std::unique(m_pSigList->begin(), m_pSigList->end()), m_pSigList->end());
}

COMMONSC void CRegChain::ClearSigList()
{
	m_pSigList->clear();
}

COMMONSC CRegRule::CRegRule()
{
	m_pRegVec = new std::vector<CRegChain>;
}

COMMONSC CRegRule::~CRegRule()
{
	delete m_pRegVec;
}

COMMONSC CRegRule::CRegRule(const CRegRule &other)
{
	m_pRegVec = new std::vector<CRegChain>;
	*this = other;
}

COMMONSC size_t CRegRule::Size() const
{
	return m_pRegVec->size();
}

COMMONSC CRegChain& CRegRule::Back() const
{
	return m_pRegVec->back();
}

COMMONSC void CRegRule::Reserve(size_t nCount)
{
	m_pRegVec->reserve(nCount);
}
COMMONSC void CRegRule::Resize(size_t nSize)
{
	m_pRegVec->resize(nSize);
}

COMMONSC void CRegRule::PushBack(CRegChain &nRegChain)
{
	m_pRegVec->push_back(nRegChain);
}

COMMONSC CRegChain& CRegRule::operator[](size_t nIdx)
{
	return (*m_pRegVec)[nIdx];
}

COMMONSC const CRegChain& CRegRule::operator[](size_t nIdx) const
{
	return (*m_pRegVec)[nIdx];
}

COMMONSC const CRegRule& CRegRule::operator = (const CRegRule &other)
{
	*this->m_pRegVec = *other.m_pRegVec;
	return *this;
}

COMMONSC CNfaTree::CNfaTree()
{
	m_pTree = new std::vector<CNfa>;
}

COMMONSC CNfaTree::~CNfaTree()
{
	delete m_pTree;
}

COMMONSC CNfaTree::CNfaTree(const CNfaTree &other)
{
	m_pTree = new std::vector<CNfa>;
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

COMMONSC void CNfaTree::Reserve(size_t nCount)
{
	m_pTree->reserve(nCount);
}

COMMONSC void CNfaTree::Resize(size_t nSize)
{
	m_pTree->resize(nSize);
}
COMMONSC CNfa& CNfaTree::Back()
{
	return m_pTree->back();
}
COMMONSC void CNfaTree::PushBack(const CNfa &cnfa)
{
	m_pTree->push_back(cnfa);
}
CNfa& CNfaTree::operator[](size_t nIdx)
{
	return (*m_pTree)[nIdx];
}

const CNfa& CNfaTree::operator[](size_t nIdx) const
{
	return (*m_pTree)[nIdx];
}

COMMONSC CSnortRule::CSnortRule()
	: m_nSid(0), m_nFlag(0)
{
	m_pOptions = new std::vector<RULEOPTION*>;
}

COMMONSC CSnortRule::CSnortRule(const CSnortRule &other)
{
	m_pOptions = new std::vector<RULEOPTION*>;
	*this = other;
}

const CSnortRule& CSnortRule::operator = (const CSnortRule &other)
{
	m_nSid = other.m_nSid;
	m_nFlag = other.m_nFlag;
	*m_pOptions = *other.m_pOptions;
	return *this;
}

COMMONSC CSnortRule::~CSnortRule()
{
	Release();
	delete m_pOptions;
}

void CSnortRule::Release()
{
	std::vector<RULEOPTION*> &opts = *m_pOptions;
	for (std::vector<RULEOPTION*>::iterator i = opts.begin(); i != opts.end(); ++i)
	{
		delete *i;
	}
	opts.clear();
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

COMMONSC RULEOPTION* CSnortRule::operator[](size_t nIdx) const
{
	return (*m_pOptions)[nIdx];
}

COMMONSC void printNfa(const CNfa &nfa)
{
	for (size_t i = 0; i < nfa.Size(); ++i)
	{
		const CNfaRow &row = nfa[i];
		std::cout << i << ": ";
		for (size_t j = 0; j < CHARSETSIZE; ++j)
		{
			size_t nCnt = row.DestCnt(j);
			for (size_t k = 0; k < nCnt; ++k)
			{
				std::cout << "(" << j << "," << row.GetDest(j, k) << ")";
			}
		}
		std::cout << std::endl;
	}
}

