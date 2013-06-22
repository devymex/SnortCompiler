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

/*	函数名：Grouping
**	参数：res-由所有PCRE链编译成功的DFA集，groupRes-分组结果
**	功能：对DFA进行分组，具有相同signature的DFA分为一组
*/
GROUPINGHDR void Grouping(class CCompileResults &res, CGroupRes &groupRes);
