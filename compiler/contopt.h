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

/*!
* @addtogroup groupAutomaton
* @{
*/

/*!
* @brief		����һ��contentѡ������ݣ�����������������ѡ���ֵ��
* @remark		�ṩת����CPcreOption�Ĺ��ܡ�
*/
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

	/// @brief		��content��������ѡ��ת��ΪPCRE����
	/// @param[out]	pcreOpt һ��CPcreOption��ѡ�
	void ToPcre(CPcreOption &pcreOpt) const;

	/// @brief		����Snort�����е�contentѡ������ԡ�
	/// @param[in]	strPat Snort������content���������ַ�����
	virtual void FromPattern(const CDllString &strPat);

	/// @brief		���Ƶ�ǰcontentѡ�������ݡ�
	virtual CRuleOption* Clone() const;

public:
	/// @brief		
	int m_nOffset;

	/// @brief		
	int m_nDepth;

	/// @brief		
	int m_nDistance;

	/// @brief		
	int m_nWithin;

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	CByteArray m_data;
};

/*!
@}
*/
