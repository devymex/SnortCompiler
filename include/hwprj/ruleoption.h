/*
**	@file		ruleoption.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CRuleOption class
**
**	Extract relevant attribute of a rule, 
**	such as "content", "pcre" and "uricontent".
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\dllstring.h>

#ifndef SNORTRULEHDR_DS
class STRING;
#define SNORTRULEHDR __declspec(dllimport)
#else
#define SNORTRULEHDR __declspec(dllexport)
#endif

class SNORTRULEHDR CRuleOption
{
public:
	typedef ulong OPTIONFLAG;

	static const OPTIONFLAG NOFLAG = 0;
	static const OPTIONFLAG HASNOT = (1 << 0);

public:
	CRuleOption();
	CRuleOption(const CRuleOption &other);
	virtual ~CRuleOption();

	CRuleOption&	operator = (const CRuleOption &other);

	OPTIONFLAG		GetFlags() const;
	void			SetFlags(OPTIONFLAG nFlags);
	void			AddFlags(OPTIONFLAG nFlags);
	bool			HasFlags(OPTIONFLAG nFlags) const;

	void			GetPattern(CDllString &out) const;

	virtual void			FromPattern(pcstr &pBeg, pcstr &pEnd);
	virtual	CRuleOption*	Clone() const;

protected:
	OPTIONFLAG		m_nFlags;
	STRING*			m_pPat;
};
