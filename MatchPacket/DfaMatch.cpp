﻿#include "stdafx.h"
#include <hwprj\compiler.h>
#include <hwprj\grouping.h>
#include <hwprj\dfa.h>
#include "DfaMatch.h"

static ulong dpktnum = 0;

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

MATCHPKT void DfaMatchPkt(const u_char *data, ulong len, DFAMCH &dfamch)
{
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
	std::vector<ulong> matchdfas;
	std::vector<ulong> matcheddfaids;
	GetMchDfas(data, len, dfamch.hashtable, matchdfas);

	for (std::vector<ulong>::iterator iter = matchdfas.begin(); iter != matchdfas.end(); ++iter)
	{
		MatchOnedfa(data, len, dfamch.mergedDfas.GetDfaTable()[*iter], matcheddfaids);
	}

	if (!matcheddfaids.empty())
	{
		std::unordered_map<ulong, ulong> &dId_sId = dfamch.dIdSId.dId_sId;
		std::unordered_map<ulong, CUnsignedArray> &sId_dIdVec = dfamch.dIdSId.sId_dIdVec;

		std::unordered_map<ulong, CUnsignedArray> resultmap;
		for(std::vector<ulong>::iterator iter = matcheddfaids.begin(); iter != matcheddfaids.end(); ++iter)
		{
			ulong &csid = dId_sId[*iter];
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

		std::vector<ulong> matchvec;
		for(std::unordered_map<ulong, CUnsignedArray>::iterator iter = resultmap.begin(); iter != resultmap.end(); ++iter)
		{
			if(iter->second.Size() == sId_dIdVec[iter->first].Size())
			{
				matchvec.push_back(iter->first);
			}
		}

		std::sort(matchvec.begin(), matchvec.end());
		for(std::vector<ulong>::iterator iter = matchvec.begin(); iter != matchvec.end(); ++iter)
		{
			matchresult << *iter << "  ";
		}
	}

	matchresult << std::endl;
	matchresult.close();
}



void __stdcall DPktParam(const ip_header *ih, const byte *data, void* user)
{
	++dpktnum;
	
	DFAMCH &dfamch = *(DFAMCH *)user;
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
	
	if (dpktnum % 100 == 0)
	{
		std::cout << dpktnum << std::endl;
	}
}

bool DMyLoadCapFile(const char* pFile, PACKETRECV cv, void* pUser)
{
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