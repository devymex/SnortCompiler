#include "stdafx.h"
#include <hwprj\logger.h>

LOGGERHDR CLogger g_log;

LOGGERHDR CLogger::CLogger()
	: nl("\n\r")
{
	m_pLogFile = &std::cout;
	//m_pLogFile = new std::ofstream("compile.log");
}

LOGGERHDR CLogger::~CLogger()
{
	if (m_pLogFile != &std::cout)
	{
		delete m_pLogFile;
	}
}

LOGGERHDR CLogger::CLogger(const CLogger&)
{
}

LOGGERHDR CLogger& CLogger::operator = (const CLogger&)
{
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (char val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (unsigned char val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (short val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (unsigned short val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (int val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (unsigned int val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (long val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (unsigned long val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (float val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (double val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (const void *val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (const char *val)
{
	(*m_pLogFile) << val;
	return *this;
}

LOGGERHDR CLogger& CLogger::operator << (const CLogger &l)
{
	(*m_pLogFile) << (*l.m_pLogFile);
	return *this;
}
