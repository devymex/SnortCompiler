/**
**  @file        common.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CVectorUnsigned, CCString
**
*/

#include "stdafx.h"
#include "common.h"

COMMONSC CVectorUnsigned::CVectorUnsigned()
{
	m_pSet = new std::vector<size_t>;
}

COMMONSC CVectorUnsigned::~CVectorUnsigned()
{
	delete m_pSet;
}

COMMONSC CVectorUnsigned::CVectorUnsigned(const CVectorUnsigned &other)
{
	m_pSet = new std::vector<size_t>;
	*this = other;
}

COMMONSC const CVectorUnsigned& CVectorUnsigned::operator = (const CVectorUnsigned &other)
{
	*m_pSet = *other.m_pSet;
	return *this;
}

COMMONSC size_t& CVectorUnsigned::operator[](size_t nIdx)
{
	return (*m_pSet)[nIdx];
}

COMMONSC const size_t& CVectorUnsigned::operator[](size_t nIdx) const
{
	return (*m_pSet)[nIdx];
}

COMMONSC bool CVectorUnsigned::operator == (const CVectorUnsigned &other)
{
	return *m_pSet == *other.m_pSet;
}

COMMONSC size_t CVectorUnsigned::Size() const
{
	return m_pSet->size();
}

COMMONSC void CVectorUnsigned::PopBack()
{
	m_pSet->pop_back();
}

COMMONSC void CVectorUnsigned::PushBack(size_t nState)
{
	m_pSet->push_back(nState);
}

COMMONSC void CVectorUnsigned::Reserve(size_t nCount)
{
	m_pSet->reserve(nCount);
}

COMMONSC void CVectorUnsigned::Resize(size_t nSize)
{
	m_pSet->resize(nSize);
}

COMMONSC size_t& CVectorUnsigned::Back()
{
	return m_pSet->back();
}

COMMONSC void CVectorUnsigned::Sort()
{
	std::sort(m_pSet->begin(), m_pSet->end());
}

COMMONSC void CVectorUnsigned::Unique()
{
	Sort();
	m_pSet->erase(std::unique(m_pSet->begin(), m_pSet->end()), m_pSet->end());
}

COMMONSC void CVectorUnsigned::Fill(size_t _Val)
{
	std::fill(m_pSet->begin(), m_pSet->end(), _Val);
}

COMMONSC void CVectorUnsigned::Clear()
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
	m_pNfa->shrink_to_fit();
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

COMMONSC size_t CNfa::GetDfaTermsNum() const
{
	return m_DfaTerms->size();
}

COMMONSC CNfa::DFATERMS CNfa::GetDfaTerms(size_t num) const
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

COMMONSC void CNfa::Dump(const char *pFile) const
{
	size_t stateNum = m_pNfa->size();
	std::ofstream fout(pFile);
	fout << "\t";
	for(size_t t = 0; t < 257; ++t)
	{
		fout << t << "\t";
	}
	fout << std::endl;
	for(size_t i = 0; i < stateNum; ++i)
	{
		fout << i << "\t";
		const CNfaRow &row = (*m_pNfa)[i];
		for(size_t j = 0; j < 257; ++j)
		{
			size_t nCnt = row.DestCnt(j);
			if(nCnt == 0)
			{
				fout << -1 << "\t";
			}
			else
			{
				for(size_t k = 0; k < nCnt; ++k)
				{
					fout << row.GetDest(j, k) << ", ";
				}
				fout << "\t";
			}
		}
		fout << std::endl;
	}
	fout.close();
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

COMMONSC const char* CCString::GetStr()
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

COMMONSC void CRegChain::PushBack(const CCString &pcreStr)
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

COMMONSC void CRegChain::PushBackSig(const SIGNATURE &signature)
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

COMMONSC CRuleOption::CRuleOption()
	: m_nFlag(0)
{
	m_pPattern = new std::string;
}

COMMONSC CRuleOption::CRuleOption(const CRuleOption &other)
{
	m_pPattern = new std::string;
	*this = other;
}

COMMONSC CRuleOption::~CRuleOption()
{
	delete m_pPattern;
}

COMMONSC const CRuleOption& CRuleOption::operator=(const CRuleOption &other)
{
	*m_pPattern = *other.m_pPattern;
	m_nFlag = other.m_nFlag;
	return *this;
}

COMMONSC size_t CRuleOption::GetPattern(LPSTR lpStr, size_t nLen) const
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

COMMONSC void CRuleOption::SetPattern(LPCSTR lpStr)
{
	*m_pPattern = lpStr;
}

COMMONSC size_t CRuleOption::GetFlag() const
{
	return m_nFlag;
}

COMMONSC void CRuleOption::SetFlag(size_t nFlag)
{
	m_nFlag = nFlag;
}

COMMONSC void CRuleOption::AddFlag(size_t nFlag)
{
	m_nFlag |= nFlag;
}

COMMONSC BOOL CRuleOption::TestFlag(size_t nFlag) const
{
	return ((m_nFlag & nFlag) != 0);
}

COMMONSC CSnortRule::CSnortRule()
	: m_nSid(0), m_nFlag(0)
{
	m_pOptions = new std::vector<CRuleOption*>;
}

COMMONSC CSnortRule::CSnortRule(const CSnortRule &other)
{
	m_pOptions = new std::vector<CRuleOption*>;
	*this = other;
}

COMMONSC const CSnortRule& CSnortRule::operator = (const CSnortRule &other)
{
	m_nSid = other.m_nSid;
	m_nFlag = other.m_nFlag;
	*m_pOptions = *other.m_pOptions;
	return *this;
}

COMMONSC CSnortRule::~CSnortRule()
{
	std::vector<CRuleOption*> &opts = *m_pOptions;
	for (std::vector<CRuleOption*>::iterator i = opts.begin(); i != opts.end(); ++i)
	{
		delete *i;
	}
	opts.clear();
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

COMMONSC void CSnortRule::PushBack(CRuleOption* ruleoption)
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

COMMONSC CRuleOption* CSnortRule::operator[](size_t nIdx) const
{
	return (*m_pOptions)[nIdx];
}

