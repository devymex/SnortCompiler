/*!
* @file		ruleoption.h
**
* @author		Lab 435, Xidian University
**
* @brief			Declaration of the CRuleOption class
**
* Extract relevant attribute of a rule, 
* such as "content", "pcre" and "uricontent".
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

	CRuleOption& operator = (const CRuleOption &other);

	/// @brief		获取Snort规则选项标记。
	OPTIONFLAG GetFlags() const;

	/// @brief		设置Snort规则选项标记。
	/// @param	nFlags-选标记。
	/// Snort规则中选项具有相关修饰属性，用nFlags的不同取值标记该选项具有哪些属性。
	void SetFlags(OPTIONFLAG nFlags);

	/// @brief		添加该选项的属性值。
	/// @param	nFlags-选项标记。
	void AddFlags(OPTIONFLAG nFlags);

	/// @brief		删除选项的标记。
	/// @param	nFlags-选项标记。
	void DelFlags(OPTIONFLAG nFlags);

	/// @brief		用来标记是否处理该规则。
	/// @param	nFlags-选项标记。
	///	规则中包含多个选项，若其中有一个选项不属于处理范围，则该规则被剔除。
	bool HasFlags(OPTIONFLAG nFlags) const;

	/// @brief		解析Snort规则的选项字符串。
	/// @param	out Snort规则的选项字符串。
	/// 若规则选项内容错误或者选项中包含'!'，则不处理该规则。
	virtual void FromPattern(const CDllString &out) = 0;

	/// @brief		复制当前选项内容
	virtual	CRuleOption* Clone() const = 0;

protected:
	virtual void FormatPattern(CDllString &out);

protected:
	OPTIONFLAG m_nFlags;
};
