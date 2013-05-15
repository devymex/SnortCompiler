#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <fstream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../compilernew/compilernew.h"

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

void main()
{
	CResNew result;
	//result.ReadFromFile(_T("..\\..\\output\\result(back_up).cdt"));
	//CResNew resultnew;
	//resultnew.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	//for (size_t i = 0; i < result.GetSidDfaIds().Size(); ++i)
	//{
	//	for (size_t j = 0; j < result.GetSidDfaIds()[i].m_dfaIds.Size(); ++j)
	//	{
	//		if (result.GetSidDfaIds()[i].m_dfaIds[j] == 227)
	//		{
	//			std::cout << result.GetSidDfaIds()[i].m_nSid << std::endl;
	//		}
	//	}
	//}
	//for (size_t i = 0; i < result.GetDfaTable().Size(); ++i)
	//{
	//	if (result.GetDfaTable()[i].Size() != resultnew.GetDfaTable()[i].Size() || result.GetDfaTable()[i].GetGroupCount() != resultnew.GetDfaTable()[i].GetGroupCount())
	//	{
	//		std::cout << (size_t)result.GetDfaTable()[i].Size() << std::endl;
	//		std::cout << (size_t)resultnew.GetDfaTable()[i].Size() << std::endl;
	//		std::cout << (size_t)result.GetDfaTable()[i].GetGroupCount() << std::endl;
	//		std::cout << (size_t)resultnew.GetDfaTable()[i].GetGroupCount() << std::endl;
	//		continue;
	//	}
	//}
	CTimer t;
	compilenew(_T("..\\..\\input\\allrules.rule"), result);
	result.WriteToFile(_T("..\\..\\output\\result_after.cdt"));
	std::cout << t.Reset() << std::endl;
	system("pause");
}
