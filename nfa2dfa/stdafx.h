#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <windows.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <unordered_map>
#include <stack>

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
private:
	__int64 m_nFreq;
	__int64 m_nStart;
};

#define CREDFA_H_