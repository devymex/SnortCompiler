/*!
* @file			pcrematch.h
* @author		Lab 435, Xidian University
* @brief		定义了PCRE匹配函数。
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

#include <hwprj\pcreopt.h>

#ifndef PCREMATCHHDR_DS
#define PCREMATCHHDR __declspec(dllimport)
#else
#define PCREMATCHHDR __declspec(dllexport)
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief		判断PCRE与数据包是否匹配
* @param[in]	src 输入数据包二进制字节流
* @param[in]	length 数据包长度
* @param[in]	Regex 输入PCRE
* @param[out]	Pos 匹配位置
* @retval		true PCRE与数据包匹配成功
* @retval		false PCRE与数据包匹配失败
*/
PCREMATCHHDR bool PcreMatch(const char* src,
							int length,
							CPcreOption &pcreopt,
							int &Pos);

/*!
@}
*/
