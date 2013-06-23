/*!
* @file		compres.h
**
* @author		Lab 435, Xidian University
**
* @brief			Definition of the CCompileResults class
**
* For compiler's result, access member, read and write etc. 
**
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

class COMPRESHDR CCompileResults
{
public:
	/// @brief		��ȡDFA��ת����
	CDfaArray& GetDfaTable();

	/// @brief		��ȡ����sid��DfaId�Ķ�Ӧ��ϵ
	CSidDfaIds& GetSidDfaIds();

	/// @brief		��ȡ����sid��DfaId�Ķ�Ӧ��ϵ
	CRegRule &GetRegexTbl();

	const CDfaArray& GetDfaTable() const;
	const CSidDfaIds& GetSidDfaIds() const;
	const CRegRule &GetRegexTbl() const;

	/// @brief		��DFA�������ļ�
	/// @param	filename-��������ļ�·��
	ulong WriteToFile(const char *filename);

	/// @brief		����DFA����ļ�
	/// @param	filename-��������ļ�·��
	ulong ReadFromFile(const char *filename);

protected:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	CDfaArray m_dfaTbl;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	CSidDfaIds m_sidDfaIds;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	CRegRule m_RegexTbl;
};
