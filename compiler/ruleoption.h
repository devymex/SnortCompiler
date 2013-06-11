#pragma once

#include <hwprj\common.h>

class CRuleOption
{
public:
	CRuleOption();
	CRuleOption(const CRuleOption &other);
	const CRuleOption& operator = (const CRuleOption &other);
	virtual ~CRuleOption();

	void SetPattern(LPCSTR lpStr);
	ulong GetPattern(LPSTR lpStr, ulong nLen) const;

	ulong GetFlag() const;
	void SetFlag(ulong nFlag);
	void AddFlag(ulong nFlag);
	BOOL TestFlag(ulong nFlag) const;

protected:
	ulong m_nFlag;
	std::string *m_pPattern;
};

struct OPTIONCONTENT : public CRuleOption
{
	BYTEARY vecconts;//content data
	int nOffset;// offset constraint in snort rule
	int nDepth;//depth constraint in snort rule
	int nDistance;//distance constraint in snort rule
	int nWithin;//within constraint in snort rule
};

struct OPTIONPCRE : public CRuleOption
{
};
