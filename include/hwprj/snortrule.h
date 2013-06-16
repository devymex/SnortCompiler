/*
**	@file		snortrule.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CSnortRule class
**
**	Parse snort rules
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\ruleoption.h>

#ifndef SNORTRULEHDR_DS
class OPTIONVEC;
#endif

class SNORTRULEHDR CSnortRule
{
public:
	typedef ulong PARSE_INFO;
	static const PARSE_INFO RULE_HASBYTE	= 0x0001;
	static const PARSE_INFO RULE_HASNOT		= 0x0002;

	CSnortRule();
	CSnortRule(const CSnortRule &other);
	virtual ~CSnortRule();

	CSnortRule&		operator = (const CSnortRule &other);
	CRuleOption*	operator[](ulong nIdx) const;

	ulong			Size() const;
	void			Clear();
	void			PushBack(CRuleOption* pRuleOption);
	void			PopBack();
	CRuleOption*	Back();

	ulong			GetSid() const;
	void			SetSid(ulong sid);
	PARSE_INFO		GetFlag() const;
	void			SetFlag(PARSE_INFO flag);

protected:
	ulong m_nSid;
	PARSE_INFO m_nFlag;
	OPTIONVEC *m_pOptions;
};
