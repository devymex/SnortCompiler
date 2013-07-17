#include <iostream>
#include <tchar.h>
#include <filesystem>

#include <hwprj\ctimer.h>
#include <hwprj\compiler.h>
#include <hwprj\compres.h>
#include <hwprj\trace.h>
#include <hwprj\pcreopt.h>
#include <hwprj\ruleoption.h>

bool CompareFileExt(const std::string &strFile, const std::string &strExt)
{
	if (strFile.size() <= strExt.size())
	{
		return false;
	}
	if (strFile[strFile.size() - strExt.size() - 1] != '.')
	{
		return false;
	}
	typedef std::string::const_reverse_iterator STRCRITER;
	STRCRITER rb = strFile.rbegin();
	STRCRITER re = rb + 5;
	for (STRCRITER ri = rb; ri != re; ++ri)
	{
		STRCRITER rj = strExt.rbegin() + (ri - rb);
		if (tolower(*ri) != tolower(*rj))
		{
			return false;
		}
	}
	return true;
}

void main(int nArgs, char **pArgs)
{
	CTimer t;

	if (nArgs != 2)
	{
		return;
	}

	std::string strPath = pArgs[1];
	if (strPath.back() != '\\')
	{
		strPath.push_back('\\');
	}
	// Defina a path object to express a directory
	std::tr2::sys::path rulePath(strPath);
	// Construct a directory iterator for visit this path.
	std::tr2::sys::directory_iterator iDirCur(rulePath);
	//the end iterator for this path.
	std::tr2::sys::directory_iterator iDirEnd;

	CCompileResults result;
	std::string strRuleFileExt = "rules";
	bool bCompSuc = true;
	double dTimeCnt = 0;
	for (; iDirCur != iDirEnd; ++iDirCur)
	{
		const std::tr2::sys::path &curPath = *iDirCur;
		if (!std::tr2::sys::is_directory(curPath))
		{
			std::string strFullName = rulePath.directory_string();
			strFullName.append(curPath.directory_string());
			if (CompareFileExt(strFullName, strRuleFileExt))
			{
				//当前处理的文件
				std::cout << strFullName << std::endl;
				try
				{
					t.Reset();
					//编译规则
					CompileRuleFile(strFullName.c_str(), result);
					dTimeCnt += t.Cur();
				}
				catch (CTrace &e)
				{
					std::cout << e.File() << " - " << e.Line() << ": "
						<< e.What() << std::endl;
					bCompSuc = false;
					break;
				}
			}
		}
	}

	std::cout << "规则转PCRE链用时： " << rule2pcretime << std::endl;
	std::cout << "PCRE链转nfa用时： " << pcre2nfatime << std::endl;
	std::cout << "nfa转dfa用时： " << nfa2dfatime << std::endl;
	std::cout << "dfa最小化用时： " << dfamintimetime << std::endl;

	if (bCompSuc == true)
	{
		//编译结果文件
		std::string strFileName = pArgs[1];
		strFileName += "result.cdt";

		//写编译结果文件
		result.WriteToFile(strFileName.c_str());
	}

	std::cout << "Total time: " << dTimeCnt << std::endl;
	system("pause");

	//if (nArgs != 2)
	//{
	//	return;
	//}
	//
	////std::string strFile = pArgs[1];
	////编译结果文件

	//CCompileResults result;
	////读入编译结果文件
	//int contCnt = 0;
	//int pcreCnt = 0;
	//if (0 == result.ReadFromFile("..\\rulesresult.cdt"))
	//{
	//	CSidDfaIds sidDfaIds = result.GetSidDfaIds();
	//	CRegRule regrules = result.GetRegexTbl();
	//	ulong sidSize = sidDfaIds.Size();
	//	for (size_t i = 0; i < sidSize; ++i)
	//	{
	//		struct COMPILEDINFO &curIds = sidDfaIds[i];
	//		if (curIds.m_nResult == COMPILEDINFO::RES_SUCCESS)
	//		{
	//			CUnsignedArray &curIdx = curIds.m_dfaIds;
	//			for (size_t j = 0; j < curIdx.Size(); ++j)
	//			{
	//				CPcreChain &curchains = regrules[curIdx[j]];
	//				for (size_t k = 0; k < curchains.Size(); ++k)
	//				{
	//					if (curchains[k].HasFlags(CPcreOption::PF_F))
	//					{
	//						++contCnt;
	//					}
	//					else
	//					{
	//						++pcreCnt;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//std::cout << contCnt << std::endl;
	//std::cout << pcreCnt << std::endl;

	//ulong nSuccess = 0;
	//ulong nFailure = 0;
	//ulong nExceed = 0;
	//for (ulong i = 0; i < result.GetSidDfaIds().Size(); ++i)
	//{
	//	if (result.GetSidDfaIds()[i].m_nResult == COMPILEDINFO::RES_SUCCESS)
	//	{
	//		CDfaArray dfas;
	//		for (ulong j = 0; j < result.GetSidDfaIds()[i].m_dfaIds.Size(); ++j)
	//		{
	//			dfas.PushBack(result.GetDfaTable()[result.GetSidDfaIds()[i].m_dfaIds[j]]);
	//		}
	//		CDfa mergeDfa;
	//		if (MergeMultipleDfas(dfas, mergeDfa))
	//		{
	//			++nSuccess;
	//			if (mergeDfa.Size() > 150)
	//			{
	//				++nExceed;
	//			}
	//		}
	//		else
	//		{
	//			++nFailure;
	//		}
	//	}
	//}
	//std::cout << "Success: " << nSuccess << std::endl;
	//std::cout << "Failure: " << nFailure << std::endl;
	//std::cout << "Exceed: " << nExceed << std::endl;
	//system("pause");
}