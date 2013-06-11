#pragma once

#include <hwprj\compres.h>
#include <hwprj\snortrule.h>

#ifndef COMPILERNEW_H_
#define COMPILERNEW __declspec(dllimport)
#else
#define COMPILERNEW __declspec(dllexport)
#endif

typedef void (__stdcall *RECIEVER)(const CSnortRule &rule, void *lpParam);

COMPILERNEW void CompileRuleSet(const char *filename, CCompileResults &result);

COMPILERNEW ulong CompileFile(const char *fileName, RECIEVER recv, void *lpUser);

COMPILERNEW void Rule2Dfas(const CSnortRule &rule, CCompileResults &result,
						   COMPILEDRULENEW &ruleResult);
