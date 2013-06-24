/*!
* @file				compiler.h
**
* @author			Lab 435, Xidian University
**
* @brief			Primary file of compiler
**
* Functions declaration of access enter
**
*/

#pragma once

#include <hwprj\compres.h>

#ifndef COMPILERHDR_DS
#define COMPILERHDR __declspec(dllimport)
#else
#define COMPILERHDR __declspec(dllexport)
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief		定义了解析SNORT规则时的标记
*/
struct PARSEFLAG {
	typedef ulong TYPE;

	/// 解析成功。
	static const TYPE PARSE_SUCCESS		= 0;

	/// 规则包含byte_test或byte_jump选项，无法处理。
	static const TYPE PARSE_HASBYTE		= (1 << 0);

	/// 规则包含'!'（逻辑非）的选项，无法处理。
	static const TYPE PARSE_HASNOT		= (1 << 1);

	/// 规则解析出错。
	static const TYPE PARSE_ERROR		= (1 << 2);

	/// 规则不含任何可编译的选项。
	static const TYPE PARSE_EMPTY		= (1 << 3);
};


/*!
* @brief		解析SNORT规则后产生的结果数据结构
*/
struct PARSERESULT
{
	/// @brief		规则解析后产生的结构化规则对象。
	CRegRule regRule;

	/// @brief		该规则的sid。
	ulong ulSid;

	/// @brief		解析该规则时产生的标记。
	/// @see		@ref PARSEFLAG
	PARSEFLAG::TYPE ulFlag;
};

/// @brief		解析规则使用的回调函数。
/// @param[in]	parseRes 解析结果数据。
/// @param[in]	lpUser 调用者指定的自定义参数。
typedef void (__stdcall *RECIEVER)(const PARSERESULT &parseRes, void *lpUser);

/// @brief		解析一个Snort规则文件，并通过回调函数给出结果。
/// @param		pFileName 输入规则文件名。
/// @param		recv 回调函数的地址。
///	@param		lpUser 自定义参数，将被传入回调函数。
COMPILERHDR void ParseRuleFile(const char *pFileName, RECIEVER recv, void *lpUser);

/// @brief		编译一个Snort规则文件，生成编译结果集。
/// @param[in]	pFileName 输入规则文件名。
/// @param[out]	compRes 编译结果集对象。
/// @remark		将规则中的content、uricontent、pcre选项编写为PCRE链将PCRE链编译为DFA，
///				并提取连续4字节长度signature。
COMPILERHDR void CompileRuleFile(const char *pFileName, CCompileResults &compRes);

COMPILERHDR void ExtractSequence(const CByteArray &pcResult, std::vector<CByteArray> &seqAry);

COMPILERHDR void ExtractSignatures(const CByteArray &seqAry, CUnsignedArray &sigs);

/// @brief		利用PCRE选项的预编译数据，编译生成NFA。
/// @param[in]	pcResult PCRE选项的预编译数据。
/// @param[in]	bFromBeg 指定该PCRE是否需要从数据包首部开始匹配。
/// @param[out]	nfa 一个CNfa类型的nfa。
COMPILERHDR void CodeToNFA(const CByteArray &pcResult, bool bFromBeg, CNfa &nfa);

/*!
@}
*/
