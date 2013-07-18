#pragma once


#ifndef DFACOMPRESS_DS
#define DFACOMPRESS __declspec(dllimport)
#else
#define DFACOMPRESS __declspec(dllexport)
#endif

DFACOMPRESS void DfaCompress();
