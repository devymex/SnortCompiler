#include "stdafx.h"
#include "../compilernew/compilernew.h"
#include "DfaMatch.h"
#include "../grouping/grouping.h"

static size_t dpktnum = 0;

void MatchOnedfa(const u_char * &data, size_t len, CDfaNew &dfa,
				 std::vector<size_t> &matchedDids)
{
	if(dpktnum == 77)
	{
		std::unordered_map<size_t, std::set<size_t>> dfaids;
		const CFinalStates &finStas = dfa.GetFinalState();
		for (size_t i = 0; i < finStas.Size(); ++i)
		{
			STATEID nStaId = finStas[i];
			dfaids[nStaId] = finStas._GetDfaIds(nStaId);
		}

		STATEID curSta = dfa.GetStartId();
		for (size_t edgeiter = 0; edgeiter != len; ++edgeiter)
		{
			BYTE group = dfa.GetGroup(data[edgeiter]);

			if (0 == (dfa[curSta].GetFlag() & CDfaRow::TERMINAL))
			{
				if (dfa[curSta][group] != (STATEID)-1)
				{
					curSta = dfa[curSta][group];
				}
				else
				{
					return;
				}
			}
			else
			{
				matchedDids.insert(matchedDids.end(), dfaids[curSta].begin(), dfaids[curSta].end());
				if(dfa[curSta][group] != (STATEID)-1)
				{
					curSta = dfa[curSta][group];
				}
				else 
				{
					return;
				}
			}
		}
	}
}

void MchDfaHdler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	ip_header *ih;
	ih = (ip_header *)(pkt_data + ETHDRLEN);
	pkt_data = pkt_data + ETHDRLEN;
	PACKETPARAM *pParam = (PACKETPARAM*)param;
	pParam->pFunc(ih, pkt_data, pParam->pUser);
}

void GetMchDfas(const u_char *data, size_t len, HASHRES &hashtable, std::vector<size_t> &matchdfas)
{
	if (len > 3)
	{
		SIGNATURE sig;
		u_char csig[4];
		size_t flag = 0;

		for (const u_char* iter = data; iter != &data[len - 4]; ++iter)
		{
			for (size_t i = 0; i < 4; ++i)
			{
				csig[i] = tolower(*(iter + i));
			}
			sig = *(SIGNATURE *)csig;

			if(sig == 0)
			{
				size_t hashsig = hash(sig);
				if (hashtable.count(hashsig))
				{
					for (std::vector<HASHNODE>::iterator iter = hashtable[hashsig].begin(); iter != hashtable[hashsig].end(); ++iter)
					{
						if (sig == iter->m_sig)
						{
							matchdfas.push_back(iter->m_nDfaId);
						}
					}
				}

				flag = 1;
			}

			if(flag == 0)
			{
				size_t hashsig = hash(sig);
				if (hashtable.count(hashsig))
				{
					for (std::vector<HASHNODE>::iterator iter = hashtable[hashsig].begin(); iter != hashtable[hashsig].end(); ++iter)
					{
						if (sig == iter->m_sig)
						{
							matchdfas.push_back(iter->m_nDfaId);
						}
					}
				}
			}

		}
		std::sort(matchdfas.begin(), matchdfas.end());
		matchdfas.erase(std::unique(matchdfas.begin(), matchdfas.end()), matchdfas.end());
	}
}

