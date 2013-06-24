/*!
* @file			logger.h
* @author		Lab 435, Xidian University
* @brief		定义了CLogger类及其导出符号
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
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
* @brief	日志类
* @remark	不可直接构造此类对象，应使用全局变量g_log。用法如同std::cout，
*			可选择输出到屏幕或文件。
*/
class LOGGERHDR CLogger
{
public:
	/// @brief		构造函数。
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