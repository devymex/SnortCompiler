#include <iostream>
#include <filesystem>

#include <hwprj\ctimer.h>
#include <hwprj\trace.h>
#include <hwprj\compiler.h>
#include <hwprj\compres.h>
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>
#include <hwprj\buildhash.h>
#include <hwprj\logger.h>

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
	std::string strLogFile = strPath + "compile.log";
	g_log.SetLogFile(strLogFile.c_str());

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
	std::vector<std::string> allRules;
	for (; iDirCur != iDirEnd; ++iDirCur)
	{
		const std::tr2::sys::path &curPath = *iDirCur;
		if (!std::tr2::sys::is_directory(curPath))
		{
			std::string strFullName = rulePath.directory_string();
			strFullName.append(curPath.directory_string());
			if (CompareFileExt(strFullName, strRuleFileExt))
			{
				//��ǰ������ļ�
				std::cout << strFullName << std::endl;
				try
				{
					compTimer.Reset();
					//�������
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
		//�������ļ�
		std::string strFileName(strPath);
		strFileName += "result.cdt";

		//д�������ļ�
		result.WriteToFile(strFileName.c_str());
	}

	// ͳ�Ƴɹ�����Ĺ�������
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
	//����
	Grouping(result, groupRes);

	HASHRES HashResMap;
	//��ϣ
	HashMapping(groupRes, HashResMap);

	//�洢���ս�����ļ�
	std::string strWriteFileName(strPath);
	strWriteFileName += "FinalResult.cdt";

	//д���ս���ļ�
	groupRes.WriteToFile(strWriteFileName.c_str());

	std::ofstream fout("RunResult.log");

	fout << "Successed compiled rule: " << nRules << std::endl;
	fout << "Total compiled chain: " << result.GetRegexTbl().Size() << std::endl;
	fout << "Compile time: " << dCompTime << std::endl;

	//�ܷ�����
	size_t nGrounSize = groupRes.GetGroups().Size();
	fout << "Groups: " << nGrounSize << std::endl;

	fout << "Hash size: " << groupRes.GetBucketCnt() << std::endl;

	//ʹ�õ��Ĺ�ϣ����
	fout << "Used hash buckets: " << HashResMap.size() << std::endl;

	// ��ͻ��
	fout << "Conflict rate: " <<
		(nGrounSize - HashResMap.size()) / (float)nGrounSize << std::endl;	
	fout << "Group and hash time: " << compTimer.Reset() << std::endl;

	fout.close();

	system("pause");
}