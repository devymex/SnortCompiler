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

/*	��������ParseRuleFile
**	������pFileName-��������ļ�·����recv-�ص�������������lpUser-��������Ӧ��DFA
**	���ܣ���������
*/
COMPILERHDR void ParseRuleFile(const char *pFileName, RECIEVER recv, void *lpUser);

/*	�ӿں�����CompileRuleFile
**	������pFileName-��������ļ�·����compRes-��������Ӧ��DFA
**	���ܣ�����Snort���򣬽������е�content��uricontent��pcreѡ���дΪPCRE��
**	��PCRE������ΪDFA������ȡ����4�ֽڳ���signature
*/
COMPILERHDR void CompileRuleFile(const char *pFileName, CCompileResults &compRes);

COMPILERHDR void ExtractSequence(const CByteArray &pcResult, std::vector<CByteArray> &seqAry);

COMPILERHDR void ExtractSignatures(const CByteArray &seqAry, CUnsignedArray &sigs);

/*	��������CodeToNFA
**	������pcResult-PCRE����bFromBeg-�ж�pcre�п�ʼλ���Ƿ����"^"��nfa-һ��CNfa���͵�nfa
**	���ܣ�����PCRE��������һ��NFA
*/
COMPILERHDR void CodeToNFA(const CByteArray &pcResult, bool bFromBeg, CNfa &nfa);
