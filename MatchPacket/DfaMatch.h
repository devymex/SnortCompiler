#pragma once
#include "MatchPkt.h"
#include <fstream>
#include "../hashmapping/hashmapping.h"

struct DFAMCH
{
	HASHRES &hashtable;
	CGROUPRes &mergedDfas;
	std::ofstream &matchresult;
};

void MatchOnedfa(const u_char * &data, size_t len, CDfaNew &dfa, std::vector<size_t> &matchedDids);
void MchDfaHdler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
void GetMchDfas(const u_char *data, size_t len, HASHRES &hashtable, std::vector<size_t> &matchdfas);
MATCHPKT void DfaMatchPkt(const u_char *data, size_t len, DFAMCH dfamch);
void CALLBACK DPktParam(const ip_header *ih, const BYTE *data, void* user);
MATCHPKT bool DLoadCapFile(const char* pFile, void* pUser);
MATCHPKT void DHandleAllFile(const std::string &path, void* user);




