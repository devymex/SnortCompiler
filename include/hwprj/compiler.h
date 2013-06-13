#pragma once

#include <hwprj\compres.h>
#include <hwprj\snortrule.h>

#ifndef COMPILERHDR_DS
#define COMPILERHDR __declspec(dllimport)
#else
#define COMPILERHDR __declspec(dllexport)
#endif

typedef void (__stdcall *RECIEVER)(const CSnortRule &rule, void *lpParam);

COMPILERHDR void CompileRuleSet(const char *filename, CCompileResults &result);

COMPILERHDR ulong CompileFile(const char *fileName, RECIEVER recv, void *lpUser);

COMPILERHDR void Rule2Dfas(const CSnortRule &rule, CCompileResults &result,
							COMPILEDRULENEW &ruleResult);
