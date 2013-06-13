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
#include <hwprj\snortrule.h>

#ifndef COMPILERHDR_DS
#define COMPILERHDR __declspec(dllimport)
#else
#define COMPILERHDR __declspec(dllexport)
#endif

struct PARSERESULT
{
	CRegRule	regRule;
	ulong		ulFlags;
	ulong		ulSid;
	ulong		ulRet;
};

typedef void (__stdcall *RECIEVER)(const PARSERESULT &parseRes, void *lpParam);

COMPILERHDR void __stdcall CompileResult(const PARSERESULT &parseRes, void *lpVoid);

COMPILERHDR ulong ParseRuleFile(const char *pFileName, RECIEVER recv, void *lpUser);
