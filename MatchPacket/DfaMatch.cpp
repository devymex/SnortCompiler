#include "stdafx.h"
#include <hwprj\compiler.h>
#include <hwprj\grouping.h>
#include <hwprj\dfa.h>
#include "DfaMatch.h"

ulong64 hitedDfaState = 0;
ulong64 hitedStState  = 0;

ulong64 g_ulAllDp;
ulong64 g_ulHashed;

ulong g_dpktnum = 0;
ulong ucprnum = 0;
ulong signum = 0;
ulong usigtime = 0;

ushort threshold = 2;

std::vector<std::vector<ST> > skipTable;


void ReadSkipTable(const char *str, std::vector<std::vector<ST> > &skipTable)
{
	std::fstream filein(str, std::ios::in | std::ios::binary);
	ushort number;
	filein.read(reinterpret_cast<char *>(&number), sizeof(ushort));
	for(ushort i = 0; i != number; ++i)
	{
		skipTable.push_back(std::vector<ST>());
	}
	for(ushort i = 0; i != number; ++i)
	{
		ushort dfaId, stateNum;
		filein.read(reinterpret_cast<char *>(&dfaId), sizeof(ushort));
		filein.read(reinterpret_cast<char *>(&stateNum), sizeof(ushort));
		for(ushort j = 0; j != stateNum; ++j)
		{
			ST st;
			filein.read(reinterpret_cast<char *>(&st), sizeof(st));

			//std::vector<ushort> skipNode;
			//for(ushort k = 0; k != threshold; ++k)
			//{
			//	char temp;
			//	filein.read(reinterpret_cast<char *>(&temp), sizeof(char));
			//	skipNode.push_back(static_cast<ushort>(temp));
			//}

			skipTable[dfaId].push_back(st);
		}
	}
}

