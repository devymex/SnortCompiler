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
	/// @brief		���캯����
	CRuleOption();

	/// @brief		�������캯����
	CRuleOption(const CRuleOption &other);

	/// @brief		����������
	virtual ~CRuleOption();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	CRuleOption& operator = (const CRuleOption &other);

	/// @brief		��ȡSnort����ѡ���ǡ�
	OPTIONFLAG GetFlags() const;

	/// @brief		����Snort����ѡ���ǡ�
	/// @param[in]	nFlags ѡ��ǡ�
	/// @remark		Snort������ѡ���������������ԣ���nFlags�Ĳ�ͬȡֵ
	///				��Ǹ�ѡ�������Щ���ԡ�
	void SetFlags(OPTIONFLAG nFlags);

	/// @brief		��Ӹ�ѡ�������ֵ��
	/// @param[in]	nFlags ѡ���ǡ�
	void AddFlags(OPTIONFLAG nFlags);

	/// @brief		ɾ��ѡ��ı�ǡ�
	/// @param[in]	nFlags ѡ���ǡ�
	void DelFlags(OPTIONFLAG nFlags);

	/// @brief		��������Ƿ���ù���
	/// @param[in]	nFlags ѡ���ǡ�
	///	�����а������ѡ���������һ��ѡ����ڴ���Χ����ù����޳���
	bool HasFlags(OPTIONFLAG nFlags) const;

	/// @brief		����Snort�����ѡ���ַ�����
	/// @param[out]	out Snort�����ѡ���ַ�����
	/// ������ѡ�����ݴ������ѡ���а���'!'���򲻴���ù���
	virtual void FromPattern(const CDllString &out) = 0;

	/// @brief		���Ƶ�ǰѡ������
	virtual	CRuleOption* Clone() const = 0;

protected:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	virtual void FormatPattern(CDllString &out);

protected:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	OPTIONFLAG m_nFlags;
};
