#pragma once
//#include "stdafx.h"

#include "../SnortCommon/snortcommon.h"

#define HASHMODULO 1000000

#define EMPTY 256

#ifndef CREDFA_H_
#define CREDFA __declspec(dllimport)
#else
#define CREDFA __declspec(dllexport)
#endif

CREDFA void NfaToDfa(CNfa &oneNfaTab, CDfa &dfaTab);
