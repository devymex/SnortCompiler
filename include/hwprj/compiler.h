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

COMPILERHDR void ParseRuleFile(const char *pFileName, RECIEVER recv, void *lpUser);

COMPILERHDR void CompileRuleFile(const char *pFileName, CCompileResults &compRes);

typedef std::vector<byte>               BYTEARY1;
typedef std::vector<BYTEARY1>			CHAINCOMPDATA1;
typedef std::vector<CHAINCOMPDATA1>		RULECOMPDATA1;

COMPILERHDR void ProcessRule(CRegRule &regRule, RULECOMPDATA1 &result);
