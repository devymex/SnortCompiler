#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <fstream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../compilernew/compilernew.h"
#include "../pcre2nfa/pcre2nfa.h"

void main()
{
	CTimer t;
	CResNew result;
	//result.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	//for (size_t i = 0; i < result.GetRegexTbl().Size(); ++i)
	//{
	//	if (result.GetRegexTbl()[i].GetSigCnt() == 0)
	//	{
	//		std::cout << "No Sig" << std::endl;
	//	}
	//}
	//size_t success_cNt = 0;
	//size_t error_cNt = 0;
	//size_t exceed_cNt = 0;
	//size_t hasbyte_cNt = 0;
	//size_t hasnot_cNt = 0;
	//size_t empty_cNt = 0;
	//size_t hasnosig_cNt = 0;
	//size_t exceedlimit_cNt = 0;
	//for (size_t i = 0; i < result.GetSidDfaIds().Size(); ++i)
	//{
	//	if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULENEW::RES_SUCCESS)
	//	{
	//		++success_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULENEW::RES_ERROR)
	//	{
	//		++error_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULENEW::RES_EXCEED)
	//	{
	//		++exceed_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULENEW::RES_HASBYTE)
	//	{
	//		++hasbyte_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULENEW::RES_HASNOT)
	//	{
	//		++hasnot_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULENEW::RES_EMPTY)
	//	{
	//		++empty_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULENEW::RES_HASNOSIG)
	//	{
	//		++hasnosig_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULENEW::RES_EXCEEDLIMIT)
	//	{
	//		++exceedlimit_cNt;
	//	}
	//	else
	//	{
	//		
	//	}
	//}
	//std::cout << "success: " << success_cNt << std::endl;
	//std::cout << "error: " << error_cNt << std::endl;
	//std::cout << "exceed: " << exceed_cNt << std::endl;
	//std::cout << "hasbyte: " << hasbyte_cNt << std::endl;
	//std::cout << "hasnot: " << hasnot_cNt << std::endl;
	//std::cout << "empty: " << empty_cNt << std::endl;
	//std::cout << "hasnosig: " << hasnosig_cNt << std::endl;
	//std::cout << "exceedlimit: " << exceedlimit_cNt << std::endl;
	//std::cout << success_cNt + error_cNt + exceed_cNt + hasbyte_cNt + hasnot_cNt + empty_cNt + hasnosig_cNt + exceedlimit_cNt << std::endl;
	//std::cout << result.GetSidDfaIds().Size() << std::endl;

	compilenew(_T("..\\allrules.rule"), result);
	result.WriteToFile(_T("..\\..\\output\\result_2.cdt"));
	std::cout << t.Reset() << std::endl;
	system("pause");
}
