/*!
* @file				grouping.h
**
* @author			Lab 435, Xidian University
**
* @brief			Grouping algorithm
**
* Declaration of CGROUPS and CGROUPRes class
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

/// @param		res 由所有PCRE链编译成功的DFA集及一些其他信息(包括每个DFA的Signature等)，groupRes-分组结果
/// @brief		对DFA进行分组，具有相同signature的DFA分为一组
GROUPINGHDR void Grouping(class CCompileResults &res, CGroupRes &groupRes);
