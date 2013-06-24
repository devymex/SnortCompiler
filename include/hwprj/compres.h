/*!
* @file				compres.h
* @author			Lab 435, Xidian University
* @brief			定义CCompileResults类
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
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
	/// @brief		获取所有编译好的CDfa对象集合。
	CDfaArray& GetDfaTable();

	/// @brief		获取所有编译好的CDfa对象集合（常量）。
	const CDfaArray& GetDfaTable() const;

	/// @brief		获得所有CDfa对象所对应的sid的集合。
	CSidDfaIds& GetSidDfaIds();

	/// @brief		获取规则sid与DFA编号的对应关系（常量）。
	const CSidDfaIds& GetSidDfaIds() const;

	/// @brief		获得所有解析得到的规格化规则集合。
	CRegRule &GetRegexTbl();

	/// @brief		获得所有解析得到的规格化规则集合（常量）。
	const CRegRule &GetRegexTbl() const;

	/// @brief		将DFA结果输出文件。
	/// @param[in]	filename 输入包含文件路径的文件名。
	ulong WriteToFile(const char *filename);

	/// @brief		读入DFA结果文件。
	/// @param[in]	filename 输入包含文件路径的文件名。
	ulong ReadFromFile(const char *filename);

private:
	/// @brief		私有成员，仅供内部使用。
	CDfaArray m_dfaTbl;

	/// @brief		私有成员，仅供内部使用。
	CSidDfaIds m_sidDfaIds;

	/// @brief		私有成员，仅供内部使用。
	CRegRule m_RegexTbl;
};

/*!
@}
*/
