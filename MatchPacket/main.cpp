#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "../MatchPacket/DfaMatch.h"

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


	CGROUPRes &groupRes = dfamch.mergedDfas;

	HASHRES &hashResMap = dfamch.hashtable;
	groupRes.ReadFromFile("..\\..\\output\\FinalResut.cdt");

	for(size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		hashResMap[hash(groupRes.GetGroups()[i].currSig)].push_back(HASHNODE(groupRes.GetGroups()[i].currSig, groupRes.GetGroups()[i].mergeDfaId));
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

	std::string path = "E:\\allPkt";

	DHandleAllFile(path, &dfamch);

	system("pause");
}



//#include "../MatchPacket/MatchPkt.h"
//#include "../common/common.h"
//#include <fstream>
//#include <tchar.h>
//
//void main()
//{
//	REGRULESMAP rulesmap;
//	MchCompile(_T("..\\..\\input\\ProcessRules.rule"), &rulesmap);
//
//	rulesmap.resultpath = "..\\..\\output\\pmatchresult";
//	HandleAllFile("E:\\allPkt", &rulesmap);
//}