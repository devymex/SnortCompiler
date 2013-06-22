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

	/*	函数名：GetSid
	**	功能：获取规则sid
	*/
	ulong			GetSid() const;

	/*	函数名：SetSid
	**	参数：sid-unsigned long类型规则编号
	**	功能：设置规则sid
	*/
	void			SetSid(ulong sid);

	/*	函数名：GetFlags
	**	功能：获取规则标记
	*/
	PARSE_INFO		GetFlags() const;

	/*	函数名：AddFlags
	**	参数：nFlags-规则标记
	**	功能：增加规则标记
	*/
	void			AddFlags(PARSE_INFO nFlags);

	/*	函数名：SetFlags
	**	参数：nFlags-规则标记
	**	功能：设置规则标记，用于标记该规则是否处理，规则选项内容中包含取非运算的
	**	或者规则选项名为“bytetest”和“bytejump”的规则不做处理
	*/
	void			SetFlags(PARSE_INFO nFlags);

	/*	函数名：HasFlags
	**	参数：nFlags-规则标记
	**	功能：判断该规则是否包含标记
	*/
	bool			HasFlags(PARSE_INFO nFlags) const;

protected:
	ulong			m_nSid;
	PARSE_INFO		m_nFlags;
	OPTIONVEC*		m_pOptions;
};
