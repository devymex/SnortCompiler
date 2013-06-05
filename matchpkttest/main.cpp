#include "../MatchPacket/MatchPkt.h"
#include "../common/common.h"
#include <fstream>
#include <tchar.h>


void main()
{
	REGRULESMAP rulesmap;
	rulesmap.mchresult.open(_T("../../output/matchresult.txt"));
	MchCompile(_T("../../input/allrules.rule"), &rulesmap);
	HandleAllFile("E:\\dataPacket_huawei", &rulesmap);
	rulesmap.mchresult.close();
}
