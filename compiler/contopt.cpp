#include "stdafx.h"
#include "contopt.h"


// Regrex metacharacter map
// 0: this char needn't eascape
// 1: this char need to add a backslash to eascape
// 2: this char need to add a backslash and convert to hex to eascape
const BYTE g_MetaChar[256] = {
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	/*
	   !  "  #  $  %  &  '  (  )  *  +  ,  -  .  / */
	0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
	/*
	0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ? */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	/*
	@  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*
	P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _ */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
	/*
	`  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*
	p  q  r  s  t  u  v  w  x  y  z  {  |  }  ~  */
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
	if (this != &other)
	{
		CRuleOption::operator = (other);
		m_nOffset	= other.m_nOffset;
		m_nDepth	= other.m_nDepth;
		m_nDistance	= other.m_nDistance;
		m_nWithin	= other.m_nWithin;
		m_data		= other.m_data;
	}

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

void CContentOption::FromPattern(const CDllString &strPat)
{
	CDllString strTmp = strPat;
	FormatPattern(strTmp);
	if (HasFlags(CRuleOption::HASNOT))
	{
		return;
	}

	m_data.Clear();
	pcstr pBeg = strTmp.Data();
	pcstr pEnd = pBeg + strTmp.Size();
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
					pcstr pNext = pBeg + 1;
					if (pNext == pEnd)
					{
						TTHROW(TI_INVALIDDATA);
					}
					m_data.PushBack((Char2HexBit(*pBeg) << 4) | Char2HexBit(*pNext));
					++pBeg;
				}
			}
		}
		else
		{
			m_data.PushBack(byte(*pBeg));
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
		if (HasFlags(WITHIN) || m_nDepth < int(m_data.Size()))
		{
			TTHROW(TI_INVALIDDATA);
		}
		nMaxSkip = m_nDepth - m_data.Size();
	}
	if (HasFlags(WITHIN))
	{
		if (HasFlags(DEPTH) || m_nWithin < int(m_data.Size()))
		{
			TTHROW(TI_INVALIDDATA);
		}
		nMaxSkip = m_nWithin - m_data.Size();
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
	ssPcre << "\"/";

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

	char code[3] = {0};
	for (ulong i = 0; i < m_data.Size(); ++i)
	{
		byte byCurCode = m_data[i];
		switch (g_MetaChar[byCurCode])
		{
		case 0:
			ssPcre << char(byCurCode);
			break;
		case 1:
			ssPcre << '\\' << char(byCurCode);
			break;
		case 2:
			Hex2UpperCase(byCurCode, code);
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
	ssPcre << '"';

	pcreOpt.FromPattern(CDllString(ssPcre.str().c_str()));
	pcreOpt.AddFlags(CPcreOption::PF_F);
}
