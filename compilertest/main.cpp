#include <iostream>
#include <tchar.h>
#include <filesystem>
#include <sstream>
#include <numeric>
#include <algorithm>

#include <hwprj\dfa.h>
#include <hwprj\dfaarray.h>
#include <hwprj\ctimer.h>
#include <hwprj\compiler.h>
#include <hwprj\compres.h>
#include <hwprj\trace.h>

#define NumOfDelete 2500

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
	if (bCompSuc == true)
	{
		//编译结果文件
		std::string strFileName = pArgs[1];
		strFileName += "result.cdt";

		//写编译结果文件
		result.WriteToFile(strFileName.c_str());
	}

	std::cout << "Total time: " << dTimeCnt << std::endl;

	//CCompileResults result;
	//std::string strPath = pArgs[1];
	//result.ReadFromFile((strPath + "result.cdt").c_str());

	//CUnsignedArray allSigs;

	//std::ifstream fin("..\\Statistics.txt");
	//std::string str;
	//SIGNATURE oneSig;
	//while (std::getline(fin, str))
	//{
	//	std::stringstream ss(str);
	//	ss >> oneSig;
	//	allSigs.PushBack(oneSig);
	//}
	//fin.clear();
	//fin.close();

	//CUnsignedArray deletedSigs;

	//for (ulong i = 0; i < NumOfDelete; ++i)
	//{
	//	bool bDisplayable = true;
	//	for (ulong j = 0; j < 4; ++j)
	//	{
	//		if (((byte*)&allSigs[i])[j] > 127)
	//		{
	//			bDisplayable = false;
	//			break;
	//		}
	//	}
	//	if (bDisplayable)
	//	{
	//		deletedSigs.PushBack(allSigs[i]);
	//	}
	//}

	//std::ofstream fout("F:\\test.txt");
	//for (ulong i = 0; i < deletedSigs.Size(); ++i)
	//{
	//	fout << ((char*)&deletedSigs[i])[3] << ((char*)&deletedSigs[i])[2] << ((char*)&deletedSigs[i])[1] << ((char*)&deletedSigs[i])[0] << std::endl;
	//}
	//fout.clear();
	//fout.close();

	//int count = 0;
	//for (ulong i = 0; i < result.GetSidDfaIds().Size(); ++i)
	//{
	//	if (result.GetSidDfaIds()[i].m_nResult == COMPILEDINFO::RES_SUCCESS)
	//	{
	//		ulong idx = result.GetSidDfaIds()[i].m_dfaIds[0];
	//		CUnsignedArray newSigs;
	//		CUnsignedArray &oldSigs = result.GetRegexTbl()[idx].GetSigs();

	//		for (ulong j = 0; j < oldSigs.Size(); ++j)
	//		{
	//			ulong k = 0;
	//			for ( ; k < deletedSigs.Size(); ++k)
	//			{
	//				if (oldSigs[j] == deletedSigs[k])
	//				{
	//					break;
	//				}
	//			}
	//			if (k == deletedSigs.Size())
	//			{
	//				newSigs.PushBack(oldSigs[j]);
	//			}
	//		}

	//		if (newSigs.Size() != 0)
	//		{
	//			++count;
	//			CUnsignedArray &dfaIds = result.GetSidDfaIds()[i].m_dfaIds;
	//			for (ulong j = 0; j < dfaIds.Size(); ++j)
	//			{
	//				result.GetRegexTbl()[dfaIds[j]].GetSigs() = newSigs;
	//			}
	//		}
	//	}
	//}

	//std::cout << count << std::endl;

	//result.WriteToFile((strPath + "newResult.cdt").c_str());

	//CCompileResults result;
	//std::string strPath = pArgs[1];
	//result.ReadFromFile((strPath + "result.cdt").c_str());

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

	//CCompileResults result;
	//std::string strPath = pArgs[1];
	//result.ReadFromFile((strPath + "result.cdt").c_str());

	//std::ifstream fin((strPath + "allrules.rules").c_str());
	//std::vector<std::string> vecRules;
	//std::string str;
	//while (std::getline(fin, str))
	//{
	//	vecRules.push_back(str);
	//}
	//fin.clear();
	//fin.close();

	//std::ofstream fout("..//OnlyOneSig.txt");
	//for (ulong i = 0; i < result.GetSidDfaIds().Size(); ++i)
	//{
	//	if (result.GetSidDfaIds()[i].m_nResult == COMPILEDINFO::RES_SUCCESS &&
	//		result.GetRegexTbl()[result.GetSidDfaIds()[i].m_dfaIds[0]].GetSigs().Size() == 1)
	//	{
	//		std::stringstream ss;
	//		ss << "sid:";
	//		ss << result.GetSidDfaIds()[i].m_nSid;
	//		ss << ";";
	//		std::string str;
	//		ss >> str;
	//		for (ulong j = 0; j < vecRules.size(); ++j)
	//		{
	//			if (vecRules[j].find(str, 0) != std::string::npos)
	//			{
	//				fout << vecRules[j] << std::endl;
	//			}
	//		}
	//	}
	//}
	//fout.clear();
	//fout.close();

	system("pause");
}