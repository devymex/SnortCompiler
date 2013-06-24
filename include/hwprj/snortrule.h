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
*  @{
*/

/*!
*/
class SNORTRULEHDR CSnortRule
{
public:
	typedef ulong PARSE_INFO;

	static const PARSE_INFO NORMAL		= 0;
	static const PARSE_INFO HASBYTE		= (1 << 0);
	static const PARSE_INFO HASNOT		= (1 << 1);

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

	CRuleOption* operator[](ulong nIdx) const;

	ulong Size() const;
	void Clear();
	void PushBack(CRuleOption* pRuleOption);
	void PopBack();
	CRuleOption* Back();

	/// @brief		��ȡ����sid��
	ulong GetSid() const;

	/// @brief		���ù���sid��
	/// @param		sid �����š�
	void SetSid(ulong sid);

	/// @brief		��ȡ�����ǡ�
	PARSE_INFO GetFlags() const;

	/// @brief		���ӹ����ǡ�
	/// @param		nFlags �����ǡ�
	void AddFlags(PARSE_INFO nFlags);

	/// @brief		���ù����ǣ����ڱ�Ǹù����Ƿ�������ѡ�������а���ȡ������ġ�
	/// @param		nFlags �����ǡ�
	void SetFlags(PARSE_INFO nFlags);

	/// @brief		�жϸù����Ƿ������ǡ�
	/// @param		nFlags �����ǡ�
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
