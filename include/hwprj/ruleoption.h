#pragma once

#include <hwprj\common.h>

#ifndef SNORTRULEHDR_DS
class STRING;
#define SNORTRULEHDR __declspec(dllimport)
#else
#define SNORTRULEHDR __declspec(dllexport)
#endif

class SNORTRULEHDR CRuleOption
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
	bool TestFlag(ulong nFlag) const;

protected:
	ulong m_nFlag;
	STRING *m_pPattern;
};
