#include "stdafx.h"
#include <hwprj\nfa.h>

typedef NFAROWARY::iterator			NFAROWARY_ITER;
typedef NFAROWARY::const_iterator	NFAROWARY_CITER;

NFAHDR CNfa::CNfa()
{
	m_pNfa = new NFAROWARY;
}

NFAHDR CNfa::~CNfa()
{
	delete m_pNfa;
}

NFAHDR CNfa::CNfa(const CNfa &other)
{
	m_pNfa = new NFAROWARY;
	*this = other;
}

NFAHDR CNfa& CNfa::operator=(const CNfa &other)
{
	*this->m_pNfa = *other.m_pNfa;
	return *this;
}

NFAHDR void CNfa::Reserve(ulong _Count)
{
	m_pNfa->reserve(_Count);
}

NFAHDR void CNfa::Shrink()
{
	m_pNfa->shrink_to_fit();
}

NFAHDR void CNfa::Resize(ulong _Newsize)
{
	m_pNfa->resize(_Newsize);
	for (NFAROWARY_ITER i = m_pNfa->begin(); i != m_pNfa->end(); ++i)
	{
		i->Resize(SC_CHARSETSIZE);
	}
}

NFAHDR ulong CNfa::Size() const
{
	return m_pNfa->size();
}

NFAHDR void CNfa::Clear()
{
	m_pNfa->clear();
}

NFAHDR CNfaRow& CNfa::Back()
{
	return m_pNfa->back();
}

NFAHDR CNfaRow& CNfa::operator[](ulong index)
{
	return (*m_pNfa)[index];
}

NFAHDR const CNfaRow& CNfa::operator[](ulong index) const
{
	return (*m_pNfa)[index];
}

NFAHDR void CNfa::PopBack()
{
	m_pNfa->pop_back();
}

NFAHDR void CNfa::PushBack(const CNfaRow &row)
{
	m_pNfa->push_back(row);
}

NFAHDR void CNfa::SortAll()
{
	for(NFAROWARY_ITER i = m_pNfa->begin(); i != m_pNfa->end(); ++i)
	{
		i->SortAll();
	}
}

NFAHDR void CNfa::Dump(const char *pFile) const
{
	ulong stateNum = m_pNfa->size();
	std::ofstream fout(pFile);
	fout << "\t";
	for(ulong t = 0; t < 257; ++t)
	{
		fout << t << "\t";
	}
	fout << std::endl;
	for(ulong i = 0; i < stateNum; ++i)
	{
		fout << i << "\t";
		const CNfaRow &row = (*m_pNfa)[i];
		for(ulong j = 0; j < 257; ++j)
		{
			ulong nCnt = row.DestCnt(j);
			if(nCnt == 0)
			{
				fout << -1 << "\t";
			}
			else
			{
				for(ulong k = 0; k < nCnt; ++k)
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
