/*!
* @file			grouping.h
* @author		Lab 435, Xidian University
* @brief		分组算法
*/

#pragma once

#include <hwprj\compres.h>
#include <hwprj\groupres.h>

#ifndef GROUPINGHDR_DS
#define GROUPINGHDR __declspec(dllimport)
#else
#define GROUPINGHDR __declspec(dllexport)
#endif

/// @brief		对DFA进行分组，具有相同signature的DFA分为一组
/// @param		res 由所有PCRE链编译成功的DFA集，groupRes 分组结果
GROUPINGHDR void Grouping(class CCompileResults &res, CGroupRes &groupRes);
