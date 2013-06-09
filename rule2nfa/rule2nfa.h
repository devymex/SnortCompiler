#pragma once
#include "../common/common.h"
#include "../pcre2nfa/pcre2nfa.h"
#include <Windows.h>

#ifndef CRECHANFA_H_
#define CRECHANFA __declspec(dllimport)
#else
#define CRECHANFA __declspec(dllexport)
#endif


class CRECHANFA CRegChain
{
public:
	CRegChain();
	~CRegChain();
	CRegChain(const CRegChain &other);
	const CRegChain& operator = (const CRegChain &other);

	CCString& operator[](size_t nIdx);

	size_t Size() const;
	CCString& Back() const;
	void PushBack(const CCString &pcreStr);
	void Resize(size_t nSize);

	CSignatures& GetSigs();
	const CSignatures& GetSigs() const;


protected:
	std::vector<CCString> *m_pRegList;
	CSignatures m_sigs;
};

class CRECHANFA CRegRule
{
public:
	CRegRule();
	~CRegRule();
	CRegRule(const CRegRule &other);
	const CRegRule& operator = (const CRegRule &other);

	CRegChain& operator[](size_t nIdx);
	const CRegChain& operator[](size_t nIdx) const;

	size_t Size() const;
	void Reserve(size_t nCount);
	void Resize(size_t nSize);
	CRegChain& Back() const;
	void PushBack(const CRegChain &nRegChain);

protected:
	std::vector<CRegChain> *m_pRegVec;
};

class CRuleOption;

class CRECHANFA CSnortRule
{
public:
	enum {RULE_HASBYTE = 0x0001, RULE_HASNOT = 0x0002, RULE_HASNOSIG = 0x0004};

	CSnortRule();
	CSnortRule(const CSnortRule &other);
	const CSnortRule& operator = (const CSnortRule &other);
	~CSnortRule();

	CRuleOption* operator[](size_t nIdx) const;

	size_t Size() const;
	size_t GetSid() const;
	void SetSid(size_t sid);
	size_t GetFlag() const;
	void SetFlag(size_t flag);
	void PushBack(CRuleOption* ruleoption);
	void PopBack();

protected:
	size_t m_nSid;
	size_t m_nFlag;
	std::vector<CRuleOption*> *m_pOptions;
};

typedef void (CALLBACK *RECIEVER)(const CSnortRule &rule, LPVOID lpParam);

CRECHANFA size_t CompileRuleSet(LPCTSTR fileName, RECIEVER recv, LPVOID lpUser);

CRECHANFA void CompileRule(LPCSTR rule, RECIEVER recv, LPVOID lpUser);

CRECHANFA size_t Rule2PcreList(const CSnortRule &rule, CRegRule &regrule);

CRECHANFA size_t CRegChainToNFA(CRegChain &regchain, CNfa &nfa);

