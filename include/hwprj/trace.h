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
* @addtogroup groupCommon
* @{
*/

/*!
* @brief	异常类。
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

	/// @brief		获得发生异常的代码行号。
	ulong Line()
	{
		return m_ulLine;
	}

	/// @brief		获得异常信息字符串。
	pcstr What()
	{
		return m_pMsg;
	}

	/// @brief		获得发生异常的代码文件名。
	pcstr File()
	{
		return m_pFile;
	}

private:
	/// @brief		私有成员，仅供内部使用。
	pcstr m_pFile;

	/// @brief		私有成员，仅供内部使用。
	pcstr m_pMsg;

	/// @brief		私有成员，仅供内部使用。
	ulong m_ulLine;
};


/// @breif		通用异常信息字符串的宏定义。
TRACEHDR extern pcstr TI_BADALLOC;		/// 内存分配错误。
TRACEHDR extern pcstr TI_NOTFOUND;		/// 指定的内容未找到。
TRACEHDR extern pcstr TI_OUTOFRANGE;	/// 越界访问。
TRACEHDR extern pcstr TI_INVALIDDATA;	/// 输入的数据错误。
TRACEHDR extern pcstr TI_BADPARAM;		/// 参数错误。
TRACEHDR extern pcstr TI_UNSUPPORT;		/// 该操作尚未被支持。
TRACEHDR extern pcstr TI_NOTALLOW;		/// 该操作已被禁止。
TRACEHDR extern pcstr TI_INCOMPATIBLE;	/// 指定的数据不兼容。

/// @brief		抛出一个CTrace异常，包括当前的代码文件、行号以及错误信息。
/// @param		m 异常信息字符串，可从通用异常信息字符串列表中选择。
#define TTHROW(m) throw CTrace(__FILE__, __LINE__, m);

#ifdef _DEBUG
/// @brief		断言宏
/// @param		s 任意表达式
/// @remark		若s为真则不做任何操作，否则，抛出以s为信息字符串的异常。
#define TASSERT(s)	if (!(s)) {TTHROW("!("#s")");}
/// @brief		诊断宏
/// @param		s 任意表达式
/// @remark		若s为真则不做任何操作，否则，抛出以s为信息字符串的异常。
#define TVERIFY(s)	if (!(s)) {TTHROW("!("#s")");}
#else
/// @brief		断言宏。
/// @param		s 任意表达式。
/// @remark		非Debug模式下不做任何操作。
#define TASSERT(s)	;
/// @brief		断言宏。
/// @param		s 任意表达式。
/// @remark		非Debug模式下仅执行s。
#define TVERIFY(s)	(s);
#endif

/*!
@}
*/
