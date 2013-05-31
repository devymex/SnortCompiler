#include "stdafx.h"
#include "MatchPkt.h"

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	ip_header *ih;
	ih = (ip_header *)(pkt_data + ETHDRLEN);
	pkt_data = pkt_data + ETHDRLEN;
	PACKETPARAM *pParam = (PACKETPARAM*)param;
	pParam->pFunc(ih, pkt_data, pParam->pUser);
}

void CALLBACK PktParam(const ip_header *ih, const BYTE *data, void* user)
{
	std::vector<std::vector<u_char>> &allPkt = *(std::vector<std::vector<u_char>> *)user;
	u_short  _ihl = (ih->ver_ihl & 0x0f) * 4;
	
	u_short _tlen = ih->tlen;
	std::swap(((BYTE*)&_tlen)[0], ((BYTE*)&_tlen)[1]);

	u_char  _proto = ih->proto;

	tcp_header *ptcp = (tcp_header *)data;
	udp_header *pudp = (udp_header *)data;

	switch (_proto)
	{
	case _TCP:
		{
			ptcp = (tcp_header*)((BYTE*) ptcp + _ihl);
			u_short tcpHdrLen = ((ptcp->lenres & 0xf0) >> 4) * 4;
			data += _ihl + tcpHdrLen;

			size_t tcpdatalen = _tlen - _ihl - tcpHdrLen;
			if(tcpdatalen > 0)
			{
				allPkt.resize(allPkt.size() + 1);
				allPkt.back().insert(allPkt.back().begin(), data, data + tcpdatalen);
			}
			break;
		}

	case _UDP:
		{
			pudp = (udp_header*)((BYTE*) pudp + _ihl);
			data += _ihl + UDPHDRLEN;

			size_t udpdatalen = _tlen - _ihl - UDPHDRLEN;
			if(udpdatalen > 0)
			{
				allPkt.resize(allPkt.size() + 1);
				allPkt.back().insert(allPkt.back().begin(), data, data + udpdatalen);
			}
			break;
		}
	}
}

bool MyLoadCapFile(const char* pFile, PACKETRECV cv, void* pUser)
{
	char* ebuff = new char;
	pcap_t *mypcap = pcap_open_offline(pFile, ebuff);
	PACKETPARAM pp;
	pp.pUser = pUser;
	pp.pFunc = cv;
	pcap_loop(mypcap, 0, packet_handler, (BYTE*)&pp);

	return true;
}

MATCHPKT bool LoadCapFile(const char* pFile, void* pUser)
{
	return MyLoadCapFile(pFile, PktParam, pUser);
}

void FindSig(size_t sig, std::map<size_t, std::vector<SIGSID>> &hashtable, std::vector<size_t> &matchSids)
{
	if (hashtable.count(sig))
	{
		std::vector<SIGSID> &onevec = hashtable[sig];
		if (!onevec.empty())
		{
			for(std::vector<SIGSID>::iterator iter = onevec.begin(); iter != onevec.end(); ++iter)
			{
				if (sig == iter->sig)
				{
					matchSids.resize(matchSids.size() + 1);
					matchSids.back() = iter->sig;
				}
			}
		}
	}
}

void MatchOnedfa(std::vector<u_char> &onepkt, CDfanew &dfa, std::vector<size_t> &matchedSids)
{

}

MATCHPKT void MatchPkt(std::vector<std::vector<u_char>> &allPkt, std::map<size_t, std::vector<SIGSID>> &hashtable, std::vector<CDfanew> &alldfas, std::vector<MATCHRESULT> &matchresult)
{
	for (std::vector<std::vector<u_char>>::iterator allPktIter = allPkt.begin(); allPktIter != allPkt.end(); ++allPktIter)
	{
		SIGNATURE onesig = -1;
		std::vector<size_t> matchDids;
		std::vector<size_t> matchedSids;

		for (std::vector<u_char>::iterator pktIter = allPktIter->begin(); pktIter + 3 != allPktIter->end(); ++pktIter)
		{
			onesig = *(SIGNATURE *)&(*pktIter);
			FindSig(HashFcn(onesig), hashtable, matchDids);
		}

		if (!matchDids.empty())
		{
			for(std::vector<size_t>::iterator idIter = matchDids.begin(); idIter != matchDids.end(); ++idIter)
			{
				MatchOnedfa(*allPktIter, alldfas[*idIter], matchedSids);
			}
		}

		if(!matchedSids.empty())
		{
			matchresult.resize(matchresult.size() + 1);
			matchresult.back().pktnum = allPktIter - allPkt.begin();
			matchresult.back().matchedSids = matchedSids;
		}
	}
}