#include "../MatchPacket/MatchPkt.h"
#include "../common/common.h"
#include <fstream>
#include <tchar.h>


void main()
{
	REGRULESMAP rulesmap;
	rulesmap.mchresult.open("E:\\test\\matchresult.txt");
	MchCompile(_T("../../input/allrules.rule"), &rulesmap);
	HandleAllFile("E:\\test", rulesmap);
	rulesmap.mchresult.close();
}
