/**
**  @file        common.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CDllArray, CDllString
**
*/

#pragma once

#include <windows.h>
#include <vector>
#include <list>

/* compile configurations */
#define SC_MAXDFASIZE	255
#define SC_DFACOLCNT	256
#define SC_CHARSETSIZE	260
#define SC_STATELIMIT	500
#define SC_SUCCESS		0

#define SC_ERROR		ULONG(-1)
#define SC_EXCEED		ULONG(-2)


/* Common type defination */
typedef unsigned short			STATEID;
typedef std::list<STATEID>		STATELIST;
typedef std::vector<STATEID>	STATEVEC;
typedef STATELIST::iterator		STATELIST_ITER;
typedef STATEVEC::iterator		STATEVEC_ITER;
typedef class CDllArray	CStateSet;

class CTimer
{
public:
	__forceinline CTimer()
	{
		QueryPerformanceFrequency((PLARGE_INTEGER)&m_nFreq);
		QueryPerformanceCounter((PLARGE_INTEGER)&m_nStart);
	}
	__forceinline double Cur()
	{
		__int64 nCur;
		double dCur;

		QueryPerformanceCounter((PLARGE_INTEGER)&nCur);
		dCur = double(nCur - m_nStart) / double(m_nFreq);

		return dCur;
	}
	__forceinline double Reset()
	{
		__int64 nCur;
		double dCur;

		QueryPerformanceCounter((PLARGE_INTEGER)&nCur);
		dCur = double(nCur - m_nStart) / double(m_nFreq);
		m_nStart = nCur;

		return dCur;
	}
protected:
	__int64 m_nFreq;
	__int64 m_nStart;
};
