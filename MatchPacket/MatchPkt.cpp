#include "stdafx.h"
#include "MatchPkt.h"

static size_t edata = 0;

void GetMchRule(std::vector<u_char> &pkt, REGRULESMAP &rulesmap, std::vector<size_t> &rules)
{
	SIGSMAP sigmap = rulesmap.sigmap;
	SIGNATURE sig;
	u_char csig[4];
	for(std::vector<u_char>::iterator iter = pkt.begin(); iter + 3 != pkt.end(); ++iter)
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

MATCHPKT void HandleAllFile(const std::string &path, REGRULESMAP &rulesmap)
{
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
				HandleAllFile(str + std::string(wfda.cFileName), rulesmap);
			}
		}
		else
		{
			std::string &temp = str + std::string(wfda.cFileName);
			std::string &ext1 = temp.substr(temp.size() - 4, 4);
			if(ext1 == ".cap")
			{
				//LoadCapFile(temp.c_str(), &rulesmap);
				std::cout << temp << std::endl;
			}
		}
	}
}

void HdlOnePkt(const u_char *data, size_t len)
{
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
				HdlOnePkt(data, tcpdatalen);
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
				HdlOnePkt(data, udpdatalen);
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

void CALLBACK MyProcess(const CSnortRule &rule, LPVOID lpVoid)
{
	REGRULESMAP &rulesmap = *(REGRULESMAP*)lpVoid;
	size_t nFlag = rule.GetFlag();

	if (rule.Size() == 0)
	{
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		return;
	}

	else
	{
		rulesmap.result.resize(rulesmap.result.size() + 1);
		rulesmap.result.back().m_nSid = rule.GetSid();
		Rule2PcreList(rule, rulesmap.result.back().regrule);
	}
}

MATCHPKT void MchCompile(LPCTSTR filename, LPVOID lpVoid)
{
	//int sids[4653];
	//std::memset(sids, 0, sizeof(sids));
	std::vector<SIGNATURE> sigvec;
	CompileRuleSet(filename, MyProcess, lpVoid);
	REGRULESMAP &rulesmap = *(REGRULESMAP*)lpVoid;
	std::vector<REGRULES>::iterator mapbegin = rulesmap.result.begin();
	for (std::vector<REGRULES>::iterator iter = mapbegin; iter != rulesmap.result.end(); ++iter)
	{
		SIGNATURE tempsig = (SIGNATURE)-1;
		size_t flag = 0;
        size_t pos = iter - mapbegin;
		for (size_t chainsize = 0; chainsize < iter->regrule.Size(); ++chainsize)
		{
			SIGNATURE sig;
			size_t sigcnt = (iter->regrule)[chainsize].GetSigCnt();
			if (sigcnt > 0)
			{
				flag |= (1 << 1);
				size_t random = rand() % sigcnt;
				tempsig = (iter->regrule)[chainsize].GetSig(random);

				for (size_t cursig = 0; cursig < sigcnt; ++cursig)
				{
					sig = (iter->regrule)[chainsize].GetSig(cursig);

					if (rulesmap.sigmap.count(sig) != 0)
					{
						continue;
					}
					else
					{
						SIGSMAP &temp = rulesmap.sigmap;
						temp[sig].resize(temp[sig].size() + 1);
						temp[sig].back() = pos;
						//sigvec.push_back(sig);
						//sids[pos] = 1;
						flag |= 1;
						goto END;
					}
				}
			}
		}
END:	if(((flag & 1) == 0) && ((flag & 1 << 1) != 0))
		{
			SIGSMAP &temp = rulesmap.sigmap;
			temp[tempsig].resize(temp[tempsig].size() + 1);
			temp[tempsig].back() = pos;
			//sigvec.push_back(tempsig);
			//sids[pos] = 1;

		}
	}
	//std::sort(sigvec.begin(), sigvec.end());
	//sigvec.erase(std::unique(sigvec.begin(),sigvec.end()), sigvec.end());

	//std::vector<size_t> sidVec;
	//for(int i = 0; i < 4653; ++i)
	//{
	//	if(0 == sids[i])
	//	{
	//		//sidVec.push_back(rulesmap.result[i].m_nSid);
	//		std::cout << rulesmap.result[i].m_nSid << std::endl;
	//	}
	//}
	size_t e0 = 0, e1 = 0, e2 = 0;
	SIGSMAP &temp = rulesmap.sigmap;
	for(SIGSMAP::iterator iter = temp.begin(); iter != temp.end(); ++ iter)
	{
		size_t size = iter->second.size();
		if (size == 0)
		{
			++e0;
		}
		else if(size == 1)
			++e1;
		else 
			++e2;
	}
	std::cout << std::endl;
}
