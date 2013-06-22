/*
**	@file		ruleoption.h
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

	CRuleOption&	operator = (const CRuleOption &other);

	/*	��������GetFlags
	**	���ܣ���ȡSnort����ѡ����
	*/
	OPTIONFLAG		GetFlags() const;

	/*	��������SetFlags
	**	������nFlags-ѡ����
	**	���ܣ�����Snort����ѡ���ǣ�Snort������ѡ���������������ԣ�
	**	��nFlags�Ĳ�ͬȡֵ��Ǹ�ѡ�������Щ����
	*/
	void			SetFlags(OPTIONFLAG nFlags);

	/*	��������AddFlags
	**	������nFlags-ѡ����
	**	���ܣ���Ӹ�ѡ�������ֵ
	*/
	void			AddFlags(OPTIONFLAG nFlags);

	/*	��������DelFlags
	**	������nFlags-ѡ����
	**	���ܣ�ɾ��ѡ��ı��
	*/
	void			DelFlags(OPTIONFLAG nFlags);

	/*	��������HasFlags
	**	������nFlags-ѡ����
	**	���ܣ���������Ƿ���ù��򡣹����а������ѡ���������
	**	һ��ѡ����ڴ���Χ����ù����޳�
	*/
	bool			HasFlags(OPTIONFLAG nFlags) const;

	/*	��������FromPattern
	**	������out-Snort�����ѡ���ַ���
	**	���ܣ�����Snort�����ѡ���ַ�����������ѡ�����ݴ���
	**	����ѡ���а������������򲻴���ù���
	*/
	virtual void	FromPattern(const CDllString &out) = 0;

	/*	��������FromPattern
	**	���ܣ����Ƶ�ǰѡ������
	*/
	virtual	CRuleOption*	Clone() const = 0;

protected:
	virtual void	FormatPattern(CDllString &out);

protected:
	OPTIONFLAG		m_nFlags;
};
