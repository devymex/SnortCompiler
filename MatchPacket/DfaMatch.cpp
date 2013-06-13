#include "stdafx.h"
#include "../compilernew/compilernew.h"
#include "DfaMatch.h"
#include "../grouping/grouping.h"

static size_t dpktnum = 0;
//���� 0 ��ʾû��ƥ���ϣ����� 1 ��ʾƥ����
void MatchOnedfa(const u_char * &data, size_t len, CDfaNew &dfa, std::vector<size_t> &matchedDids)
{
	size_t flags[260];
	size_t size = sizeof(flags);
	std::memset(flags, 0, sizeof(flags));


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
			dfaids[term.dfaSta].reserve(10);
			dfaids[term.dfaSta].push_back(term.dfaId);
		}
	}

	////将dfa转为矩阵
	//BYTE stacount = dfa.Size();
	//const BYTE groupcount = dfa.GetGroupCount();

	//STATEID *dfamtx = new STATEID[stacount * groupcount];
	//for (size_t sta = 0; sta < dfa.Size(); ++sta)
	//{
	//	for (size_t group = 0; group < groupcount; ++group)
	//	{

	//	}
	//}

	//BYTE *edgegroup = new BYTE[256];
	//for(size_t edge = 0; edge < 256; ++edge)
	//{
	//	edgegroup[edge] = dfa.GetOneGroup(edge);
	//}

	STATEID curSta = dfa.GetStartId();
	for (size_t edgeiter = 0; edgeiter != len; ++edgeiter)
	{
		BYTE group = dfa.GetOneGroup(data[edgeiter]);

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
			if (0 == flags[curSta])
			{
				matchedDids.insert(matchedDids.end(), dfaids[curSta].begin(), dfaids[curSta].end());
				flags[curSta] = 1;
			}
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

	if((dfa[curSta].GetFlag() & CDfaRow::TERMINAL) != 0)
	{
		if(0 == flags[curSta])
		{
			matchedDids.insert(matchedDids.end(), dfaids[curSta].begin(), dfaids[curSta].end());
			flags[curSta] = 1;
		}
	}

	std::sort(matchedDids.begin(), matchedDids.end());
	matchedDids.erase(std::unique(matchedDids.begin(), matchedDids.end()), matchedDids.end());
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
		std::ofstream sigfile;
		sigfile.open("..\\..\\output\\sigfile.txt");
		SIGNATURE sig;
		u_char csig[4];
		char flags[3000];
		std::memset(flags, 0, sizeof(flags));

		for (const u_char* iter = data; iter != &data[len - 4]; ++iter)
		{
			for (size_t i = 0; i < 4; ++i)
			{
				csig[i] = tolower(*(iter + i));
			}
			sig = *(SIGNATURE *)csig;


				size_t hashsig = hash(sig);
				if (hashtable.count(hashsig))
				{
					for (std::vector<HASHNODE>::iterator iter = hashtable[hashsig].begin(); iter != hashtable[hashsig].end(); ++iter)
					{
						if ((sig == iter->m_sig) && (flags[iter->m_nDfaId] == 0))
						{
							matchdfas.push_back(iter->m_nDfaId);
							flags[iter->m_nDfaId] = 1;
						}
					}
				}
		}
		if (matchdfas.size() > 8)
		{
			sigfile << dpktnum << "  ";
		}
		//std::sort(matchdfas.begin(), matchdfas.end());
		//matchdfas.erase(std::unique(matchdfas.begin(), matchdfas.end()), matchdfas.end());
	}
}

MATCHPKT void DfaMatchPkt(const u_char *data, size_t len, DFAMCH dfamch)
{
	//std::ofstream f729;
	//f729.open("..\\..\\output\\test.txt");
	//for(size_t i = 0; i < len; ++i)
	//{
	//	f729 << data[i];
	//}
	//f729.close();

	std::ofstream matchresult;
	if (dpktnum == 1)
	{
		matchresult.open(dfamch.resultPath);
		matchresult << "-----------------------" << dfamch.resultPath << "-----------------------" << std::endl;
	}
	else
	{
		matchresult.open(dfamch.resultPath, std::ofstream::app);
	}
	matchresult << dpktnum << " : ";
	std::vector<size_t> matchdfas;
	std::vector<size_t> matcheddfaids;
	GetMchDfas(data, len, dfamch.hashtable, matchdfas);

	for (std::vector<size_t>::iterator iter = matchdfas.begin(); iter != matchdfas.end(); ++iter)
	{
		MatchOnedfa(data, len, dfamch.mergedDfas.GetDfaTable()[*iter], matcheddfaids);
	}

	if (!matcheddfaids.empty())
	{
		std::unordered_map<size_t, size_t> &dId_sId = dfamch.dIdSId.dId_sId;
		std::unordered_map<size_t, CVectorNumber> &sId_dIdVec = dfamch.dIdSId.sId_dIdVec;


		std::unordered_map<size_t, CVectorNumber> resultmap;
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

		std::vector<size_t> matchvec;
		for(std::unordered_map<size_t, CVectorNumber>::iterator iter = resultmap.begin(); iter != resultmap.end(); ++iter)
		{
			if(iter->second.Size() == sId_dIdVec[iter->first].Size())
			{
				matchvec.push_back(iter->first);
				//matchresult << iter->first << "  ";
			}
		}

		std::sort(matchvec.begin(), matchvec.end());
		for(std::vector<size_t>::iterator iter = matchvec.begin(); iter != matchvec.end(); ++iter)
		{
			matchresult << *iter << "  ";
		}

	}
	matchresult << std::endl;
	matchresult.close();
}



void CALLBACK DPktParam(const ip_header *ih, const BYTE *data, void* user)
{
	++dpktnum;
	//if(dpktnum == 729)
	//{
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

			size_t udpdatalen = _tlen - _ihl - UDPHDRLEN;
			if(udpdatalen > 0)
			{
				DfaMatchPkt(data, udpdatalen, dfamch);
			}

			break;
		}
	}
	std::cout << dpktnum << std::endl;
	//}
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
	std::string dfapath = dfamch.resultPath;

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
			if(ext1 == ".cap")
			{
				dfamch.resultPath = dfapath + "\\" + std::string(wfda.cFileName) + ".txt";

				DLoadCapFile(temp.c_str(), &dfamch);
			}
			dpktnum = 0;
		}
	}
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