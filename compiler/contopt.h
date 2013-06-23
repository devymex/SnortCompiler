/*!
* @file		contopt.h
**
* @author		Lab 435, Xidian University
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
	static const OPTIONFLAG NOCASE		= (1 << 2);
	static const OPTIONFLAG OFFSET		= (1 << 3);
	static const OPTIONFLAG DEPTH		= (1 << 4);
	static const OPTIONFLAG DISTANCE	= (1 << 5);
	static const OPTIONFLAG WITHIN		= (1 << 6);

public:
	CContentOption();
	CContentOption(const CContentOption &other);
	virtual ~CContentOption();

	CContentOption&			operator = (const CContentOption &other);

	/// @brief		��content��������ѡ��ת��ΪPCRE��
	/// @param	pcreOpt-PCRE������
	void					ToPcre(CPcreOption &pcreOpt) const;

	/// @brief		����Snort�����е�contentѡ�������
	/// @param	strPat-Snort������content���������ַ���
	virtual void			FromPattern(const CDllString &strPat);

	/// @brief		���Ƶ�ǰcontentѡ��������
	virtual CRuleOption*	Clone() const;

public:
	int						m_nOffset;
	int						m_nDepth;
	int						m_nDistance;
	int						m_nWithin;

protected:
	CByteArray				m_data;
};
