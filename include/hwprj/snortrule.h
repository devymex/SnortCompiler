/*!
* @file			snortrule.h
* @author		Lab 435, Xidian University
* @brief		����CSnortRule��
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\ruleoption.h>

#ifndef SNORTRULEHDR_DS
class OPTIONVEC;
#endif

/*! @addtogroup groupCompiler
* @{
*/

/*!
* @brief
*/
class SNORTRULEHDR CSnortRule
{
public:
	typedef ulong PARSE_INFO;

	/// һ�����
	static const PARSE_INFO NORMAL		= 0;
	/// �ù������byte_test��byte_jump��
	static const PARSE_INFO HASBYTE		= (1 << 0);
	/// �ù������'!'���߼��ǣ���ʶ��
	static const PARSE_INFO HASNOT		= (1 << 1);

public:
	/// @brief		���캯����
	CSnortRule();

	/// @brief		�������캯����
	CSnortRule(const CSnortRule &other);

	/// @brief		����������
	virtual ~CSnortRule();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	CSnortRule& operator = (const CSnortRule &other);

	/// @brief
	CRuleOption* operator[](ulong nIdx) const;

	/// @brief
	ulong Size() const;

	/// @brief		ɾ�����������еĹ���ѡ����󣬲��ͷ����ǵ��ڴ档
	void Clear();

	/// @brief		��һ��CRuleOption�����ָ����ӵ������ĩβ��
	/// @param		pRuleOption һ��CRuleOption�����ָ�룬��ָ���������ࡣ
	void PushBack(CRuleOption* pRuleOption);

	/// @brief		ɾ�����������һ��ѡ����ͷ����ڴ档
	void PopBack();

	/// @brief		��ȡ���������һ��ѡ���ָ�롣
	/// @return		һ��CRuleOption�����ָ�롣����ָ���������ࡣ
	CRuleOption* Back();

	/// @brief		��ȡ����sid��
	ulong GetSid() const;

	/// @brief		���ù���sid��
	/// @param[in]	sid �����š�
	void SetSid(ulong sid);

	/// @brief		��ȡ�����ǡ�
	PARSE_INFO GetFlags() const;

	/// @brief		���ӹ����ǡ�
	/// @param[in]	nFlags �����ǡ�
	void AddFlags(PARSE_INFO nFlags);

	/// @brief		���ù����ǣ����ڱ�Ǹù����Ƿ�������ѡ�������а���ȡ������ġ�
	/// @param[in]	nFlags �����ǡ�
	void SetFlags(PARSE_INFO nFlags);

	/// @brief		�жϸù����Ƿ������ǡ�
	/// @param[in]	nFlags �����ǡ�
	bool HasFlags(PARSE_INFO nFlags) const;

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	ulong m_nSid;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	PARSE_INFO m_nFlags;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	OPTIONVEC* m_pOptions;
};

/*!
@}
*/
