/*
**	@file		pcrematch.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Match the packet and pcre library
**
*/

#pragma once

#ifndef PCREMATCHHDR_DS
#define PCREMATCHHDR __declspec(dllimport)
#else
#define PCREMATCHHDR __declspec(dllexport)
#endif

PCREMATCHHDR bool PcreMatch(const char* src, int length, std::string Regex, int &Pos);
