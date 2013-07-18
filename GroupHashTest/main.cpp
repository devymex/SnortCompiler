#include <iostream>

#include <hwprj\ctimer.h>
#include <hwprj\compres.h>
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>
#include <hwprj\buildhash.h>
#include <hwprj\dfaarray.h>

#include <sstream>

void main(int nArgs, char **pArgs)
{

	if (nArgs != 2)
	{
		return;
	}
	
	std::string strFile = pArgs[1];
	//编译结果文件

	CCompileResults result;
	//读入编译结果文件
	if (0 == result.ReadFromFile(strFile.c_str()))
	{
		CTimer t;
		CGroupRes groupRes;
		//分组
		Grouping(result, groupRes);

		HASHRES HashResMap;
		//哈希
		HashMapping(groupRes, HashResMap);

		//std::stringstream ss;

		//for(size_t i = 0; i < groupRes.GetDfaTable().Size(); ++i)
		//{
		//	std::string fileName = "F:\\cppProject\\huawei\\PreciseMatch\\CompressTest\\retDfa\\";
		//	ss.clear();
		//	ss.str("");
		//	ss << i;
		//	fileName += ss.str() + ".txt";
		//	groupRes.GetDfaTable()[i].Dump(fileName.c_str());
		//}


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
	}

	system("pause");
}