#include <iostream>
#include <tchar.h>
#include <string>
#include <fstream>
//#include "../MatchPacket/DfaMatch.h"
//#include "../hashmapping/hashmapping.h"
//#include "../grouping/grouping.h"
//#include "../compilernew/compilernew.h"
//
//void main()
//{
//	DFAMCH dfamch;
//	dfamch.resultPath = "..\\..\\output\\dmatchresult.txt";
//
//
//	CGROUPRes &groupRes = dfamch.mergedDfas;
//
//	HASHRES &HashResMap = dfamch.hashtable;
//	groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
//
//	HashMapping(groupRes, HashResMap);
//
//	
//		std::cout << "µÚ " << 817 << " ×é£º";
//		for(size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
//		{
//			if(groupRes.GetGroups()[i].mergeDfaId == 817)
//			{
//				for(size_t j = 0; j < groupRes.GetGroups()[i].DfaIds.Size(); ++j)
//				{
//					std::cout << groupRes.GetGroups()[i].DfaIds[j] << "  ";
//				}
//			}
//		}
//
//	DfaidSidMap(groupRes, dfamch.dIdSId);
//	std::string path = "E:\\allPkt";
//
//
//	DHandleAllFile(path, &dfamch);
//
//	system("pause");
//}

#include "../MatchPacket/MatchPkt.h"
#include "../common/common.h"
#include <fstream>
#include <tchar.h>

void main()
{
	REGRULESMAP rulesmap;
	MchCompile(_T("../../input/allrules.rule"), &rulesmap);

	rulesmap.resultpath = "E:\\dataPacket_huawei\\matchresult";
	HandleAllFile("E:\\dataPacket_huawei\\06", &rulesmap);

}