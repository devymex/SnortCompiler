#include <iostream>
#include <tchar.h>
#include <filesystem>

#include <hwprj\ctimer.h>
#include <hwprj\compiler.h>
#include <hwprj\compres.h>
#include <hwprj\trace.h>

void main(int nArgs, char **pArgs)
{
	CTimer t;

	if (nArgs != 2)
	{
		return;
	}

	// Defina a path object to express a directory
	std::tr2::sys::path rulePath(pArgs[1]);
	// Construct a directory iterator for visit this path.
	std::tr2::sys::directory_iterator iDirCur(rulePath);
	//the end iterator for this path.
	std::tr2::sys::directory_iterator iDirEnd;

	CCompileResults result;
	char szExt[] = {'s', 'e', 'l', 'u', 'r'};
	for (; iDirCur != iDirEnd; ++iDirCur)
	{
		const std::tr2::sys::path &curPath = *iDirCur;
		if (!std::tr2::sys::is_directory(curPath))
		{
			std::string strFullName = rulePath.directory_string();
			strFullName.append(curPath.directory_string());
			if (strFullName.size() < 5)
			{
				continue;
			}
			std::string::reverse_iterator rb = strFullName.rbegin();
			std::string::reverse_iterator re = rb + 5;
			std::string::reverse_iterator ri = rb;
			for (; ri != re; ++ri)
			{
				if (tolower(*ri) != tolower(szExt[ri - rb]))
				{
					break;
				}
			}
			if (ri != re)
			{
				continue;
			}
			//当前处理的文件
			std::cout << strFullName << std::endl;
			try
			{
				//编译规则
				CompileRuleFile(strFullName.c_str(), result);
			}
			catch (CTrace &e)
			{
				std::cout << e.File() << " - " << e.Line() << ": " << e.What() << std::endl;
				system("pause");
			}
		}
	}
	//编译结果文件
	std::string strFileName = pArgs[1];
	strFileName += "result.cdt";

	//写编译结果文件
	result.WriteToFile(strFileName.c_str());

	std::cout << "Total time: " << t.Reset() << std::endl;

	system("pause");
}