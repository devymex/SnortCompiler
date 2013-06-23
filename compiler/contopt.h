/*!
* @file		contopt.h
**
* @author		Lab 435, Xidian University
**
* @brief			Declaration of the CContentOption class
**
* inherit CRuleOption class
* process “content” attribute in a Snort rule
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

	/// @brief		将content及其修饰选项转换为PCRE链
	/// @param	pcreOpt-PCRE链类型
	void					ToPcre(CPcreOption &pcreOpt) const;

	/// @brief		解析Snort规则中的content选项及其属性
	/// @param	strPat-Snort规则中content及其属性字符串
	virtual void			FromPattern(const CDllString &strPat);

	/// @brief		复制当前content选项中内容
	virtual CRuleOption*	Clone() const;

public:
	int						m_nOffset;
	int						m_nDepth;
	int						m_nDistance;
	int						m_nWithin;

protected:
	CByteArray				m_data;
};
