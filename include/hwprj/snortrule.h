/*!
* @file			snortrule.h
* @author		Lab 435, Xidian University
* @brief		����CSnortRule��
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
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
* @brief	��ȡSnort����ѡ�����ݡ�
* @remark	��װstd::vector<class CRuleOption*>�������ڼ�¼����ѡ�����ݣ�
*			����ܹ�����Ĺ���͹����Ӧ��sid
*/
class SNORTRULEHDR CSnortRule
{
public:
	typedef ulong PARSE_INFO;

	///@brief		һ�����
	static const PARSE_INFO NORMAL		= 0;

	/// @brief		�ù������byte_test��byte_jump��
	static const PARSE_INFO HASBYTE		= (1 << 0);

	/// @brief		 �ù������'!'���߼��ǣ���ʶ��
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

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õĹ���ѡ������á�
	/// @param[in]	nIdx ָ��λ�õ��±�
	/// @return		����ָ��λ�õĹ���ѡ������ã���������
	CRuleOption* operator[](ulong nIdx) const;

	/// @brief		�õ�CSnortRule��ǰ�Ĵ�С��
	/// @return		CSnortRule��ǰ�Ĵ�С����������
	ulong Size() const;

	/// @brief		���CSnortRule��
	void Clear();

	/// @brief		��ָ����CRuleOption�������ΪCSnortRule���һ��Ԫ�ء�
	/// @param[in]	pRuleOption ָ����CRuleOption����
	void PushBack(CRuleOption* pRuleOption);

	/// @brief		ɾ��CSnortRule�����һ��Ԫ�ء�
	void PopBack();

	/// @brief		�õ�CSnortRule�������һ��pRuleOption��������á�
	/// @return		���һ��pRuleOption��������á�
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
