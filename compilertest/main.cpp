#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <fstream>
#include "../compiler/compiler.h"
#include "../common/common.h"

void main()
{
	CRes result;
	compile(_T("..\\..\\input\\testrules.rule"), result);
	size_t count = 0;
	for (size_t i = 0; i < result.GetSidDfaIds().Size(); ++i)
	{
		if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULE::RES_SUCCESS)
		{
			++count;
		}
	}
	std::cout << count << std::endl;
	result.WriteToFile(_T("..\\..\\output\\result.cdt"));

	//CRes Res;
	//Res.GetSidDfaIds().Resize(1);
	//COMPILEDRULE &ruleResult = Res.GetSidDfaIds().Back();
	//ruleResult.m_nSid = 1292;
	//ruleResult.m_nResult = COMPILEDRULE::RES_SUCCESS;
	//ruleResult.m_dfaIds.PushBack(1);
	//Res.GetDfaTable().Resize(1);
	//CDfa &dfa = Res.GetDfaTable()[0];
	//BYTE pGroup[DFACOLSIZE];
	//pGroup[0] = 0;
	//pGroup[1] = 1;
	//pGroup[2] = 2;
	//pGroup[3] = 3;
	//pGroup[4] = 4;
	//for (size_t i = 5; i < DFACOLSIZE - 1; ++i)
	//{
	//	pGroup[i] = 5;
	//}
	//pGroup[DFACOLSIZE - 1] = 0;
	//dfa.SetGroup(pGroup);
	//dfa.Resize(1);
	//for (size_t i = 0; i < 6; ++i)
	//{
	//	dfa[0][i] = i;
	//}
	//Res.WriteToFile(_T("..\\..\\output\\result.cdt"));

	//CRes readRes;
	//readRes.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	//readRes.WriteToFile(_T("..\\..\\output\\result1.cdt"));

	//CRes readRes;
	//readRes.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	//size_t count = 0;
	//for (size_t i = 0; i < readRes.GetSidDfaIds().Size(); ++i)
	//{
	//	if (readRes.GetSidDfaIds()[i].m_nResult == COMPILEDRULE::RES_SUCCESS)
	//	{
	//		++count;
	//	}
	//}
	//std::cout << count << std::endl;


	//readRes.WriteToFile(_T("..\\..\\output\\WriteAfterRead.cdt"));

	//std::ofstream foutBeforeWrite("..\\..\\output\\BeforeWrite.txt");

	//for (size_t i = 0; i < result.GetDfaTable().Size(); ++i)
	//{
	//	for (size_t j = 0; j < result.GetDfaTable()[i].Size(); ++j)
	//	{
	//		foutBeforeWrite << j << ":";
	//		for (size_t k = 0; k < CHARSETSIZE; ++k)
	//		{
	//			foutBeforeWrite << "(" << k << "," << result.GetDfaTable()[i][j][k] << ")";
	//		}
	//		foutBeforeWrite << std::endl;
	//	}
	//}
	//foutBeforeWrite.close();
	//foutBeforeWrite.clear();

	//std::ofstream foutAfterRead("..\\..\\output\\AfterRead.txt");

	//for (size_t i = 0; i < readRes.GetDfaTable().Size(); ++i)
	//{
	//	for (size_t j = 0; j < readRes.GetDfaTable()[i].Size(); ++j)
	//	{
	//		foutAfterRead << j << ":";
	//		for (size_t k = 0; k < CHARSETSIZE; ++k)
	//		{
	//			foutAfterRead << "(" << k << "," << result.GetDfaTable()[i][j][k] << ")";
	//		}
	//		foutAfterRead << std::endl;
	//	}
	//}
	//foutAfterRead.close();
	//foutAfterRead.clear();

	system("pause");
}
