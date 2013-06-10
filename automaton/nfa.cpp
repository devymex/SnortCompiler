#include "stdafx.h"
#include <hwprj/nfa.h>

NFAHDR CNfa::CNfa()
{
	m_pNfa = new std::vector<CNfaRow>;
}

NFAHDR CNfa::~CNfa()
{
	delete m_pNfa;
}

NFAHDR void CNfa::Reserve(ULONG _Count)
{
	m_pNfa->reserve(_Count);
}

NFAHDR void CNfa::Shrink()
{
	m_pNfa->shrink_to_fit();
}

NFAHDR void CNfa::Resize(ULONG _Newsize)
{
	m_pNfa->resize(_Newsize);
	for (std::vector<CNfaRow>::iterator i = m_pNfa->begin(); i != m_pNfa->end(); ++i)
	{
		i->Resize(CHARSETSIZE);
	}
}

NFAHDR ULONG CNfa::Size() const
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

NFAHDR CNfaRow& CNfa::operator[](ULONG index)
{
	return (*m_pNfa)[index];
}

NFAHDR const CNfaRow& CNfa::operator[](ULONG index) const
{
	return (*m_pNfa)[index];
}

NFAHDR CNfa::CNfa(const CNfa &other)
{
	m_pNfa = new std::vector<CNfaRow>;
	*this = other;
}
NFAHDR CNfa& CNfa::operator=(const CNfa &other)
{
	*this->m_pNfa = *other.m_pNfa;
	return *this;
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
	for(std::vector<CNfaRow>::iterator i = m_pNfa->begin(); i != m_pNfa->end(); ++i)
	{
		i->SortAll();
	}
}

NFAHDR void CNfa::Dump(const char *pFile) const
{
	ULONG stateNum = m_pNfa->size();
	std::ofstream fout(pFile);
	fout << "\t";
	for(ULONG t = 0; t < 257; ++t)
	{
		fout << t << "\t";
	}
	fout << std::endl;
	for(ULONG i = 0; i < stateNum; ++i)
	{
		fout << i << "\t";
		const CNfaRow &row = (*m_pNfa)[i];
		for(ULONG j = 0; j < 257; ++j)
		{
			ULONG nCnt = row.DestCnt(j);
			if(nCnt == 0)
			{
				fout << -1 << "\t";
			}
			else
			{
				for(ULONG k = 0; k < nCnt; ++k)
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
