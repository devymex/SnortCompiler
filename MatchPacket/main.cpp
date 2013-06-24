#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "DfaMatch.h"

//
//struct GROUPNUM
//{
//	CSIGNATURES ComSigs;
//	std::size_t mergeDfaId;
//};

void main()
{
	PCREDFA pcredfa;

	DFAMCH &dfamch = pcredfa.dfamch;
	dfamch.resultFolder = "..\\..\\output\\dmatchresult";
	CGroupRes &groupRes = dfamch.mergedDfas;
	HASHRES &hashResMap = dfamch.hashtable;
	groupRes.ReadFromFile("..\\..\\output\\FinalResult.cdt");
	hash.nBucketCnt = groupRes.GetBucketCnt();
	for(size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		HASHNODE hashnode;
		hashnode.m_nDfaId = groupRes.GetGroups()[i].mergeDfaId;
		hashnode.m_sig = groupRes.GetGroups()[i].currSig;
		hashResMap[hash(groupRes.GetGroups()[i].currSig)].push_back(hashnode);
	}

	DfaidSidMap(groupRes, dfamch.dIdSId);
	REGRULESMAP &rulesmap = pcredfa.rulesmap;
	MchCompile("..\\..\\input\\CanCompile.rule", &rulesmap);
	rulesmap.resultpath = "..\\..\\output\\pmatchresult";

	std::string path = "E:\\allPkt\\temp";
	PDHandleAllFile(path, &pcredfa);

	system("pause");
}



//#include "stdafx.h"
//#include "MatchPkt.h"
//
//#include <fstream>
//#include <tchar.h>
//
//void main()
//{
//	REGRULESMAP rulesmap;
//	MchCompile("..\\..\\input\\CanCompile.rule", &rulesmap);
//
//	rulesmap.resultpath = "..\\..\\output\\pmatchresult";
//	HandleAllFile("E:\\allPkt", &rulesmap);
//}

//#include "stdafx.h"
//#include "DfaMatch.h"
//
//void main()
//{
//	std::string dpath1 = "D:\\projects\\output\\backup\\d",
//		dpath2 = "D:\\projects\\output\\dmatchresult";
//
//	std::vector<std::string> result1, result2;
//	result1.reserve(400);
//	result2.reserve(400);
//
//	ResultFiles(dpath1, result1);
//	ResultFiles(dpath2, result2);
//
//	size_t max = result1.size();
//	if (result2.size() > max)
//	{
//		max = result2.size();
//	}
//
//	std::ofstream ofs;
//	ofs.open("D:\\projects\\output\\compare.txt");
//
//	for(size_t i = 0; i < max; ++i)
//	{
//		std::ifstream ifs1(result1[i]);
//		std::ifstream ifs2(result2[i]);
//
//		std::string tem1, tem2, firline1, firline2;
//		std::getline(ifs1, firline1);
//		std::getline(ifs2, firline2);
//
//		ofs << firline1 << std::endl;
//
//		while (std::getline(ifs1, tem1), std::getline(ifs2, tem2))
//		{
//			if( tem1.compare(tem2) != 0)
//			{
//				ofs << "different:  " << tem1 << std::endl << tem2 << std::endl;
//			}
//		}
//
//
//	}
//	std::cout << std::endl;
//}