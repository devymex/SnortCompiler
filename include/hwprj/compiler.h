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


struct PARSEFLAG {
	typedef ulong TYPE;
	static const TYPE PARSE_SUCCESS		= 0;
	static const TYPE PARSE_HASBYTE		= (1 << 0);
	static const TYPE PARSE_HASNOT		= (1 << 1);
	static const TYPE PARSE_ERROR		= (1 << 2);
	static const TYPE PARSE_EMPTY		= (1 << 3);
};

struct PARSERESULT
{
	CRegRule		regRule;
	ulong			ulSid;
	PARSEFLAG::TYPE	ulFlag;
};

typedef void (__stdcall *RECIEVER)(const PARSERESULT &parseRes, void *lpUser);

/// @brief		解析规则。
/// @param		pFileName 输入规则文件路径。
/// @param		recv 回调函数解析规则。
///	@param		lpUser 输出规则对应的DFA。
COMPILERHDR void ParseRuleFile(const char *pFileName, RECIEVER recv, void *lpUser);

/// @brief		处理Snort规则。
/// @param		pFileName 输入规则文件路径。
/// @param		compRes 输出规则对应的DFA。
/// @remark		将规则中的content、uricontent、pcre选项编写为PCRE链将PCRE链编译为DFA，
///				并提取连续4字节长度signature。
COMPILERHDR void CompileRuleFile(const char *pFileName, CCompileResults &compRes);

COMPILERHDR void ExtractSequence(const CByteArray &pcResult, std::vector<CByteArray> &seqAry);

COMPILERHDR void ExtractSignatures(const CByteArray &seqAry, CUnsignedArray &sigs);

/// @brief		解析PCRE链，构造一个NFA。
/// @param		pcResult PCRE链。
/// @param		bFromBeg 判断pcre中开始位置是否包含'^'。
/// @param		nfa 一个CNfa类型的nfa。
COMPILERHDR void CodeToNFA(const CByteArray &pcResult, bool bFromBeg, CNfa &nfa);
