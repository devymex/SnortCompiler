#include <iostream>

#include <hwprj\ctimer.h>
#include <hwprj\compres.h>
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>
#include <hwprj\buildhash.h>

void main(int nArgs, char **pArgs)
{
	CTimer t;

	if (nArgs != 2)
	{
		return;
	}
	
	//编译结果文件
	std::string strReadFileName(pArgs[1]);
	strReadFileName += "result.cdt";

	CCompileResults result;
	//读入编译结果文件
	result.ReadFromFile(strReadFileName.c_str());

	CGroupRes groupRes;
	//分组
	Grouping(result, groupRes);

	HASHRES HashResMap;
	//哈希
	HashMapping(groupRes, HashResMap);

	//存储最终结果的文件
	std::string strWriteFileName(pArgs[1]);
	strWriteFileName += "FinalResult.cdt";

	//写最终结果文件
	groupRes.WriteToFile(strWriteFileName.c_str());

	//总分组数
	std::cout << groupRes.GetGroups().Size() << std::endl;
	
	//使用到的哈希槽数
	std::cout << HashResMap.size() << std::endl;

	std::cout << "Total time: " << t.Reset() << std::endl;

	system("pause");
}