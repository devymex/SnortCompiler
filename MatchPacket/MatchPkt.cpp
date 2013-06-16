#include "stdafx.h"
#include "MatchPkt.h"

static size_t pktnum = 0;

void GetMchRule(const u_char *data, size_t len, void* user, std::vector<size_t> &rules)
{
	if(len > 3)
	{
		REGRULESMAP &rulesmap = *(REGRULESMAP *)user;
		SIGSMAP &sigmap = rulesmap.sigmap;
		SIGNATURE sig;
		u_char csig[4];
		size_t flag = 0;

		for(const u_char* iter = data; iter != &data[len - 4]; ++iter)
		{
			for(size_t i = 0; i < 4; ++i)
			{
				csig[i] = tolower(*(iter + i));
			}
			sig = *(SIGNATURE *)csig;
			if(sigmap.count(sig))
			{
				rules.insert(rules.end(), sigmap[sig].begin(), sigmap[sig].end());
			}
		}
	}
	std::sort(rules.begin(), rules.end());
	rules.erase(std::unique(rules.begin(), rules.end()), rules.end());
}

//调用pcre库进行数据包匹配
bool MyPcreMatch(const u_char *data, size_t len, CRegRule &regRule)
{
	for(size_t i = 0; i < regRule.Size(); ++i)
	{
		//从数据包头开始匹配
		const u_char *pData = data;
		size_t dataSize = len;
		for(size_t j = 0; j < regRule[i].Size(); ++j)
		{
			//对规则选项进行匹配
			int Pos = -1;

			bool flag = PcreMatch((const char*)pData, dataSize, regRule[i][j].GetStr(), Pos);
			if(!flag)
			{
				return false;
			}
			else if(Pos < dataSize)
			{
				pData += Pos;
				dataSize -= Pos;
			}
			else
			{
				pData = null;
				dataSize = 0;
			}
		}
	}
	return true;
}

void HdlOnePkt(const u_char *data, size_t len, void*user)
{
	//const u_char p[] = {0, 0, 'f', 'g', 0, 'a', 'b', 'C', 'd', 0, 'd', 'e', 235, 0, 42, 'A', 123, 'B', 40, '1', '2', 93, 63, 'a', 'b', 'c', 'd', 'e', 'a'};
	//data = p;
	//len = sizeof(p);
	REGRULESMAP &rulesmap = *(REGRULESMAP *)user;
	std::vector<size_t> rules;
	GetMchRule(data, len, user, rules);
	std::vector<size_t> matchvec;
	rulesmap.mchresult << pktnum << " : ";
	for(size_t i = 0; i < rules.size(); ++i)
	{
		bool flag = MyPcreMatch(data, len, rulesmap.result[rules[i]].regrule);
		if(flag)
		{
			matchvec.push_back(rulesmap.result[rules[i]].m_nSid);
			//rulesmap.mchresult << rulesmap.result[rules[i]].m_nSid << "  ";
		}
	}
	std::sort(matchvec.begin(), matchvec.end());
	for(std::vector<size_t>::iterator iter = matchvec.begin(); iter != matchvec.end(); ++iter)
	{
		rulesmap.mchresult << *iter << "  ";
	}
	rulesmap.mchresult << std::endl;
}

MATCHPKT void HandleAllFile(const std::string &path, void* user)
{
	REGRULESMAP &rulesmap = *(REGRULESMAP *)user;
	WIN32_FIND_DATAA wfda;
	const std::string ext = "*.*";
	std::string str = path + std::string("\\");
	std::string pat = str + ext;
	HANDLE hff = ::FindFirstFileA(pat.c_str(), &wfda);
	if(hff == INVALID_HANDLE_VALUE)
	{
		return;
	}

	for(BOOL br = TRUE; br == TRUE; br = FindNextFileA(hff, &wfda))
	{
		if(wfda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if (wfda.cFileName[0] != '.')
			{
				HandleAllFile(str + std::string(wfda.cFileName), user);
			}
		}
		else
		{
			std::string &temp = str + std::string(wfda.cFileName);
			std::string &ext1 = temp.substr(temp.size() - 4, 4);

			if(ext1 == ".cap")
			{
			std::string str = rulesmap.resultpath + "\\" +std::string(wfda.cFileName) +".txt";
			rulesmap.mchresult.open(rulesmap.resultpath + "\\" +std::string(wfda.cFileName) +".txt");	
			rulesmap.mchresult << "-----------------------" << temp << "-----------------------" << std::endl;
			LoadCapFile(temp.c_str(), &rulesmap);
			rulesmap.mchresult.close();
			}

			pktnum = 0;
		}
	}
}

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
	++pktnum;	
	REGRULESMAP &rulesmap = *(REGRULESMAP *)user;
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
				HdlOnePkt(data, tcpdatalen, user);
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
				HdlOnePkt(data, udpdatalen, user);
			}
			break;
		}
	}
	std::cout << pktnum << std::endl;
}

bool MyLoadCapFile(const char* pFile, PACKETRECV cv, void* pUser)
{
	//char* ebuff = new char;
	pcap_t *mypcap = pcap_open_offline(pFile, NULL);
	PACKETPARAM pp;
	pp.pUser = pUser;
	pp.pFunc = cv;

	pcap_loop(mypcap, 0, packet_handler, (BYTE*)&pp);
	pcap_close(mypcap);
	//delete(ebuff);
	return true;
}

MATCHPKT bool LoadCapFile(const char* pFile, void* pUser)
{
	return MyLoadCapFile(pFile, PktParam, pUser);
}

