#include "../MatchPacket/MatchPkt.h"
#include <fstream>


void main()
{
	std::ofstream matchresult("E:\\test\\matchresult.txt");
	LoadCapFile("c://test//firpack.cap", &matchresult);
}
