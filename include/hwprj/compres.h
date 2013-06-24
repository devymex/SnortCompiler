/*!
* @file				compres.h
* @author			Lab 435, Xidian University
* @brief			����CCompileResults��
*/

#pragma once

#include <hwprj\dfaarray.h>
#include <hwprj\siddfaids.h>
#include <hwprj\regrule.h>

#ifndef COMPRESHDR_DS
#define COMPRESHDR __declspec(dllimport)
#else
#define COMPRESHDR __declspec(dllexport)
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief		����DFA��PCRE������Ϣ��
* @remark		�������е�DFA����������Ϣ��PCRE����
*/
class COMPRESHDR CCompileResults
{
public:
	/// @brief		��ȡDFA��ת����
	CDfaArray& GetDfaTable();

	/// @brief		��ȡ����sid��DFA��ŵĶ�Ӧ��ϵ
	CSidDfaIds& GetSidDfaIds();

	/// @brief		��ȡ�����õ��Ĺ�񻯹��򼯡�
	CRegRule &GetRegexTbl();

	const CDfaArray& GetDfaTable() const;
	const CSidDfaIds& GetSidDfaIds() const;
	const CRegRule &GetRegexTbl() const;

	/// @brief		��DFA�������ļ�
	/// @param[in]	filename ��������ļ�·�����ļ���
	ulong WriteToFile(const char *filename);

	/// @brief		����DFA����ļ�
	/// @param[in]	filename ��������ļ�·�����ļ���
	ulong ReadFromFile(const char *filename);

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	CDfaArray m_dfaTbl;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	CSidDfaIds m_sidDfaIds;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	CRegRule m_RegexTbl;
};

/*!
@}
*/
