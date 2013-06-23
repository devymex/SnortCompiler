/*!
* @file		pcreopt.h
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
#include <hwprj\strary.h>
#include <hwprj\byteary.h>
#include <hwprj\ruleoption.h>

#ifndef SNORTRULEHDR_DS
class BYTEARY;
#endif

class SNORTRULEHDR CPcreOption : public CRuleOption
{
public:
	static const OPTIONFLAG PF_i = (1 << 2);
	static const OPTIONFLAG PF_s = (1 << 3);
	static const OPTIONFLAG PF_m = (1 << 4);
	static const OPTIONFLAG PF_x = (1 << 5);
	static const OPTIONFLAG PF_A = (1 << 6);
	static const OPTIONFLAG PF_E = (1 << 7);
	static const OPTIONFLAG PF_G = (1 << 8);
	static const OPTIONFLAG PF_R = (1 << 9);
	static const OPTIONFLAG PF_U = (1 << 10);
	static const OPTIONFLAG PF_B = (1 << 11);
	static const OPTIONFLAG PF_P = (1 << 12);
	static const OPTIONFLAG PF_H = (1 << 13);
	static const OPTIONFLAG PF_M = (1 << 14);
	static const OPTIONFLAG PF_C = (1 << 15);
	static const OPTIONFLAG PF_O = (1 << 16);
	static const OPTIONFLAG PF_I = (1 << 17);
	static const OPTIONFLAG PF_D = (1 << 18);
	static const OPTIONFLAG PF_K = (1 << 19);
	static const OPTIONFLAG PF_S = (1 << 20);
	static const OPTIONFLAG PF_Y = (1 << 21);
	static const OPTIONFLAG PF_F = (1 << 22); 

public:
	CPcreOption();
	CPcreOption(const CPcreOption &other);
	virtual ~CPcreOption();

	CPcreOption&			operator = (const CPcreOption &other);
	void					Append(const CPcreOption &next);

	/// @brief		处理Snort规则中pcre字符串内容及其修饰项，存入当前处理后的pcre字符串
	/// @param	strPat-Snort规则中pcre字符串
	virtual void			FromPattern(const CDllString &strPat);

	/// @brief		拷贝当前pcre字符串
	virtual CRuleOption*	Clone() const;

	/// @brief		设置处理后的pcre字符串
	/// @param	strPcre-设置pcre
	void					SetPcreString(const CDllString& strPcre);

	/// @brief		获取处理后的pcre字符串
	CDllString&				GetPcreString();
	const CDllString&		GetPcreString() const;

	/// @param	pcResult-PCRE链
	void					Precompile(CByteArray &pcResult) const;

protected:

	CDllString				m_strPcre;
};
