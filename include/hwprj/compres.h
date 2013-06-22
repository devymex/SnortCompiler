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
	/*	函数名：GetDfaTable
	**	功能：获取DFA跳转矩阵
	*/
	CDfaArray& GetDfaTable();

	/*	函数名：GetSidDfaIds
	**	功能：获取规则sid与DfaId的对应关系
	*/
	CSidDfaIds& GetSidDfaIds();

	/*	函数名：GetRegexTbl
	**	功能：获取规则sid与DfaId的对应关系
	*/
	CRegRule &GetRegexTbl();

	const CDfaArray& GetDfaTable() const;
	const CSidDfaIds& GetSidDfaIds() const;
	const CRegRule &GetRegexTbl() const;

	/*	函数名：WriteToFile
	**	参数：filename-输入规则文件路径
	**	功能：将DFA结果输出文件
	*/
	ulong WriteToFile(const char *filename);

	/*	函数名：ReadFromFile
	**	参数：filename-输入规则文件路径
	**	功能：读入DFA结果文件
	*/
	ulong ReadFromFile(const char *filename);

protected:
	CDfaArray m_dfaTbl;
	CSidDfaIds m_sidDfaIds;
	CRegRule m_RegexTbl;
};
