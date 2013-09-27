/*!
* @file			logger.h
* @author		Lab 435, Xidian University
* @brief		������CLogger�༰�䵼������
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
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
* @brief		��־�ࡣ
* @remark		��Ӧֱ��ʹ�ø��๹�����Ӧʹ��ȫ�ֶ���g_log��
*/
class LOGGERHDR CLogger
{
public:
	/// @brief		���캯����
	CLogger();

	/// @brief		����������
	~CLogger();

	/// @brief		������־�ļ�
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
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	CLogger(const CLogger &other);

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	CLogger& operator = (const CLogger &other);

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	LOGFILE *m_pLogFile;
};

/// ��־���ȫ�ֶ���
LOGGERHDR extern CLogger g_log;


/*!
@}
*/
