/*!
* @file			common.h
* @author		Lab 435, Xidian University
* @brief		ͨ�û����������͵Ķ��壬�����Ķ���ȡ�
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once

/*!
* @addtogroup groupCommon
* @{
*/

/*!
* ͨ�û�����������
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
* ȫ�ֳ���
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
