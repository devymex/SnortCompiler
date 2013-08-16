#include <iostream>
#include <tchar.h>
#include <filesystem>
#include <algorithm>
#include <sstream>

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
	std::ifstream ifile("pcreSource.txt");
	
	std::getline(ifile, strSource);
	InitNode(strSource, node);
	sigs = ExtrSig(node);

	// 打印结果
	for (ulong idx = 0; idx != sigs.Size(); ++idx)
	{
		std::cout << sigs[idx] << std::endl;
	}
	std::cout << sigs.Size() << std::endl;

	system("pause");
}