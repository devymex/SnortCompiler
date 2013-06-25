/*!
* @file			common.h
* @author		Lab 435, Xidian University
* @brief		源代码文档。通用基本数据类型的定义，常量的定义等。
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

/*!
* @addtogroup groupCommon
* @{
*/

typedef unsigned char			byte;
typedef unsigned long			ulong;
typedef unsigned short			ushort;
typedef char*					pstr;
typedef const char*				pcstr;
typedef ushort					STATEID;
typedef ulong					DFAID;
typedef ulong					SIGNATURE;


/// 可接受的最大的DFA状态数。
#define SC_MAXDFASIZE			255

/// DFA默认字符集的大小。
#define SC_DFACOLCNT			256

/// 编译为DFA时最大的NFA的状态数。
#define SC_STATELIMIT			500

/// 解析规则时每条链预留的大小。
#define SC_CHAINRESERV			8		

#ifndef null
/// 空指针。
#define null					0
#endif

/*!
@}
*/
