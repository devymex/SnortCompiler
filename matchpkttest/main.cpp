#include "../MatchPacket/MatchPkt.h"


void main()
{
	std::vector<std::vector<u_char>> allPkt;
	LoadCapFile("c://test//firpack.cap", &allPkt);
}
