#include "stdafx.h"
#include "snortcommon.h"

COMMONSC CNfaRow::CNfaRow()
{
	for (size_t i = 0; i < CHARSETSIZE; ++i)
	{
		m_pDestSet[i] = new std::vector<size_t>;
	}
}

COMMONSC CNfaRow::~CNfaRow()
{
	for (size_t i = 0; i < CHARSETSIZE; ++i)
	{
		delete m_pDestSet[i];
	}
}

COMMONSC size_t CNfaRow::GetSetSize(size_t nChar)
{
	return m_pDestSet[nChar]->size();
}

COMMONSC void CNfaRow::GetSet(size_t nChar, size_t *pSet)
{
	for (std::vector<size_t>::iterator i = m_pDestSet[nChar]->begin(); i != m_pDestSet[nChar]->end(); ++i)
	{
		*(pSet++) = *i;
	}
}

COMMONSC std::vector<size_t> &CNfaRow::operator[](size_t nChar)
{
	return *(m_pDestSet[nChar]);
}

COMMONSC CNfaRow::CNfaRow(const CNfaRow &other)
{
	for (size_t i = 0; i < CHARSETSIZE; ++i)
	{
		m_pDestSet[i] = new std::vector<size_t>;
	}
	*this = other;
}

COMMONSC CNfaRow& CNfaRow::operator=(const CNfaRow &other)
{
	for (size_t i = 0; i < CHARSETSIZE; ++i)
	{
		*m_pDestSet[i] = *other.m_pDestSet[i];
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

COMMONSC void CNfa::reserve(size_t _Count)
{
	m_pNfa->reserve(_Count);
}

COMMONSC void CNfa::resize(size_t _Newsize)
{
	m_pNfa->resize(_Newsize);
}

COMMONSC size_t CNfa::size() const
{
	return m_pNfa->size();
}

COMMONSC CNfaRow &CNfa::back()
{
	return m_pNfa->back();
}

COMMONSC CNfaRow &CNfa::operator[](size_t index)
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

COMMONSC void CNfa::pop_back()
{
	m_pNfa->pop_back();
}