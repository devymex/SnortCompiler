/*!
* @file			grouping.h
* @author		Lab 435, Xidian University
* @brief		�����㷨
*/

#pragma once

#include <hwprj\compres.h>
#include <hwprj\groupres.h>

#ifndef GROUPINGHDR_DS
#define GROUPINGHDR __declspec(dllimport)
#else
#define GROUPINGHDR __declspec(dllexport)
#endif

/// @brief		��DFA���з��飬������ͬsignature��DFA��Ϊһ��
/// @param		res ������PCRE������ɹ���DFA����groupRes ������
GROUPINGHDR void Grouping(class CCompileResults &res, CGroupRes &groupRes);
