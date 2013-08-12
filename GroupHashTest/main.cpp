#include <iostream>
#include <filesystem>

#include <hwprj\ctimer.h>
#include <hwprj\trace.h>
#include <hwprj\compiler.h>
#include <hwprj\compres.h>
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>
#include <hwprj\buildhash.h>

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
	CTimer compTimer;
	double dCompTime = 0;
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
					compTimer.Reset();
					//编译规则
					CompileRuleFile(strFullName.c_str(), result);
					dCompTime += compTimer.Cur();
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
		std::string strFileName(strPath);
		strFileName += "result.cdt";

		//写编译结果文件
		result.WriteToFile(strFileName.c_str());
	}

	// 统计成功编译的规则数量
	size_t nRules = 0;
	for (size_t i = 0; i < result.GetSidDfaIds().Size(); ++i)
	{
		if (result.GetSidDfaIds()[i].m_nResult == COMPILEDINFO::RES_SUCCESS)
		{
			++nRules;
		}
	}

	compTimer.Reset();
	CGroupRes groupRes;
	//分组
	Grouping(result, groupRes);

	HASHRES HashResMap;
	//哈希
	HashMapping(groupRes, HashResMap);

	//存储最终结果的文件
	std::string strWriteFileName(strPath);
	strWriteFileName += "FinalResult.cdt";

	//写最终结果文件
	groupRes.WriteToFile(strWriteFileName.c_str());

	std::cout << std::endl << std::endl;
	std::cout << "Successed compiled rule: " << nRules << std::endl;
	std::cout << "Total compiled chain: " << result.GetRegexTbl().Size() << std::endl;
	std::cout << "Compile time: " << dCompTime << std::endl;

	//总分组数
	size_t nGrounSize = groupRes.GetGroups().Size();
	std::cout << "Groups: " << nGrounSize << std::endl;

	std::cout << "Hash size: " << groupRes.GetBucketCnt() << std::endl;

	//使用到的哈希槽数
	std::cout << "Used hash buckets: " << HashResMap.size() << std::endl;

	// 冲突率
	std::cout << "Conflict rate: " <<
		(nGrounSize - HashResMap.size()) / (float)nGrounSize << std::endl;	
	std::cout << "Group and hash time: " << compTimer.Reset() << std::endl;

	std::cout << std::endl;

	system("pause");
}