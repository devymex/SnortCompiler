/*
**	@file		contopt.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CContentOption class
**
**	inherit CRuleOption class
**	process “content” attribute in a Snort rule
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\ruleoption.h>
#include <hwprj\signatures.h>
#include <hwprj\pcreopt.h>

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

	/*	函数名：ExtractSignatures
	**	参数：sigs-特征字符串类型
	**	功能：从
	*/
	void					ExtractSignatures(CSignatures &sigs) const;
	void					ToPcre(CPcreOption &pcreOpt) const;

	virtual void			FromPattern(const CDllString &strPat);
	virtual CRuleOption*	Clone() const;

public:
	int						m_nOffset;
	int						m_nDepth;
	int						m_nDistance;
	int						m_nWithin;

protected:
	BYTEARY					m_data;
};
