#include "../MatchPacket/MatchPkt.h"

void MyPacketRecv(const ip_header *ih, const BYTE *data, void* pParam)
{
}
void main()
{
	LoadCapFile("c://test//firpack.cap", MyPacketRecv, NULL);
}
