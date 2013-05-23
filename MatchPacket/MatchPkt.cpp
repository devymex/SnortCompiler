#include "stdafx.h"
#include "MatchPkt.h"

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	ip_header *ih;
	ih = (ip_header *)(pkt_data + ETHDRLEN);
	u_char pro = ih->proto;
	pkt_data = pkt_data + ETHDRLEN;
	PACKETPARAM *pParam = (PACKETPARAM*)param;
	pParam->pFunc(ih, pkt_data, pParam->pUser);
}

MATCHPKT bool LoadCapFile(const char* pFile, PACKETRECV cv, void* pUser)
{
	char* ebuff = new char;
	pcap_t *mypcap = pcap_open_offline(pFile, ebuff);
	PACKETPARAM pp;
	pp.pUser = pUser;
	pp.pFunc = cv;
	pcap_loop(mypcap, 0, packet_handler, (BYTE*)&pp);

	return true;
}