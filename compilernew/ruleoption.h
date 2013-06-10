#pragma once

class CRuleOption
{
public:
	CRuleOption();
	CRuleOption(const CRuleOption &other);
	const CRuleOption& operator = (const CRuleOption &other);
	virtual ~CRuleOption();

	void SetPattern(LPCSTR lpStr);
	ULONG GetPattern(LPSTR lpStr, ULONG nLen) const;

	ULONG GetFlag() const;
	void SetFlag(ULONG nFlag);
	void AddFlag(ULONG nFlag);
	BOOL TestFlag(ULONG nFlag) const;

protected:
	ULONG m_nFlag;
	std::string *m_pPattern;
};

struct OPTIONCONTENT : public CRuleOption
{
	std::vector<BYTE> vecconts;//content data
	int nOffset;// offset constraint in snort rule
	int nDepth;//depth constraint in snort rule
	int nDistance;//distance constraint in snort rule
	int nWithin;//within constraint in snort rule
};

struct OPTIONPCRE : public CRuleOption
{
};
