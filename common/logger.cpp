#include "stdafx.h"
#include <hwprj\logger.h>

LOGGERHDR CLogger g_log;

LOGGERHDR CLogger::CLogger()
	: nl("\r\n")
{
	//m_pLogFile = &std::cout;
	m_pLogFile = new std::ofstream("compile.log");
	//m_pLogFile->set_rdbuf(std::cout.rdbuf());
}

LOGGERHDR CLogger::~CLogger()
{
	delete m_pLogFile;
}

LOGGERHDR CLogger::CLogger(const CLogger&)
{
}

void CLogger::SetLogFile(const char *pStrFile)
{
	delete m_pLogFile;
	m_pLogFile = new std::ofstream(pStrFile);
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