void MatchOnedfa(const unsigned char * &data, ulong len, CDfa &dfa,
				 std::vector<ulong> &matchedDids)
{
	size_t flags[260];
	size_t size = sizeof(flags);
	std::memset(flags, 0, sizeof(flags));

	std::unordered_map<ulong, CUnsignedArray> dfaids;
	const CFinalStates &finStas = dfa.GetFinalStates();
	for (ulong i = 0; i < finStas.Size(); ++i)
	{
		STATEID nStaId = finStas[i];

		finStas.GetDfaIdSet(nStaId).CopyTo(dfaids[nStaId]);
	}





	STATEID curSta = dfa.GetStartState();
	for (size_t edgeiter = 0; edgeiter != len; ++edgeiter)
	{
		BYTE group = dfa.Char2Group(data[edgeiter]);

		if (0 == (dfa[curSta].GetFlags() & CDfaRow::FINAL))
		{
			if (dfa[curSta][group] != (STATEID)-1)
			{
				// 累加命中dfa的所有状态数
				++hitedDfaState;

				// 累加命中dfa的特跳次数
				ST &st = skipTable[dfa.GetId()][group];
				if (curSta == st.ch[0] || curSta == st.ch[1])
				{
					++hitedStState;
				}

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
				for(size_t i = 0 ; i < dfaids[curSta].Size(); ++i)
				{
					matchedDids.push_back(dfaids[curSta][i]);
				}	

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

	if((dfa[curSta].GetFlags() & CDfaRow::FINAL) != 0)
	{
		if(0 == flags[curSta])
		{
			for(size_t i = 0 ; i < dfaids[curSta].Size(); ++i)
			{
				matchedDids.push_back(dfaids[curSta][i]);
			}
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

void GetMchDfas(const u_char *data, ulong len, HASHRES &hashtable, std::vector<ulong> &matchdfas)
{
	if (len > 3)
	{
		std::ofstream sigfile;
		sigfile.open("..\\..\\output\\sigfile.txt", std::ofstream::app);
		SIGNATURE sig;
		u_char csig[4];
		char flags[3000];
		std::memset(flags, 0, sizeof(flags));

		ulong signum = 0;
		for (const u_char* iter = data; iter != &data[len - 3]; ++iter)
		{
			for (size_t i = 0; i < 4; ++i)
			{
				csig[i] = tolower(*(iter + i));
			}
			sig = *(SIGNATURE *)csig;

			size_t hashsig = hash(sig);
			if (hashtable.count(hashsig))
			{
				++signum;
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

	}
}

MATCHPKT void DfaMatchPkt(const u_char *data, ulong len, DFAMCH &dfamch)
{
	//std::ofstream matchresult;

	//matchresult.open(dfamch.resultPath, std::ofstream::app);

	//matchresult << g_dpktnum << " : ";
	std::vector<ulong> matchdfas;
	//std::vector<ulong> matcheddfaids;

	GetMchDfas(data, len, dfamch.hashtable, matchdfas);
	g_ulHashed += matchdfas.size();
	//if (signum > 8)
	//{
	//	++usigtime;
	//	std::ofstream ofs;
	//	if (usigtime == 1)
	//	{
	//		ofs.open("..\\..\\output\\sigfile.txt");
	//	}
	//	else
	//	{
	//		ofs.open("..\\..\\output\\sigfile.txt", std::ofstream::app);
	//	}

	//	ofs << dfamch.pfilepath << " : " << g_pktnum << std::endl;

	//	signum = 0;
	//}
	//for (std::vector<ulong>::iterator iter = matchdfas.begin(); iter != matchdfas.end(); ++iter)
	//{
	//	MatchOnedfa(data, len, dfamch.mergedDfas.GetDfaTable()[*iter], matcheddfaids);
	//}

	//if (!matcheddfaids.empty())
	//{
	//	std::unordered_map<ulong, ulong> &dId_sId = dfamch.dIdSId.dId_sId;
	//	std::unordered_map<ulong, CUnsignedArray> &sId_dIdVec = dfamch.dIdSId.sId_dIdVec;

	//	std::unordered_map<ulong, CUnsignedArray> resultmap;
	//	for(std::vector<ulong>::iterator iter = matcheddfaids.begin(); iter != matcheddfaids.end(); ++iter)
	//	{
	//		ulong &csid = dId_sId[*iter];
	//		if (resultmap.count(csid) == 0)
	//		{
	//			resultmap[csid].Reserve(20);
	//			resultmap[csid].PushBack(*iter);
	//		}
	//		else
	//		{
	//			resultmap[csid].PushBack(*iter);
	//		}
	//	}

	//	std::vector<ulong> matchvec;
	//	for(std::unordered_map<ulong, CUnsignedArray>::iterator iter = resultmap.begin(); iter != resultmap.end(); ++iter)
	//	{
	//		if(iter->second.Size() == sId_dIdVec[iter->first].Size())
	//		{
	//			matchvec.push_back(iter->first);
	//		}
	//	}

	//	std::sort(matchvec.begin(), matchvec.end());
	//	for(std::vector<ulong>::iterator iter = matchvec.begin(); iter != matchvec.end(); ++iter)
	//	{
	//		matchresult << *iter << "  ";
	//	}
	//}

	//matchresult << std::endl;
	//matchresult.close();
}



void __stdcall DPktParam(const ip_header *ih, const byte *data, void* user)
{
	++g_dpktnum;
	
	DFAMCH &dfamch = *(DFAMCH *)user;

	std::ofstream matchresult;
	if (g_dpktnum == 1)
	{
		matchresult.open(dfamch.resultPath);
		matchresult << "-----------------------" << dfamch.resultPath << "-----------------------" << std::endl;
	}
	u_short  _ihl = (ih->ver_ihl & 0x0f) * 4;
	
	u_short _tlen = ih->tlen;
	std::swap(((byte*)&_tlen)[0], ((byte*)&_tlen)[1]);

	u_char  _proto = ih->proto;

	tcp_header *ptcp = (tcp_header *)data;
	udp_header *pudp = (udp_header *)data;

	switch (_proto)
	{
	case _TCP:
		{
			ptcp = (tcp_header*)((byte*) ptcp + _ihl);
			u_short tcpHdrLen = ((ptcp->lenres & 0xf0) >> 4) * 4;
			data += _ihl + tcpHdrLen;

			ulong tcpdatalen = _tlen - _ihl - tcpHdrLen;
			if (_tlen > (_ihl + tcpHdrLen))
			{
				DfaMatchPkt(data, tcpdatalen, dfamch);
			}

			break;
		}

	case _UDP:
		{
			pudp = (udp_header*)((byte*) pudp + _ihl);
			data += _ihl + UDPHDRLEN;


			ulong udpdatalen = _tlen - _ihl - UDPHDRLEN;
			if(udpdatalen > 0)
			{
				DfaMatchPkt(data, udpdatalen, dfamch);
			}

			break;
		}
	}
	
	if (g_dpktnum % 10000 == 0)
	{
		std::cout << g_dpktnum << std::endl;
	}

}

bool DMyLoadCapFile(const char* pFile, PACKETRECV cv, void* pUser)
{
	DFAMCH &dfamch = *(DFAMCH*)pUser;
	dfamch.pfilepath = pFile;
	pcap_t *mypcap = pcap_open_offline(pFile, null);
	PACKETPARAM pp;
	pp.pUser = pUser;
	pp.pFunc = cv;

	pcap_loop(mypcap, 0, MchDfaHdler, (byte*)&pp);
	pcap_close(mypcap);
	return true;

}

MATCHPKT bool DLoadCapFile(const char* pFile, void* pUser)
{
	return DMyLoadCapFile(pFile, DPktParam, pUser);
}

MATCHPKT void PDHandleAllFile(const std::string &path, void* user)
{
	PCREDFA &pcredfa = *(PCREDFA*) user; 
	DFAMCH &dfamch = pcredfa.dfamch;
	REGRULESMAP &rulesmap = pcredfa.rulesmap;

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
				PDHandleAllFile(str + std::string(wfda.cFileName), user);
			}
		}
		else
		{
			std::string &temp = str + std::string(wfda.cFileName);
			std::string &ext1 = temp.substr(temp.size() - 4, 4);
			if(ext1 == ".cap")
			{
				dfamch.resultPath = dfamch.resultFolder + "\\" + std::string(wfda.cFileName) + ".txt";
				DLoadCapFile(temp.c_str(), &dfamch);
				g_ulAllDp += g_dpktnum;

				//std::string pstr = rulesmap.resultpath + "\\" +std::string(wfda.cFileName) +".txt";
				//rulesmap.mchresult.open(rulesmap.resultpath + "\\" +std::string(wfda.cFileName) +".txt");	
				//rulesmap.mchresult << "-----------------------" << pstr << "-----------------------" << std::endl;
				//LoadCapFile(temp.c_str(), &rulesmap);
				//rulesmap.mchresult.close();

				//CompareResult(dfamch.resultPath.c_str(), pstr.c_str());
			}
			g_dpktnum = 0;
			//g_pktnum = 0;
		}
	}
}


MATCHPKT void DfaidSidMap(CGroupRes &mergedDfas, DFASIDMAPPING &didSid)
{
	CSidDfaIds &sd = mergedDfas.GetSidDfaIds();

	for(ulong i = 0; i < sd.Size(); ++i)
	{
		COMPILEDINFO &onemap = sd[i];

		didSid.sId_dIdVec;
		didSid.sId_dIdVec[onemap.m_nSid].Resize(20);
		didSid.sId_dIdVec[onemap.m_nSid] = sd[i].m_dfaIds;

		for(ulong j = 0; j < onemap.m_dfaIds.Size(); ++j)
		{
			didSid.dId_sId[onemap.m_dfaIds[j]] = onemap.m_nSid;
		}
	}
}

void ResultFiles(const std::string &path, std::vector<std::string> &resultFiles)
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
				ResultFiles(str + std::string(wfda.cFileName), resultFiles);
			}
		}
		else
		{
			std::string &temp = str + std::string(wfda.cFileName);
			std::string &ext1 = temp.substr(temp.size() - 4, 4);

			if(ext1 == ".txt")
			{
				resultFiles.push_back(temp);
			}

		}
	}
}

void CompareResult(const char* dpath, const char* ppath)
{
	++ucprnum;
	std::vector<std::string> dresult, presult;

	std::ofstream ofs;
	if (ucprnum == 1)
	{
		ofs.open("D:\\projects\\output\\compare.txt");
	}
	else
	{
		ofs.open("D:\\projects\\output\\compare.txt", std::ofstream::app);
	}


	std::ifstream ifs1(dpath);
	std::ifstream ifs2(ppath);

	std::string dfirline, pfirline;
	std::getline(ifs1, dfirline);
	std::getline(ifs2, pfirline);

	ofs << dfirline << std::endl << pfirline << std::endl;

	while (std::getline(ifs1, dfirline), std::getline(ifs2, pfirline))
	{
		if( dfirline.compare(pfirline) != 0)
		{
			ofs << dfirline << std::endl << pfirline << std::endl << std::endl;
		}
	}

	ofs << std::endl;
}