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
	DFAMCH dfamch;
	dfamch.resultPath = "..\\..\\output\\dmatchresult";


	CGroupRes &groupRes = dfamch.mergedDfas;

	HASHRES &hashResMap = dfamch.hashtable;
	groupRes.ReadFromFile("..\\..\\output\\FinalResult.cdt");

	for(size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		HASHNODE hashnode;
		hashnode.m_nDfaId = groupRes.GetGroups()[i].mergeDfaId;
		hashnode.m_sig = groupRes.GetGroups()[i].currSig;
		hashResMap[hash(groupRes.GetGroups()[i].currSig)].push_back(hashnode);
	}

	DfaidSidMap(groupRes, dfamch.dIdSId);


	/////////////////////////////////////////////////////////////////////
	/*for(size_t i = 0; i < groupRes.GetSidDfaIds().Size(); ++ i)
	{
	if(groupRes.GetSidDfaIds()[i].m_nSid == 467)
	{
	for(size_t j = 0; j < groupRes.GetSidDfaIds()[i].m_dfaIds.Size(); ++j)
	{
	std::cout << groupRes.GetSidDfaIds()[i].m_dfaIds[j] << "  ";
	}
	}
	}




	std::vector<size_t> cvec;
	std::unordered_map<size_t, GROUPNUM> groupresult;
	for(size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
	if(groupRes.GetGroups()[i].mergeDfaId == 817)
	{
	for(size_t n = 0; n < groupRes.GetGroups()[i].DfaIds.Size(); ++n)
	{
	cvec.push_back(groupRes.GetGroups()[i].DfaIds[n]);
	}
	}
	for(size_t j = 0; j < groupRes.GetGroups()[i].DfaIds.Size(); ++j)
	{
	groupresult[groupRes.GetGroups()[i].DfaIds[j]].ComSigs = groupRes.GetGroups()[i].ComSigs;
	groupresult[groupRes.GetGroups()[i].DfaIds[j]].mergeDfaId = groupRes.GetGroups()[i].mergeDfaId;
	}
	}

	std::vector<size_t> rvec;
	for(std::vector<size_t>::iterator iter = cvec.begin(); iter != cvec.end(); ++iter)
	{
	rvec.push_back(dfamch.dIdSId.dId_sId[*iter]);
	}
	std::sort(rvec.begin(), rvec.end());
	rvec.erase(std::unique(rvec.begin(), rvec.end()), rvec.end());

	GROUPNUM &tmp = groupresult[1243];
	char a[4] = {0,0,0,0};
	SIGNATURE sig = *(SIGNATURE *)a;*/



	//////////////////////////////////////////////////////////////////////////////

	//	for(size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	//{
	//	if(groupRes.GetGroups()[i].mergeDfaId == 0)
	//	{
	//		size_t n =  groupRes.GetGroups()[i].DfaIds.Size();
	//		for(size_t j = 0; j < n; ++j)
	//		{
	//			std::cout << groupRes.GetGroups()[i].DfaIds[j] << " ";
	//		}
	//	}

	//}

	//CDfa dfanew= groupRes.GetDfaTable()[0];
	//dfanew.Dump("..\\..\\output\\dfanew.txt");

	std::string path = "E:\\allPkt\\02";

	DHandleAllFile(path, &dfamch);

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
//
//#include "stdafx.h"
//#include "DfaMatch.h"
//
//void main()
//{
//	std::string dpath1 = "D:\\projects\\output\\pmatchresult",
//
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
//	if (result2.size() < max)
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
//				ofs << tem1 << std::endl << tem2 << std::endl << std::endl;
//			}
//		}
//
//	}
//	std::cout << std::endl;
//}