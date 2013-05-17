#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <fstream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../compilernew/compilernew.h"

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
	compilenew(_T("..\\allrules.rule"), result);
	std::cout << t.Reset() << std::endl;
	result.WriteToFile(_T("..\\result_after.cdt"));
	system("pause");
}
