#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <fstream>
#include "../compiler/compiler.h"
#include "../common/common.h"

void main()
{
	CRes result;
	compile(_T("..\\..\\input\\test.rules"), result);
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
