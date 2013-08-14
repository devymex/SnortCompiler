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
* @addtogroup groupCommon
* @{
*/

/*!
* @brief		日志类。
* @remark		不应直接使用该类构造对象。应使用全局对象g_log。
*/
class LOGGERHDR CLogger
{
public:
	/// @brief		构造函数。
	CLogger();

	/// @brief		析构函数。
	~CLogger();

	/// @brief		设置日志文件
	void SetLogFile(const char *pStrFile);

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
	/// @brief		私有成员，仅供内部使用。
	CLogger(const CLogger &other);

	/// @brief		私有成员，仅供内部使用。
	CLogger& operator = (const CLogger &other);

	/// @brief		私有成员，仅供内部使用。
	LOGFILE *m_pLogFile;
};

/// 日志类的全局对象。
LOGGERHDR extern CLogger g_log;


/*!
@}
*/
