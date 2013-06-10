
#pragma once

#ifndef SNORTRULEHDR_DS
#define SNORTRULEHDR __declspec(dllimport)
#else
#define SNORTRULEHDR __declspec(dllexport)
#endif

class CRuleOption;

class SNORTRULEHDR CSnortRule
{
public:
	enum {RULE_HASBYTE = 0x0001, RULE_HASNOT = 0x0002, RULE_HASNOSIG = 0x0004};

	CSnortRule();
	CSnortRule(const CSnortRule &other);
	const CSnortRule& operator = (const CSnortRule &other);
	~CSnortRule();

	CRuleOption* operator[](ULONG nIdx) const;

	ULONG Size() const;
	ULONG GetSid() const;
	void SetSid(ULONG sid);
	ULONG GetFlag() const;
	void SetFlag(ULONG flag);
	void PushBack(CRuleOption* ruleoption);
	void PopBack();

protected:
	ULONG m_nSid;
	ULONG m_nFlag;
	std::vector<CRuleOption*> *m_pOptions;
};
