/*
**	@file		compres.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Definition of the CCompileResults class
**
**	For compiler's result, access member, read and write etc. 
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
	/*	��������GetDfaTable
	**	���ܣ���ȡDFA��ת����
	*/
	CDfaArray& GetDfaTable();

	/*	��������GetSidDfaIds
	**	���ܣ���ȡ����sid��DfaId�Ķ�Ӧ��ϵ
	*/
	CSidDfaIds& GetSidDfaIds();

	/*	��������GetRegexTbl
	**	���ܣ���ȡ����sid��DfaId�Ķ�Ӧ��ϵ
	*/
	CRegRule &GetRegexTbl();

	const CDfaArray& GetDfaTable() const;
	const CSidDfaIds& GetSidDfaIds() const;
	const CRegRule &GetRegexTbl() const;

	/*	��������WriteToFile
	**	������filename-��������ļ�·��
	**	���ܣ���DFA�������ļ�
	*/
	ulong WriteToFile(const char *filename);

	/*	��������ReadFromFile
	**	������filename-��������ļ�·��
	**	���ܣ�����DFA����ļ�
	*/
	ulong ReadFromFile(const char *filename);

protected:
	CDfaArray m_dfaTbl;
	CSidDfaIds m_sidDfaIds;
	CRegRule m_RegexTbl;
};
