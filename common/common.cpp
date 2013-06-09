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

COMMONSC CSignatures::CSignatures()
{
	m_pSigs = new std::vector<SIGNATURE>;
}
COMMONSC CSignatures::CSignatures(const CSignatures& other)
{
	m_pSigs = new std::vector<SIGNATURE>;
	*this = other;
}
COMMONSC const CSignatures &CSignatures::operator=(const CSignatures &other)
{
	*m_pSigs = *other.m_pSigs;
	return *this;
}

COMMONSC CSignatures::~CSignatures()
{
	delete m_pSigs;
}

COMMONSC const size_t CSignatures::Size() const
{
	return m_pSigs->size();
}

COMMONSC void CSignatures::Resize(size_t nSize)
{
	m_pSigs->resize(nSize);
}

COMMONSC void CSignatures::PushBack(SIGNATURE Sig)
{
	m_pSigs->push_back(Sig);
}

COMMONSC SIGNATURE &CSignatures::operator[](size_t nIdx)
{
	return (*m_pSigs)[nIdx];
}

COMMONSC const SIGNATURE &CSignatures::operator[](size_t nIdx) const
{
	return (*m_pSigs)[nIdx];
}

COMMONSC void CSignatures::Clear()
{
	m_pSigs->clear();
}

COMMONSC void CSignatures::Unique()
{
	std::sort(m_pSigs->begin(), m_pSigs->end());
	m_pSigs->erase(std::unique(m_pSigs->begin(),
		m_pSigs->end()), m_pSigs->end());
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
}

COMMONSC CNfa::~CNfa()
{
	delete m_pNfa;
}

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
	m_pNfa->resize(_Newsize);
	for (std::vector<CNfaRow>::iterator i = m_pNfa->begin(); i != m_pNfa->end(); ++i)
	{
		i->Resize(CHARSETSIZE);
	}
}

COMMONSC size_t CNfa::Size() const
{
	return m_pNfa->size();
}

COMMONSC void CNfa::Clear()
{
	m_pNfa->clear();
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
