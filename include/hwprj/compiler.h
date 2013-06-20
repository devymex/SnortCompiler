/*
**	@file		compiler.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Primary file of compiler
**
**	Functions declaration of access enter
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
	static const TYPE PARSE_DEFAULT		= 0x0000;
	static const TYPE PARSE_HASBYTE		= 0x0001;
	static const TYPE PARSE_HASNOT		= 0x0002;
	static const TYPE PARSE_ERROR		= 0x0003;
	static const TYPE PARSE_EMPTY		= 0x0004;
};

struct PARSERESULT
{
	CRegRule		regRule;
	ulong			ulSid;
	PARSEFLAG::TYPE	ulFlag;
};

typedef void (__stdcall *RECIEVER)(const PARSERESULT &parseRes, void *lpUser);

/*	ParseRuleFile
**	参数：输入规则文件路径，回调函数解析规则，输出规则对应的DFA
**	功能：解析规则
*/
COMPILERHDR void ParseRuleFile(const char *pFileName, RECIEVER recv, void *lpUser);

/*	接口函数：CompileRuleFile
**	参数：输入规则文件路径，输出规则对应的DFA
**	功能：处理Snort规则，将规则中的content、uricontent、pcre选项编写为PCRE链
**	将PCRE链编译为DFA，并提取连续4字节长度signature
*/
COMPILERHDR void CompileRuleFile(const char *pFileName, CCompileResults &compRes);
