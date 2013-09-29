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
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>
#include <hwprj\ctimer.h>
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>
#include <hwprj\buildhash.h>
#include <hwprj\logger.h>

#define RATIO 0.155

// ����EraseSig����
void EraseSig(CUnsignedArray &ChainSigs, const std::vector<SIGNATURE> &vecNeedErase)
{
	bool flag = false;
	for (ulong idxSig = 0; idxSig != ChainSigs.Size(); ++idxSig)
	{
		if (std::find(vecNeedErase.begin(), vecNeedErase.end(), ChainSigs[idxSig]) == vecNeedErase.end())
		{
			flag = true;   
			break;
		}
	}
	if (flag)      
	{
		CUnsignedArray newChainSigs;
		for (ulong idxSig = 0; idxSig != ChainSigs.Size(); ++idxSig)
		{
			if (std::find(vecNeedErase.begin(), vecNeedErase.end(), ChainSigs[idxSig]) == vecNeedErase.end())
			{
				newChainSigs.PushBack(ChainSigs[idxSig]);   //����ƥ��ļ���
			}
		}
		ChainSigs = newChainSigs;
	}
}



void main(int nArgs, char **pArgs)
{
	// ��������result.cdt
	std::string resultPath = pArgs[1];

	// Defina a path object to express a directory
	std::tr2::sys::path rulePath( pArgs[2]);
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
			try
			{
				CompileRuleFile(strFullName.c_str(), result);
			}
			catch (CTrace &e)
			{
				std::cout << e.File() << " - " << e.Line() << ": " << e.What() << std::endl;
				system("pause");
			}
		}
	}
	std::string originReslutPath = resultPath + "orignResult.cdt";
	result.WriteToFile(originReslutPath.c_str());



	// ɾ������
	CRegRule &Tabl_3 = result.GetRegexTbl();
	ulong nSize = Tabl_3.Size();

	std::vector<SIGNATURE> vecNeedEraseSigs;
	std::string pathSatic = pArgs[3];
	pathSatic += "Statistics.txt";
	std::ifstream ifile(pathSatic);
	ulong curSig;
	std::string str;
	std::ofstream ofile("1.txt");
	while (std::getline(ifile, str))
	{

		std::stringstream ss(str);
		ss >> curSig;
		vecNeedEraseSigs.push_back(curSig);
		ofile << curSig << std::endl;
	}
	ofile.close();
	ifile.close();

	// ȥ���ƶ�signature
	int totalNum = vecNeedEraseSigs.size();
	int ratErase = totalNum * RATIO;
	vecNeedEraseSigs.resize(ratErase);
	for (ulong nIdx = 0; nIdx != Tabl_3.Size(); ++nIdx)                //����������������Table_3
	{
		CUnsignedArray &Signatures = Tabl_3[nIdx].GetSigs();           //�����õ���ĳ������SIG
		EraseSig(Signatures, vecNeedEraseSigs);
	}
	//д���ļ�
	std::string resultErasePath = resultPath + "resultErase.cdt";
	result.WriteToFile(resultErasePath.c_str());

	CGroupRes groupRes;
	// ����
	Grouping(result, groupRes);

	HASHRES HashResMap;
	//��ϣ
	HashMapping(groupRes, HashResMap);

	std::string resultFinalPath = resultPath + "resultHash.cdt";
	groupRes.WriteToFile(resultFinalPath.c_str());

	std::string resultStic = resultPath + "resutlStic.txt";
	std::ofstream fout(resultStic);

	//�ܷ�����
	size_t nGrounSize = groupRes.GetGroups().Size();
	fout << "Groups: " << nGrounSize << std::endl;

	// ��ͻ��
	fout << "Conflict rate: " << (nGrounSize - HashResMap.size()) / (float)nGrounSize << std::endl;	

	fout.close();

	system("pause");
}

