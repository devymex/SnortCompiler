/*!
* @file			trace.h
* @author		Lab 435, Xidian University
* @brief		定义了CTrace类及其导出符号
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once
#include <hwprj\common.h>

#ifndef TRACEHDR_DS
#define TRACEHDR __declspec(dllimport)
#else
#define TRACEHDR __declspec(dllexport)
#endif

/*!
* @brief	异常类
* @remark	应通过宏来使用此类，用法见相关宏定义的说明。
*/
class TRACEHDR CTrace
{
public:
	/// @brief		构造函数。
	CTrace(pcstr pFile, ulong ulLine, pcstr pMsg)
		: m_pFile(pFile), m_pMsg(pMsg), m_ulLine(ulLine)
	{
	}
	ulong Line()
	{
		return m_ulLine;
	}
	pcstr What()
	{
		return m_pMsg;
	}
	pcstr File()
	{
		return m_pFile;
	}

private:
	/// @brief		私有成员，仅供内部使用
	pcstr m_pFile;

	/// @brief		私有成员，仅供内部使用
	pcstr m_pMsg;

	/// @brief		私有成员，仅供内部使用
	ulong m_ulLine;
};

TRACEHDR extern pcstr TI_BADALLOC;
TRACEHDR extern pcstr TI_NOTFOUND;
TRACEHDR extern pcstr TI_OUTOFRANGE;
TRACEHDR extern pcstr TI_INVALIDDATA;
TRACEHDR extern pcstr TI_BADPARAM;
TRACEHDR extern pcstr TI_UNSUPPORT;
TRACEHDR extern pcstr TI_NOTALLOW;
TRACEHDR extern pcstr TI_INCOMPATIBLE;

/// @brief		抛出一个CTrace异常，包括当前的代码文件、行号以及错误信息。
#define TTHROW(m) throw CTrace(__FILE__, __LINE__, m);

#ifdef _DEBUG
/// @brief		断言宏
#define TASSERT(s)	if (!(s)) {TTHROW("!("#s")");}
/// @brief		诊断宏
#define TVERIFY(s)	if (!(s)) {TTHROW("!("#s")");}
#else
#define TASSERT(s)	;
#define TVERIFY(s)	(s);
#endif


