#include <iostream>
#include <tchar.h>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <string>

#include <hwprj\pcreopt.h>
#include <hwprj\dllstring.h>
#include <hwprj\byteary.h>
#include <hwprj\ctimer.h>
#include <hwprj\compiler.h>
#include <hwprj\compres.h>
#include <hwprj\trace.h>
#include <hwprj\dfaarray.h>


void main(int nArgs, char **pArgs)
{
	std::string strSource;
	NODE node;
	CUnsignedArray sigs;
	std::vector<CByteArray> strResult;
	// 检测文件
	std::ifstream ifile("D:\\projects\\input\\CanProcessPcres.txt");
	
	std::getline(ifile, strSource);

	size_t pos1 = strSource.find_last_of('"');
	size_t pos2 = 0;
	for (pos2 = pos1; pos2 >= 0; --pos2)
	{
		if (strSource[pos2] == '/')
		{
			break;
		}
	}

	std::string realStr(strSource);
	if (pos1 > pos2 + 1)
	{
		realStr.erase(realStr.begin() + pos2 + 1, realStr.begin() + pos1);
	}

	InitNode(realStr, node);
	sigs = ExtrSig(node);

	// 打印结果
	for (ulong idx = 0; idx != sigs.Size(); ++idx)
	{
		std::cout << sigs[idx] << std::endl;
	}
	std::cout << sigs.Size() << std::endl;

	system("pause");
}