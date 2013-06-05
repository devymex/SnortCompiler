#include "stdafx.h"
#include "MatchPkt.h"

static size_t edata = 0;

void GetMchRule(const u_char *data, size_t len, void* user, std::vector<size_t> &rules)
{
	REGRULESMAP &rulesmap = *(REGRULESMAP *)user;
	SIGSMAP sigmap = rulesmap.sigmap;
	SIGNATURE sig;
	u_char csig[4];
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

void HdlOnePkt(const u_char *data, size_t len, void*user)
{
	REGRULESMAP &rulesmap = *(REGRULESMAP *)user;
	std::vector<size_t> rules;
	GetMchRule(data, len, user, rules);
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
				LoadCapFile(temp.c_str(), &rulesmap);
				std::cout << temp << std::endl;
			}
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
			else
			{
				++edata;
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
			else
			{
				++edata;
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

//���� 0 ��ʾû��ƥ���ϣ����� 1 ��ʾƥ����
size_t MatchOnedfa(std::vector<u_char> &onepkt, CDfaNew &dfa, std::vector<size_t> &matchedDids)
{
	std::unordered_map<size_t, std::vector<size_t>> dfaids;
	for (size_t i = 0; i < dfa.GetTermCnt(); ++i)
	{
		TERMSET &term = dfa.GetTerm(i);
		if (dfaids.count(term.dfaSta))
		{
			dfaids[term.dfaSta].push_back(term.dfaId);
		}
		else
		{
			dfaids[term.dfaSta].resize(1);
			dfaids[term.dfaSta].push_back(term.dfaId);
		}
	}

	STATEID curSta = dfa.GetStartId();
	for (std::vector<u_char>::iterator edgeiter = onepkt.begin(); edgeiter != onepkt.end(); ++edgeiter)
	{
		BYTE group = dfa.GetOneGroup(*edgeiter);

		if (0 == (dfa[curSta].GetFlag() & CDfaRow::TERMINAL))
		{
			if (dfa[curSta][group] != -1)
			{
				curSta = dfa[curSta][group];
			}
			else
			{
				return 0;
			}
		}
		else
		{

		}
	}
}

MATCHPKT void MatchPkt(std::vector<std::vector<u_char>> &allPkt, std::map<size_t, std::vector<SIGSID>> &hashtable, std::vector<CDfaNew> &alldfas, std::vector<MATCHRESULT> &matchresult)
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

