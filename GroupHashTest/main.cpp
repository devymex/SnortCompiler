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
	//�������ļ�

	CCompileResults result;
	//����������ļ�
	if (0 == result.ReadFromFile(strFile.c_str()))
	{
		CTimer t;
		CGroupRes groupRes;
		//����
		Grouping(result, groupRes);

		HASHRES HashResMap;
		//��ϣ
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


		//�洢���ս�����ļ�
		std::string strWriteFileName(pArgs[1]);
		strWriteFileName += "FinalResult.cdt";

		//д���ս���ļ�
		groupRes.WriteToFile(strWriteFileName.c_str());

		//�ܷ�����
		std::cout << groupRes.GetGroups().Size() << std::endl;
	
		//ʹ�õ��Ĺ�ϣ����
		std::cout << HashResMap.size() << std::endl;

		std::cout << "Total time: " << t.Reset() << std::endl;
	}

	system("pause");
}