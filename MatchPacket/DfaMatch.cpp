#include "stdafx.h"
#include "DfaMatch.h"
//#include "../grouping/grouping.h"


//���� 0 ��ʾû��ƥ���ϣ����� 1 ��ʾƥ����
void MatchOnedfa(const u_char * &data, size_t len, CDfaNew &dfa, std::vector<size_t> &matchedDids)
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
	for (size_t edgeiter = 0; edgeiter != len; ++edgeiter)
	{
		BYTE group = dfa.GetOneGroup(edgeiter);

		if (0 == (dfa[curSta].GetFlag() & CDfaRow::TERMINAL))
		{
			if (dfa[curSta][group] != -1)
			{
				curSta = dfa[curSta][group];
			}
		}
		else
		{
			matchedDids.insert(matchedDids.end(), dfaids[curSta].begin(), dfaids[curSta].end());
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

void CALLBACK DPktPara(const ip_header *ih, const BYTE *data, void* user)
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
				//DfaMatchPkt(data, tcpdatalen, dfamch);
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
				//DfaMatchPkt(data, udpdatalen, dfamch);
			}

			break;
		}
	}
}

void GetMchDfas(const u_char *data, size_t len, HASHRES &hashtable, std::vector<size_t> &matchdfas)
{
	if (len > 3)
	{
		SIGNATURE sig;
		u_char csig[4];
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
					if (sig == iter->m_sig)
					{
						matchdfas.push_back(iter->m_nDfaId);
					}
				}
			}
		}
	}
}

//MATCHPKT void DfaMatchPkt(const u_char *data, size_t len, DFAMCH dfamch)
//{
//	std::vector<size_t> matchdfas;
//	std::vector<size_t> matcheddfaids;
//	GetMchDfas(data, len, dfamch.hashtable, matchdfas);
//
//	for (std::vector<size_t>::iterator iter = matchdfas.begin(); iter != matchdfas.end(); ++iter)
//	{
//		MatchOnedfa(data, len, dfamch.mergedDfas.GetDfaTable()[*iter], matcheddfaids);
//	}
//
//	if (!matcheddfaids.empty())
//	{
//
//	}
//}
//
//void DfaidSidMap(CGROUPRes &mergedDfas, std::unordered_map<size_t, size_t> &didSid)
//{
//	CSidDfaIdsNew sd = mergedDfas.GetSidDfaIds();
//
//	for(size_t i = 0; i < sd.Size(); ++i)
//	{
//		COMPILEDRULENEW &onemap = sd[i];
//		for(size_t j = 0; j < onemap.m_dfaIds.Size(); ++j)
//		{
//			didSid[onemap.m_dfaIds[j]] = onemap.m_nSid;
//		}
//	}
//}