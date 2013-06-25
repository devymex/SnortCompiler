/*!
* @file			common.h
* @author		Lab 435, Xidian University
* @brief		Դ�����ĵ���ͨ�û����������͵Ķ��壬�����Ķ���ȡ�
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
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


/// �ɽ��ܵ�����DFA״̬����
#define SC_MAXDFASIZE			255

/// DFAĬ���ַ����Ĵ�С��
#define SC_DFACOLCNT			256

/// ����ΪDFAʱ����NFA��״̬����
#define SC_STATELIMIT			500

/// ��������ʱÿ����Ԥ���Ĵ�С��
#define SC_CHAINRESERV			8		

#ifndef null
/// ��ָ�롣
#define null					0
#endif

/*!
@}
*/
