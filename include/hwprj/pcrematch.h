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

/*	��������PcreMatch
**	������src-�������ݰ��������ֽ�����length-���ݰ����ȣ�Regex-����PCRE��Pos-ƥ��λ��
**	���ܣ��ж�PCRE�����ݰ��Ƿ�ƥ�䣬����true����false
*/
PCREMATCHHDR bool PcreMatch(const char* src, int length, std::string Regex, int &Pos);
