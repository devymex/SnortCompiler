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

	/*	��������GetSid
	**	���ܣ���ȡ����sid
	*/
	ulong			GetSid() const;

	/*	��������SetSid
	**	������sid-unsigned long���͹�����
	**	���ܣ����ù���sid
	*/
	void			SetSid(ulong sid);

	/*	��������GetFlags
	**	���ܣ���ȡ������
	*/
	PARSE_INFO		GetFlags() const;

	/*	��������AddFlags
	**	������nFlags-������
	**	���ܣ����ӹ�����
	*/
	void			AddFlags(PARSE_INFO nFlags);

	/*	��������SetFlags
	**	������nFlags-������
	**	���ܣ����ù����ǣ����ڱ�Ǹù����Ƿ�������ѡ�������а���ȡ�������
	**	���߹���ѡ����Ϊ��bytetest���͡�bytejump���Ĺ���������
	*/
	void			SetFlags(PARSE_INFO nFlags);

	/*	��������HasFlags
	**	������nFlags-������
	**	���ܣ��жϸù����Ƿ�������
	*/
	bool			HasFlags(PARSE_INFO nFlags) const;

protected:
	ulong			m_nSid;
	PARSE_INFO		m_nFlags;
	OPTIONVEC*		m_pOptions;
};
