/*!
* @file			common.h
* @author		Lab 435, Xidian University
* @brief		通用基本数据类型的定义，常量的定义等。
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

/*!
* @addtogroup groupCommon
* @{
*/

/*!
* 通用基本数据类型
*/
typedef unsigned char			byte;
typedef unsigned long			ulong;
typedef unsigned short			ushort;
typedef char*					pstr;
typedef const char*				pcstr;
typedef ushort					STATEID;
typedef ulong					DFAID;
typedef ulong					SIGNATURE;

/*!
* 全局常量
*/
#define SC_MAXDFASIZE			255
#define SC_DFACOLCNT			256
#define SC_STATELIMIT			500
#define SC_CHAINRESERV			8
#define SC_NFAROWRESERV			500

#ifndef null
#define null					0
#endif

/*!
@}
*/
