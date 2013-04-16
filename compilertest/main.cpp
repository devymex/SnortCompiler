#include <iostream>
#include <tchar.h>
#include <windows.h>
#include "../compiler/compiler.h"
#include "../common/common.h"

void main()
{
	CRes result;
	compile(_T("..\\testrules.rule"), result);
	result.WriteToFile(_T("..\\result.cdt"));

	std::cout << result.GetDfaTable()[0].Size() << std::endl;

	//std::cout << result.GetSidDfaIds()[0].m_dfaIds.Size() << std::endl;

	//for (size_t i = 0; i < result.GetDfaTable().Size(); ++i)
	//{
	//	for (size_t j = 0; j < result.GetDfaTable()[i].Size(); ++j)
	//	{
	//		std::cout << j << ":";
	//		for (size_t k = 0; k < CHARSETSIZE; ++k)
	//		{
	//			if (result.GetDfaTable()[i][j][k] != 0 && result.GetDfaTable()[i][j][k] != -1)
	//			{
	//				std::cout << "(" << k << "," << result.GetDfaTable()[i][j][k] << ")";
	//			}
	//		}
	//		std::cout << std::endl;
	//	}
	//}

	system("pause");
}