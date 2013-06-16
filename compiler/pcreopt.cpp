#include "stdafx.h"
#include "pcreopt.h"
#include "comprule.h"

CPcreOption::CPcreOption()
{
}

CPcreOption::CPcreOption(const CPcreOption &other)
	: CRuleOption(other)
{
}

CPcreOption::~CPcreOption()
{
}

CPcreOption& CPcreOption::operator = (const CPcreOption &other)
{
	other;

	return *this;
}

void CPcreOption::FromPattern(pcstr &pBeg, pcstr &pEnd)
{
	CRuleOption::FromPattern(pBeg, pEnd);
	if (HasFlags(CRuleOption::HASNOT))
	{
		return;
	}

	pcstr pPcreBeg = std::find(pBeg, pEnd, '/');
	pcstr pPcreEnd = pEnd;
	for(; *pPcreEnd != '/'; --pPcreEnd);

	if (pPcreBeg >= pPcreEnd)
	{
		TTHROW(TI_INVALIDDATA);
	}

	STRING strTmp = STRING(pPcreEnd + 1, pEnd);

	for(STRING_ITER j = strTmp.begin(); j != strTmp.end(); ++j)
	{
		switch (*j)
		{
		case 'i':
			AddFlags(PF_i);
			continue;
		case 's':
			AddFlags(PF_s);
			continue;
		case 'm':
			AddFlags(PF_m);
			continue;
		case 'x':
			AddFlags(PF_x);
			continue;
		case 'A':
			AddFlags(PF_A);
			continue;
		case 'E':
			AddFlags(PF_E);
			continue;
		case 'G':
			AddFlags(PF_G);
			continue;
		case 'R':
			AddFlags(PF_R);
			continue;
		case 'U':
			AddFlags(PF_U);
			continue;
		case 'B':
			AddFlags(PF_B);
			continue;
		case 'P':
			AddFlags(PF_P);
			continue;
		case 'H':
			AddFlags(PF_H);
			continue;
		case 'M':
			AddFlags(PF_M);
			continue;
		case 'C':
			AddFlags(PF_C);
			continue;
		case 'O':
			AddFlags(PF_O);
			continue;
		case 'I':
			AddFlags(PF_I);
			continue;
		case 'D':
			AddFlags(PF_D);
			continue;
		case 'K':
			AddFlags(PF_K);
			continue;
		case 'S':
			AddFlags(PF_S);
			continue;
		case 'Y':
			AddFlags(PF_Y);
			continue;			
		default:
			AddFlags(0);
		}
	}
}

CRuleOption* CPcreOption::Clone() const
{
	CPcreOption *pNew = null;
	try
	{
		pNew = new CPcreOption(*this);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
	return pNew;
}
