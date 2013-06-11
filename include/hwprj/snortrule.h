
#pragma once

#include <hwprj\common.h>

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

	CRuleOption* operator[](ulong nIdx) const;

	ulong Size() const;
	ulong GetSid() const;
	void SetSid(ulong sid);
	ulong GetFlag() const;
	void SetFlag(ulong flag);
	void PushBack(CRuleOption* ruleoption);
	void PopBack();

protected:
	ulong m_nSid;
	ulong m_nFlag;
	std::vector<CRuleOption*> *m_pOptions;
};