MATCHPKT void DfaMatchPkt(const u_char *data, size_t len, DFAMCH dfamch)
{
	std::ofstream matchresult(dfamch.resultPath, std::ofstream::app);
	matchresult << dpktnum << ":  ";
	std::vector<size_t> matchdfas;
	std::vector<size_t> matcheddfaids;
	GetMchDfas(data, len, dfamch.hashtable, matchdfas);

	for (std::vector<size_t>::iterator iter = matchdfas.begin(); iter != matchdfas.end(); ++iter)
	{
		MatchOnedfa(data, len, dfamch.mergedDfas.GetDfaTable()[*iter], matcheddfaids);
	}

	std::sort(matcheddfaids.begin(), matcheddfaids.end());
	matcheddfaids.erase(std::unique(matcheddfaids.begin(), matcheddfaids.end()), matcheddfaids.end());


	if (!matcheddfaids.empty())
	{
		std::unordered_map<size_t, size_t> &dId_sId = dfamch.dIdSId.dId_sId;
		std::unordered_map<size_t, CVectorUnsigned> &sId_dIdVec = dfamch.dIdSId.sId_dIdVec;

		std::unordered_map<size_t, CVectorUnsigned> resultmap;
		for(std::vector<size_t>::iterator iter = matcheddfaids.begin(); iter != matcheddfaids.end(); ++iter)
		{
			std::size_t &csid = dId_sId[*iter];
			if (resultmap.count(csid) == 0)
			{
				resultmap[csid].Reserve(20);
				resultmap[csid].PushBack(*iter);
			}
			else
			{
				resultmap[csid].PushBack(*iter);
			}
		}

		for(std::unordered_map<size_t, CVectorUnsigned>::iterator iter = resultmap.begin(); iter != resultmap.end(); ++iter)
		{
			if(iter->second.Size() == sId_dIdVec[iter->first].Size())
			{

				std::cout << iter->second[0] << " " << sId_dIdVec[iter->first][0] ;

				matchresult << iter->first << "  ";
			}
		}

	}
	matchresult << std::endl;
	matchresult.close();
}



void CALLBACK DPktParam(const ip_header *ih, const BYTE *data, void* user)
{
	++dpktnum;
	if(dpktnum == 77)
	{
	DFAMCH &dfamch = *(DFAMCH *)user;
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

			if(*data == 227)
			{
				std::cout <<std::endl;
			}
			size_t tcpdatalen = _tlen - _ihl - tcpHdrLen;
			if(tcpdatalen > 0)
			{
				DfaMatchPkt(data, tcpdatalen, dfamch);
			}

			break;
		}

	case _UDP:
		{
			pudp = (udp_header*)((BYTE*) pudp + _ihl);
			data += _ihl + UDPHDRLEN;

			if(*data == 227)
			{
				std::cout <<std::endl;
			}
			size_t udpdatalen = _tlen - _ihl - UDPHDRLEN;
			if(udpdatalen > 0)
			{
				DfaMatchPkt(data, udpdatalen, dfamch);
			}

			break;
		}
	}
	}
	std::cout << dpktnum << std::endl;
}

bool DMyLoadCapFile(const char* pFile, PACKETRECV cv, void* pUser)
{
	pcap_t *mypcap = pcap_open_offline(pFile, NULL);
	PACKETPARAM pp;
	pp.pUser = pUser;
	pp.pFunc = cv;

	pcap_loop(mypcap, 0, MchDfaHdler, (BYTE*)&pp);
	pcap_close(mypcap);
	return true;
}

MATCHPKT bool DLoadCapFile(const char* pFile, void* pUser)
{
	return DMyLoadCapFile(pFile, DPktParam, pUser);
}

MATCHPKT void DHandleAllFile(const std::string &path, void* user)
{
	DFAMCH &dfamch = *(DFAMCH *)user;
	std::ofstream matchresult(dfamch.resultPath, std::ofstream::trunc);
	matchresult.close();
	matchresult.open(dfamch.resultPath, std::ofstream::app);

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
				DHandleAllFile(str + std::string(wfda.cFileName), user);
			}
		}
		else
		{

			std::string &temp = str + std::string(wfda.cFileName);
			std::string &ext1 = temp.substr(temp.size() - 4, 4);
			matchresult << "-----------------------" << temp << "-----------------------" << std::endl;
			if(ext1 == ".cap")
			{
				DLoadCapFile(temp.c_str(), &dfamch);
			}
			dpktnum = 0;
		}
	}
	matchresult.close();
}


MATCHPKT void DfaidSidMap(CGROUPRes &mergedDfas, DFASIDMAPPING &didSid)
{
	CSidDfaIdsNew &sd = mergedDfas.GetSidDfaIds();

	for(size_t i = 0; i < sd.Size(); ++i)
	{
		COMPILEDRULENEW &onemap = sd[i];

		didSid.sId_dIdVec;
		didSid.sId_dIdVec[onemap.m_nSid].Resize(20);
		didSid.sId_dIdVec[onemap.m_nSid] = sd[i].m_dfaIds;

		for(size_t j = 0; j < onemap.m_dfaIds.Size(); ++j)
		{
			didSid.dId_sId[onemap.m_dfaIds[j]] = onemap.m_nSid;
		}
	}
}