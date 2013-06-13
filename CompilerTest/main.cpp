#include <iostream>
#include <tchar.h>
#include <windows.h>

#include <hwprj\compiler.h>
#include <hwprj\ctimer.h>

void main()
{
	CTimer t;
	CCompileResults result;
	CompileRuleSet("..\\allrules.rule", result);
	result.WriteToFile("..\\result.cdt");
	//result.ReadFromFile("..\\..\\output\\result.cdt");
	//result.WriteToFile("..\\..\\output\\result1.cdt");

	//std::ifstream fin("../../input/allrules.rule");
	//std::vector<std::string> vecRules;
	//std::string str;
	//while (std::getline(fin, str))
	//{
	//	vecRules.push_back(str);
	//}
	//fin.clear();
	//fin.close();

	//std::ofstream fout("../../input/ProcessRules.rule");
	//for (ulong i = 0; i < result.GetSidDfaIds().Size(); ++i)
	//{
	//	if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULE::RES_SUCCESS)
	//	{
	//		str = "sid:";
	//		std::stringstream ss;
	//		ss << result.GetSidDfaIds()[i].m_nSid;
	//		str += ss.str();
	//		str += ";";
	//		for (std::vector<std::string>::iterator j = vecRules.begin(); j != vecRules.end(); ++j)
	//		{
	//			if (j->find(str, 0) != std::string::npos)
	//			{
	//				fout << *j << std::endl;
	//			}
	//		}
	//	}
	//}
	//fout.clear();
	//fout.close();

	//ulong success_cNt = 0;
	//ulong error_cNt = 0;
	//ulong exceed_cNt = 0;
	//ulong hasbyte_cNt = 0;
	//ulong hasnot_cNt = 0;
	//ulong empty_cNt = 0;
	//ulong hasnosig_cNt = 0;
	//ulong exceedlimit_cNt = 0;
	//for (ulong i = 0; i < result.GetSidDfaIds().Size(); ++i)
	//{
	//	if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULE::RES_SUCCESS)
	//	{
	//		++success_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULE::RES_ERROR)
	//	{
	//		++error_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULE::RES_EXCEED)
	//	{
	//		++exceed_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULE::RES_HASBYTE)
	//	{
	//		++hasbyte_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULE::RES_HASNOT)
	//	{
	//		++hasnot_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULE::RES_EMPTY)
	//	{
	//		++empty_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULE::RES_HASNOSIG)
	//	{
	//		++hasnosig_cNt;
	//	}
	//	else if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULE::RES_EXCEEDLIMIT)
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

	std::cout << t.Reset() << std::endl;
	system("pause");
}
