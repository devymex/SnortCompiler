/*!
* @file				common.h
**
* @author			Lab 435, Xidian University
**
* @brief			Common basic type and constant defination
**
* Common type defination, compile configurations and error codes
**
*/

#pragma once

/*!
* Common type defination
*/
typedef unsigned char			byte;
typedef unsigned long			ulong;
typedef unsigned short			ushort;
typedef char*					pstr;
typedef const char*				pcstr;
typedef ushort					STATEID;
typedef ulong					DFAID;
typedef ulong					SIGNATURE;

#ifndef null
#define null					0
#endif

/*!
* Compile configurations
*/
#define SC_MAXDFASIZE			255
#define SC_DFACOLCNT			256
#define SC_STATELIMIT			500
#define SC_CHAINRESERV			8
#define SC_NFAROWRESERV			500
