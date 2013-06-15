/*
**	@file		common.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Common classes declaration
**
**	Common type defination, compile configurations and error codes
**
*/

#pragma once

/* Common type defination */
typedef unsigned char			byte;
typedef unsigned long			ulong;
typedef unsigned short			ushort;
typedef char*					pstr;
typedef const char*				pcstr;
typedef ushort					STATEID;
typedef ulong					DFAID;
typedef ulong					SIGNATURE;

/* compile configurations */
#define SC_MAXDFASIZE	255
#define SC_DFACOLCNT	256
//#define SC_CHARSETSIZE	260
#define SC_STATELIMIT	500

/* error codes */
#define SC_SUCCESS		ulong(0)
#define SC_ERROR		ulong(-1)

