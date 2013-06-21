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
#include <hwprj\pcreopt.h>
#include <hwprj\byteary.h>

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


	/*	函数名：ToPcre
	**	参数：pcreOpt-PCRE链类型
	**	功能：将content及其修饰选项转换为PCRE链
	*/
	void					ToPcre(CPcreOption &pcreOpt) const;

	/*	函数名：FromPattern
	**	参数：strPat-Snort规则中content及其属性字符串
	**	功能：解析Snort规则中的content选项及其属性
	*/
	virtual void			FromPattern(const CDllString &strPat);

	/*	函数名：Clone
	**	功能：复制当前CContentOption内容
	*/
	virtual CRuleOption*	Clone() const;

public:
	int						m_nOffset;
	int						m_nDepth;
	int						m_nDistance;
	int						m_nWithin;

protected:
	CByteArray				m_data;
};
