#include "stdafx.h"
#include "comprule.h"
#include "pcre\pcre.h"
#include "p2nmain.h"

#include <hwprj/pcreopt.h>

SNORTRULEHDR CPcreOption::CPcreOption()
{
}

SNORTRULEHDR CPcreOption::CPcreOption(const CPcreOption &other)
	: CRuleOption(other)
{
}

SNORTRULEHDR CPcreOption::~CPcreOption()
{
}

SNORTRULEHDR CPcreOption& CPcreOption::operator = (const CPcreOption &other)
{
	other;

	return *this;
}

SNORTRULEHDR void CPcreOption::FromPattern(pcstr &pBeg, pcstr &pEnd)
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

	m_strPcre.Assign(STRING(pPcreBeg, pPcreEnd).c_str());

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

SNORTRULEHDR CRuleOption* CPcreOption::Clone() const
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

SNORTRULEHDR void CPcreOption::PcreToCode(BYTEARY &code) const
{
	int options = 0;
	if (HasFlags(PF_s))
	{
		options |= PCRE_DOTALL;
	}
	if (HasFlags(PF_m))
	{
		options |= PCRE_MULTILINE;
	}
	if (HasFlags(PF_i))
	{
		options |= PCRE_CASELESS;
	}

	const char *error;
	int erroffset;
	pcre *re = pcre_compile(m_strPcre.Data(), options, &error, &erroffset, null);
	if (re == null)
	{
		TTHROW(TI_INVALIDDATA);
	}

	unsigned int size;
	unsigned short name_table_offset;
	size = *((unsigned int*)re + 1);
	name_table_offset = *((unsigned short*)re + 12);
	for (ulong i = 0; i < size - name_table_offset; ++i)
	{
		code.push_back((byte)*((byte*)re + name_table_offset + i));
	}
}

SNORTRULEHDR CDllString& CPcreOption::GetPcreString()
{
	return m_strPcre;
}

SNORTRULEHDR const CDllString& CPcreOption::GetPcreString() const
{
	return m_strPcre;
}
