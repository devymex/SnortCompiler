#pragma once
#include "MatchPkt.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <hwprj\buildhash.h>

struct DFASIDMAPPING
{
	std::unordered_map<ulong, ulong> dId_sId;
	std::unordered_map<ulong, CUnsignedArray> sId_dIdVec;
};

struct DFAMCH
{
	std::string resultPath;
	HASHRES hashtable;
	CGROUPRes mergedDfas;
	DFASIDMAPPING dIdSId;
};

void MatchOnedfa(const u_char * &data, ulong len, CDfa &dfa, std::vector<ulong> &matchedDids);
void MchDfaHdler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
void GetMchDfas(const u_char *data, ulong len, HASHRES &hashtable, std::vector<ulong> &matchdfas);
MATCHPKT void DfaMatchPkt(const u_char *data, ulong len, DFAMCH dfamch);
void __stdcall DPktParam(const ip_header *ih, const byte *data, void* user);
bool DMyLoadCapFile(const char* pFile, PACKETRECV cv, void* pUser);
MATCHPKT bool DLoadCapFile(const char* pFile, void* pUser);
MATCHPKT void DHandleAllFile(const std::string &path, void* user);
MATCHPKT void DfaidSidMap(CGROUPRes &mergedDfas, DFASIDMAPPING &didSid);





