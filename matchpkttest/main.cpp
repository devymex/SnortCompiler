#include "../MatchPacket/MatchPkt.h"
#include "../common/common.h"
#include <fstream>
#include <tchar.h>


void main()
{
	REGRULESMAP rulesmap;
	rulesmap.mchresult.open(_T("../../output/matchresult.txt"));
	MchCompile(_T("../../input/test.rules"), &rulesmap);
	HandleAllFile("F:\\cppProject\\huawei\\PreciseMatch\\input", &rulesmap);
	rulesmap.mchresult.close();
}
