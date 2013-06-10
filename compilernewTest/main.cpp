#include <iostream>
#include <tchar.h>
#include <windows.h>


#include <hwprj\compiler.h>



//size_t StatisticsSigToFile(LPCTSTR filename, CResNew &result)
//{
//	std::ofstream fout(filename);
//	if (!fout)
//	{
//		std::cerr << "Open file Failed!" << std::endl;
//		return (size_t)-1;
//	}
//
//	CSidDfaIdsNew &sdi = result.GetSidDfaIds();
//	CRegRule &rg = result.GetRegexTbl();
//
//	for (size_t i = 0; i < sdi.Size(); ++i)
//	{
//		COMPILEDRULENEW &tmpRes = sdi[i];
//		if (tmpRes.m_nResult == COMPILEDRULENEW::RES_SUCCESS)
//		{
//			if (tmpRes.m_dfaIds.Size() > 0)
//			{
//				//fout << tmpRes.m_nSid << "\t" << rg[tmpRes.m_dfaIds[0]].GetSigs.GetSigCnt() << std::endl;
//			}
//		}
//	}
//
//	fout.close();
//}
//

void main()
{
	CTimer t;
	CCompileResults result;
	CompileRuleSet(_T("..\\allrules.rule"), result);
	result.WriteToFile(_T("..\\result.cdt"));
	//StatisticsSigToFile(_T("..\\contentSig.txt"), result);
	//StatisticsSigToFile(_T("..\\allSig.txt"), result);
	//result.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	//result.WriteToFile(_T("..\\..\\output\\result1.cdt"));

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
	//for (ULONG i = 0; i < result.GetSidDfaIds().Size(); ++i)
	//{
	//	if (result.GetSidDfaIds()[i].m_nResult == COMPILEDRULENEW::RES_SUCCESS)
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

	//ULONG success_cNt = 0;
	//ULONG error_cNt = 0;
	//ULONG exceed_cNt = 0;
	//ULONG hasbyte_cNt = 0;
	//ULONG hasnot_cNt = 0;
	//ULONG empty_cNt = 0;
	//ULONG hasnosig_cNt = 0;
	//ULONG exceedlimit_cNt = 0;
	//for (ULONG i = 0; i < result.GetSidDfaIds().Size(); ++i)
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

	std::cout << t.Reset() << std::endl;
	system("pause");
}
