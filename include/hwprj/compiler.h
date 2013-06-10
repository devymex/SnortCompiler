#pragma once

#include <hwprj\compres.h>

#ifndef COMPILERNEW_H_
#define COMPILERNEW __declspec(dllimport)
#else
#define COMPILERNEW __declspec(dllexport)
#endif

COMPILERNEW void CompileRuleSet(LPCTSTR filename, CCompileResults &result);
