/*
**	@file		grouping.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Grouping algorithm
**
**	Declaration of CGROUPS and CGROUPRes class
**
*/

#pragma once

#include <hwprj\compres.h>
#include <hwprj\groupres.h>

#ifndef GROUPINGHDR_DS
#define GROUPINGHDR __declspec(dllimport)
#else
#define GROUPINGHDR __declspec(dllexport)
#endif

/*	��������Grouping
**	������res-������PCRE������ɹ���DFA����groupRes-������
**	���ܣ���DFA���з��飬������ͬsignature��DFA��Ϊһ��
*/
GROUPINGHDR void Grouping(class CCompileResults &res, CGroupRes &groupRes);
