/*!
* @file				grouping.h
* @author			Lab 435, Xidian University
* @brief			定义了分组函数Grouping。
* @copyright		本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

#include <hwprj\compres.h>
#include <hwprj\groupres.h>

#ifndef GROUPINGHDR_DS
#define GROUPINGHDR __declspec(dllimport)
#else
#define GROUPINGHDR __declspec(dllexport)
#endif

/*! @addtogroup groupGrouphash
*  @{
*/
/// @brief		对DFA进行分组，具有相同signature的DFA分为一组。
/// @param[in]	res 由所有PCRE链编译成功的DFA集及一些其他信息(包括每个DFA的Signature等)。
/// @param[out]	groupRes 分组结果。
GROUPINGHDR void Grouping(class CCompileResults &res, CGroupRes &groupRes);

/*!
@}
*/
