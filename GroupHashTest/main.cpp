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
	
	//�������ļ�
	std::string strReadFileName(pArgs[1]);
	strReadFileName += "result.cdt";

	CCompileResults result;
	//����������ļ�
	result.ReadFromFile(strReadFileName.c_str());

	CGroupRes groupRes;
	//����
	Grouping(result, groupRes);

	HASHRES HashResMap;
	//��ϣ
	HashMapping(groupRes, HashResMap);

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

	system("pause");
}