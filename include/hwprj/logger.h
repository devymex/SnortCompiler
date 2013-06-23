/*!
* @file		dllstring.h
* @author	Lab 435, Xidian University
* @brief	Common string class
* @remark	Definition of the CDllString class
*/

#pragma once

#include <hwprj\common.h>

#ifndef LOGGERHDR_DS
class LOGFILE;
#define LOGGERHDR __declspec(dllimport)
#else
#define LOGGERHDR __declspec(dllexport)
#endif

/*!
* @brief	��־��
* @remark	����ֱ�ӹ���������Ӧʹ��ȫ�ֱ���g_log���÷���ͬstd::cout��
*			��ѡ���������Ļ���ļ���
*/
class LOGGERHDR CLogger
{
public:
	/// @brief		���캯����
	CLogger();
	~CLogger();

	CLogger& operator << (char n);
	CLogger& operator << (unsigned char n);
	CLogger& operator << (short n);
	CLogger& operator << (unsigned short n);
	CLogger& operator << (int n);
	CLogger& operator << (unsigned int n);
	CLogger& operator << (long n);
	CLogger& operator << (unsigned long n);
	CLogger& operator << (float n);
	CLogger& operator << (double n);
	CLogger& operator << (const void *p);
	CLogger& operator << (const char *p);
	CLogger& operator << (const CLogger &l);
	const char* nl;

private:
	CLogger(const CLogger &other);
	CLogger& operator = (const CLogger &other);
	LOGFILE *m_pLogFile;
};

LOGGERHDR extern CLogger g_log;