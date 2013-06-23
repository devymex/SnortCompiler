/*!
* @file		snortrule.h
**
* @author		Lab 435, Xidian University
**
* @brief			Declaration of the CSnortRule class
**
* Parse snort rules
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

	static const PARSE_INFO NORMAL		= 0;
	static const PARSE_INFO HASBYTE		= (1 << 0);
	static const PARSE_INFO HASNOT		= (1 << 1);

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

	/// @brief		获取规则sid
	ulong			GetSid() const;

	/// @brief		设置规则sid
	/// @param	sid unsigned long类型规则编号
	void			SetSid(ulong sid);

	/// @brief		获取规则标记
	PARSE_INFO		GetFlags() const;

	/// @brief		增加规则标记
	/// @param	nFlags-规则标记
	void			AddFlags(PARSE_INFO nFlags);

	/// @brief		设置规则标记，用于标记该规则是否处理，规则选项内容中包含取非运算的
	/// @param	nFlags-规则标记
	void			SetFlags(PARSE_INFO nFlags);

	/// @brief		判断该规则是否包含标记
	/// @param	nFlags-规则标记
	bool			HasFlags(PARSE_INFO nFlags) const;

protected:
	ulong			m_nSid;
	PARSE_INFO		m_nFlags;
	OPTIONVEC*		m_pOptions;
};
