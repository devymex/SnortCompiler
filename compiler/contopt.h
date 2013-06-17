/*
**	@file		contopt.h
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
#include <hwprj\ruleoption.h>
#include <hwprj\signatures.h>

class CContentOption : public CRuleOption
{
public:
	static const OPTIONFLAG NOCASE		= (1 << 2);
	static const OPTIONFLAG OFFSET		= (1 << 3);
	static const OPTIONFLAG DEPTH		= (1 << 4);
	static const OPTIONFLAG DISTANCE	= (1 << 5);
	static const OPTIONFLAG WITHIN		= (1 << 6);

public:
	CContentOption();
	CContentOption(const CContentOption &other);
	virtual ~CContentOption();

	CContentOption&			operator = (const CContentOption &other);

	void					ExtractSignatures(CSignatures &sigs) const;
	void					ToPcre(CDllString &pcre) const;

	virtual void			FromPattern(pcstr &pBeg, pcstr &pEnd);
	virtual CRuleOption*	Clone() const;

public:
	int						m_nOffset;
	int						m_nDepth;
	int						m_nDistance;
	int						m_nWithin;

protected:
	BYTEARY					m_data;
};
