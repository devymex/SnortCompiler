#pragma once
#include "../common/common.h"
#include <windows.h>
#ifndef PARSERULE_H_
#define PARSERULE __declspec(dllimport)
#else
#define PARSERULE __declspec(dllexport)
#endif


typedef void (CALLBACK *RECIEVER)(const CSnortRule &rule);

PARSERULE size_t ParseRule(LPCTSTR fileName, RECIEVER recv);

