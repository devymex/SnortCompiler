#include "stdafx.h"
#include "contopt.h"


// Regrex metacharacter map
// 0: this char needn't eascape
// 1: this char need to add a backslash to eascape
// 2: this char need to add a backslash and convert to hex to eascape
BYTE g_MetaChar[256] = {
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

CContentOption::CContentOption()
	: m_nOffset		(-1)
	, m_nDepth		(-1)
	, m_nDistance	(-1)
	, m_nWithin		(-1)
{
}

CContentOption::CContentOption(const CContentOption &other)
	: CRuleOption(other)
	, m_nOffset		(other.m_nOffset)
	, m_nDepth		(other.m_nDepth)
	, m_nDistance	(other.m_nDistance)
	, m_nWithin		(other.m_nWithin)
	, m_data		(other.m_data)
{
}

CContentOption::~CContentOption()
{
}

CContentOption& CContentOption::operator = (const CContentOption &other)
{
	m_nOffset	= other.m_nOffset;
	m_nDepth	= other.m_nDepth;
	m_nDistance	= other.m_nDistance;
	m_nWithin	= other.m_nWithin;
	m_data		= other.m_data;

	return *this;
}

inline char HexBit2UpperCase(byte hex)
{
	if (hex < 10)
	{
		return (hex + '0');
	}
	if (hex < 16)
	{
		return (hex - 10 + 'A');
	}
	TTHROW(TI_BADPARAM);
}

inline void Hex2UpperCase(byte hex, char code[2])
{
	code[0] = HexBit2UpperCase(hex >> 4);
	code[1] = HexBit2UpperCase(hex & 0x0F);
}

byte Char2HexBit(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	if (c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}
	if (c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}
	TTHROW(TI_INVALIDDATA);
}

void CContentOption::FromPattern(pcstr &pBeg, pcstr &pEnd)
{
	CRuleOption::FromPattern(pBeg, pEnd);
	if (HasFlags(CRuleOption::HASNOT))
	{
		return;
	}

	m_data.clear();
	for (; pBeg != pEnd; ++pBeg)
	{
		if (*pBeg == '|')
		{
			for (++pBeg; ; ++pBeg)
			{
				if (pBeg == pEnd)
				{
					TTHROW(TI_INVALIDDATA);
				}
				if (*pBeg == '|')
				{
					break;
				}
				if (*pBeg != ' ')
				{
					if (pEnd - pBeg < 1)
					{
						TTHROW(TI_INVALIDDATA);
					}
					m_data.push_back((Char2HexBit(*(pBeg++)) << 4) |
						Char2HexBit(*pBeg));
				}
			}
		}
		else
		{
			m_data.push_back(byte(*pBeg));
		}
	}
}

CRuleOption* CContentOption::Clone() const
{
	CContentOption *pNew = null;
	try
	{
		pNew = new CContentOption(*this);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	return pNew;
}

/*
**	this function transforms content to pcre
**	based on the content constraints: distance, within, offset, depth, nocase
**
**	@param pContent	 pointer to the original content option
**	@param pcreStr		the transformed pcre
**
**	@return
**	@retval  0 function successful
**	@retval -1 fatal error
*/
void CContentOption::ToPcre(CPcreOption &pcreOpt) const
{
	int nMinSkip = 0, nMaxSkip = -1;

	if (HasFlags(OFFSET))
	{
		if (HasFlags(DISTANCE) || m_nOffset < 0)
		{
			TTHROW(TI_INVALIDDATA);
		}
		nMinSkip = m_nOffset;
	}
	if (HasFlags(DISTANCE))
	{
		if (HasFlags(OFFSET))
		{
			TTHROW(TI_INVALIDDATA);
		}
		if (m_nDistance < 0)
		{
			TTHROW(TI_UNSUPPORT);
		}
		nMinSkip = m_nDistance;
	}

	if (HasFlags(DEPTH))
	{
		if (HasFlags(WITHIN) || m_nDepth < int(m_data.size()))
		{
			TTHROW(TI_INVALIDDATA);
		}
		nMaxSkip = m_nDepth - m_data.size();
	}
	if (HasFlags(WITHIN))
	{
		if (HasFlags(DEPTH) || m_nWithin < int(m_data.size()))
		{
			TTHROW(TI_INVALIDDATA);
		}
		nMaxSkip = m_nWithin - m_data.size();
	}

	if (nMinSkip > 0 && nMaxSkip >= 0)
	{
		nMaxSkip += nMinSkip;
	}

	if (nMaxSkip >= 0 && nMaxSkip < nMinSkip)
	{
		TTHROW(TI_INVALIDDATA);
	}

	std::stringstream ssPcre;
	ssPcre << '/';

	// If has constraint
	if (nMinSkip > 0 || nMaxSkip >= 0)
	{
		ssPcre << '^';

		// If nMaxSkip > 0
		if (nMaxSkip != 0)
		{
			ssPcre << ".{" << nMinSkip << ',';
			// If nMaxSkip has a limit
			if (nMaxSkip > 0)
			{
				ssPcre << nMaxSkip;
			}
			ssPcre << '}';
		}
	}
	else
	{
		ssPcre << ".*";
	}

	char code[3] = {0};
	for (BYTEARY_CITER i = m_data.cbegin(); i != m_data.cend(); ++i)
	{
		switch (g_MetaChar[*i])
		{
		case 0:
			ssPcre << char(*i);
			break;
		case 1:
			ssPcre << '\\' << char(*i);
			break;
		case 2:
			Hex2UpperCase(*i, code);
			ssPcre << "\\x" << code;
			break;
		}
	}

	if(HasFlags(NOCASE))
	{
		ssPcre << "/si";
	}
	else
	{
		ssPcre << "/s";
	}

	if (HasFlags(DISTANCE | WITHIN))
	{
		ssPcre << 'R';
	}

	STRING strPcre = ssPcre.str();
	pcstr pBeg = strPcre.begin()._Ptr;
	pcstr pEnd = strPcre.end()._Ptr;
	pcreOpt.FromPattern(pBeg, pEnd);
}

void CContentOption::ExtractSignatures(CSignatures &sigs) const
{
	int nBufSize = int(m_data.size());
	const byte *pBuf = m_data.data();
	byte sig[4];
	for (int i = 0; i < nBufSize - 3; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			sig[j] = byte(tolower(pBuf[i + j]));
		}
		sigs.PushBack(*(SIGNATURE*)sig);
	}
	sigs.Unique();
}
