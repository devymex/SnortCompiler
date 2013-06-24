/*!
* @file			pcreopt.h
* @author		Lab 435, Xidian University
* @brief		Declaration of the CRuleOption class
* @remark		Extract relevant attribute of a rule, 
*				such as "content", "pcre" and "uricontent".
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\strary.h>
#include <hwprj\byteary.h>
#include <hwprj\ruleoption.h>

#ifndef SNORTRULEHDR_DS
class BYTEARY;
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief		����һ��PCRE��������ݣ�����������ʽ��估����ѡ�
* @remark		��CRuleOption��̳У��ṩ��pcreѡ���ַ����ĵ��룬�Լ�Ԥ����ȹ��ܡ�
*/
class SNORTRULEHDR CPcreOption : public CRuleOption
{
public:
	static const OPTIONFLAG PF_i = (1 << 2);	/// PCRE��׺'i'
	static const OPTIONFLAG PF_s = (1 << 3);	/// PCRE��׺'s'
	static const OPTIONFLAG PF_m = (1 << 4);	/// PCRE��׺'m'
	static const OPTIONFLAG PF_x = (1 << 5);	/// PCRE��׺'x'
	static const OPTIONFLAG PF_A = (1 << 6);	/// PCRE��׺'A'
	static const OPTIONFLAG PF_E = (1 << 7);	/// PCRE��׺'E'
	static const OPTIONFLAG PF_G = (1 << 8);	/// PCRE��׺'G'
	static const OPTIONFLAG PF_R = (1 << 9);	/// PCRE��׺'R'
	static const OPTIONFLAG PF_U = (1 << 10);	/// PCRE��׺'U'
	static const OPTIONFLAG PF_B = (1 << 11);	/// PCRE��׺'B'
	static const OPTIONFLAG PF_P = (1 << 12);	/// PCRE��׺'P'
	static const OPTIONFLAG PF_H = (1 << 13);	/// PCRE��׺'H'
	static const OPTIONFLAG PF_M = (1 << 14);	/// PCRE��׺'M'
	static const OPTIONFLAG PF_C = (1 << 15);	/// PCRE��׺'C'
	static const OPTIONFLAG PF_O = (1 << 16);	/// PCRE��׺'O'
	static const OPTIONFLAG PF_I = (1 << 17);	/// PCRE��׺'I'
	static const OPTIONFLAG PF_D = (1 << 18);	/// PCRE��׺'D'
	static const OPTIONFLAG PF_K = (1 << 19);	/// PCRE��׺'K'
	static const OPTIONFLAG PF_S = (1 << 20);	/// PCRE��׺'S'
	static const OPTIONFLAG PF_Y = (1 << 21);	/// PCRE��׺'Y'
	static const OPTIONFLAG PF_F = (1 << 22); 	/// PCRE��׺'F'

public:
	/// @brief		���캯����
	CPcreOption();

	/// @brief		�������캯����
	CPcreOption(const CPcreOption &other);

	/// @brief		����������
	virtual ~CPcreOption();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	CPcreOption& operator = (const CPcreOption &other);

	/// @brief		����Snort������pcre�ַ������ݼ�����������뵱ǰ������pcre�ַ���
	/// @param		strPat Snort������pcre�ַ���
	virtual void FromPattern(const CDllString &strPat);

	/// @brief		������ǰpcre�ַ���
	virtual CRuleOption* Clone() const;

	/// @brief		���ô�����pcre�ַ���
	/// @param		strPcre pcre�ַ���������Ϊ������������ʽ����
	///				���ܴ���ǰ���'/'�Լ���׺�ȡ�
	void SetPcreString(const CDllString& strPcre);

	/// @brief		��ȡ������pcre�ַ���
	CDllString& GetPcreString();
	const CDllString& GetPcreString() const;

	/// @brief		Ԥ����pcre������Ԥ���������ݡ�
	/// @param		pcResult Ԥ����������
	void Precompile(CByteArray &pcResult) const;

protected:
	///	@brief		˽�г�Ա�������ڲ�ʹ�á�
	CDllString m_strPcre;
};

/*!
@}
*/
