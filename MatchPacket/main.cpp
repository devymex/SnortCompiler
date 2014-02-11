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

std::vector<ulong> PacketHitedDfaRecord;

void main(int nArgc, char **pArgs)
{

	clock_t start, finish, duration;
	start = clock();

	//std::string pathCdtAndStt = pArgs[1];
	//if (pathCdtAndStt.back() != '\\')
	//{
	//	pathCdtAndStt.push_back('\\');
	//}

	//std::string pathCdt = pathCdtAndStt + "resultHash.cdt";
	//std::string pathStt = pathCdtAndStt + "skipTableResult.stt";

	// 读取跳转表
	//std::string pathSkipTable = pArgs[2];
	//ReadSkipTable(pathSkipTable.c_str(), skipTable);
	//ReadSkipTable(pathStt.c_str(), skipTable);

	PCREDFA pcredfa;

	DFAMCH &dfamch = pcredfa.dfamch;
	//dfamch.resultFolder = "..\\..\\output\\dmatchresult"; //一个数据包文件对应一个txt结果
	CGroupRes &groupRes = dfamch.mergedDfas;
	HASHRES &hashResMap = dfamch.hashtable;
	groupRes.ReadFromFile("D:\\test\\FinalResultUnErase0.cdt"); //输入的编译DFA
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
	//MchCompile("..\\..\\input\\CanCompile.rule", &rulesmap);
	//rulesmap.resultpath = "..\\..\\output\\pmatchresult";

	//std::string path = pArgs[2]; //输入的数据包路径 *.cap
	std::string path = "D:\\test";
	if (path.back() != '\\')
	{
		path.push_back('\\');
	}

	ulong dfaSz = groupRes.GetGroups().Size();
	PacketHitedDfaRecord.resize(dfaSz, 0);

	multiLevelHash.Init(groupRes);
	PDHandleAllFile(path, &pcredfa);

	std::cout << "Total packets: " << g_ulAllDp << std::endl;
	std::cout << "Hit DFAs: " << g_ulHashed << std::endl;

	std::cout << "Hited special transition states: " << hitedStState << std::endl;
	std::cout << "Hited total states: " << hitedDfaState << std::endl;
	std::cout << "Hit rate of special transition hitedStState / hitedDfaState: " << (double)((double)hitedStState / (double)hitedDfaState) << std::endl;


	std::ofstream os("..\\..\\PacketHitedDfaRecord_hashdeepth1.txt");

	for (int i = 0; i != dfaSz; ++i)
	{
		//os << "命中" << i << "个DFA的数据包有：" << PacketHitedDfaRecord[i] << "个" << std::endl;

		//os << i << " " << PacketHitedDfaRecord[i] << std::endl;
		os << i << std::endl;
	}

	finish = clock();
	duration = finish - start;

	std::cout << duration << std::endl;

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