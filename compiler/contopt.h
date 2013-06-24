/*!
* @file				contopt.h
**
* @author			Lab 435, Xidian University
**
* @brief			Declaration of the CContentOption class
**
* inherit CRuleOption class
* process ��content�� attribute in a Snort rule
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\ruleoption.h>
#include <hwprj\pcreopt.h>
#include <hwprj\byteary.h>

class CContentOption : public CRuleOption
{
public:
	static const OPTIONFLAG NOCASE		= (1 << 2);		/// ���Դ�Сд
	static const OPTIONFLAG OFFSET		= (1 << 3);		/// ����offset������
	static const OPTIONFLAG DEPTH		= (1 << 4);		/// ����depth������
	static const OPTIONFLAG DISTANCE	= (1 << 5);		/// ����distance������
	static const OPTIONFLAG WITHIN		= (1 << 6);		/// ����within������

public:
	/// @brief		���캯����
	CContentOption();

	/// @brief		�������캯����
	CContentOption(const CContentOption &other);

	/// @brief		����������
	virtual ~CContentOption();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	CContentOption& operator = (const CContentOption &other);

	/// @brief		��content��������ѡ��ת��ΪPCRE��
	/// @param		pcreOpt PCRE������
	void ToPcre(CPcreOption &pcreOpt) const;

	/// @brief		����Snort�����е�contentѡ�������
	/// @param		strPat Snort������content���������ַ���
	virtual void FromPattern(const CDllString &strPat);

	/// @brief		���Ƶ�ǰcontentѡ��������
	virtual CRuleOption* Clone() const;

public:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	int m_nOffset;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	int m_nDepth;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	int m_nDistance;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	int m_nWithin;

protected:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	CByteArray m_data;
};
