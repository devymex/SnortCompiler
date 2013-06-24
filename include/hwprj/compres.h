/*!
* @file				compres.h
* @author			Lab 435, Xidian University
* @brief			定义CCompileResults类
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
* @brief		保存DFA及PCRE链等信息。
* @remark		包括所有的DFA、规则处理信息、PCRE链。
*/
class COMPRESHDR CCompileResults
{
public:
	/// @brief		获取DFA跳转矩阵
	CDfaArray& GetDfaTable();

	/// @brief		获取规则sid与DFA编号的对应关系
	CSidDfaIds& GetSidDfaIds();

	/// @brief		获取解析得到的规格化规则集。
	CRegRule &GetRegexTbl();

	const CDfaArray& GetDfaTable() const;
	const CSidDfaIds& GetSidDfaIds() const;
	const CRegRule &GetRegexTbl() const;

	/// @brief		将DFA结果输出文件
	/// @param[in]	filename 输入包含文件路径的文件名
	ulong WriteToFile(const char *filename);

	/// @brief		读入DFA结果文件
	/// @param[in]	filename 输入包含文件路径的文件名
	ulong ReadFromFile(const char *filename);

private:
	/// @brief		私有成员，仅供内部使用
	CDfaArray m_dfaTbl;

	/// @brief		私有成员，仅供内部使用
	CSidDfaIds m_sidDfaIds;

	/// @brief		私有成员，仅供内部使用
	CRegRule m_RegexTbl;
};

/*!
@}
*/
