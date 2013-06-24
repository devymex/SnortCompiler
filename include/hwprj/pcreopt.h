/*!
* @file			pcreopt.h
* @author		Lab 435, Xidian University
* @brief		Declaration of the CRuleOption class
* @remark		Extract relevant attribute of a rule, 
*				such as "content", "pcre" and "uricontent".
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\strary.h>
#include <hwprj\byteary.h>
#include <hwprj\ruleoption.h>

#ifndef SNORTRULEHDR_DS
class BYTEARY;
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief		管理一个PCRE对象的数据，包括正则表达式语句及修饰选项。
* @remark		从CRuleOption类继承，提供从pcre选项字符串的导入，以及预编译等功能。
*/
class SNORTRULEHDR CPcreOption : public CRuleOption
{
public:
	static const OPTIONFLAG PF_i = (1 << 2);	/// PCRE后缀'i'
	static const OPTIONFLAG PF_s = (1 << 3);	/// PCRE后缀's'
	static const OPTIONFLAG PF_m = (1 << 4);	/// PCRE后缀'm'
	static const OPTIONFLAG PF_x = (1 << 5);	/// PCRE后缀'x'
	static const OPTIONFLAG PF_A = (1 << 6);	/// PCRE后缀'A'
	static const OPTIONFLAG PF_E = (1 << 7);	/// PCRE后缀'E'
	static const OPTIONFLAG PF_G = (1 << 8);	/// PCRE后缀'G'
	static const OPTIONFLAG PF_R = (1 << 9);	/// PCRE后缀'R'
	static const OPTIONFLAG PF_U = (1 << 10);	/// PCRE后缀'U'
	static const OPTIONFLAG PF_B = (1 << 11);	/// PCRE后缀'B'
	static const OPTIONFLAG PF_P = (1 << 12);	/// PCRE后缀'P'
	static const OPTIONFLAG PF_H = (1 << 13);	/// PCRE后缀'H'
	static const OPTIONFLAG PF_M = (1 << 14);	/// PCRE后缀'M'
	static const OPTIONFLAG PF_C = (1 << 15);	/// PCRE后缀'C'
	static const OPTIONFLAG PF_O = (1 << 16);	/// PCRE后缀'O'
	static const OPTIONFLAG PF_I = (1 << 17);	/// PCRE后缀'I'
	static const OPTIONFLAG PF_D = (1 << 18);	/// PCRE后缀'D'
	static const OPTIONFLAG PF_K = (1 << 19);	/// PCRE后缀'K'
	static const OPTIONFLAG PF_S = (1 << 20);	/// PCRE后缀'S'
	static const OPTIONFLAG PF_Y = (1 << 21);	/// PCRE后缀'Y'
	static const OPTIONFLAG PF_F = (1 << 22); 	/// PCRE后缀'F'

public:
	/// @brief		构造函数。
	CPcreOption();

	/// @brief		拷贝构造函数。
	CPcreOption(const CPcreOption &other);

	/// @brief		析构函数。
	virtual ~CPcreOption();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		返回自身对象的引用。
	CPcreOption& operator = (const CPcreOption &other);

	/// @brief		处理Snort规则中pcre字符串内容及其修饰项，存入当前处理后的pcre字符串
	/// @param		strPat Snort规则中pcre字符串
	virtual void FromPattern(const CDllString &strPat);

	/// @brief		拷贝当前pcre字符串
	virtual CRuleOption* Clone() const;

	/// @brief		设置处理后的pcre字符串
	/// @param		strPcre pcre字符串，必须为纯净的正则表达式串，
	///				不能带有前后的'/'以及后缀等。
	void SetPcreString(const CDllString& strPcre);

	/// @brief		获取处理后的pcre字符串
	CDllString& GetPcreString();
	const CDllString& GetPcreString() const;

	/// @brief		预编译pcre，生成预编译结果数据。
	/// @param		pcResult 预编译结果数据
	void Precompile(CByteArray &pcResult) const;

protected:
	///	@brief		私有成员，仅供内部使用。
	CDllString m_strPcre;
};

/*!
@}
*/
