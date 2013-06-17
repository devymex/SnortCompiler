#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <hwprj\buildhash.h>
#include <hwprj\unsary.h>
#include <hwprj\siddfaids.h>
//#include <hwprj\groupres.h>
#include "MatchPkt.h"

struct DFASIDMAPPING
{
	std::unordered_map<ulong, ulong> dId_sId;
	std::unordered_map<ulong, CUnsignedArray> sId_dIdVec;
};

struct DFAMCH
{
	std::string resultPath;
	HASHRES hashtable;
	CGroupRes mergedDfas;
	DFASIDMAPPING dIdSId;
};

void MatchOnedfa(const u_char * &data, size_t len, CDfa &dfa, std::vector<size_t> &matchedDids);

void MchDfaHdler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

void GetMchDfas(const u_char *data, size_t len, HASHRES &hashtable, std::vector<size_t> &matchdfas);

MATCHPKT void DfaMatchPkt(const u_char *data, size_t len, DFAMCH &dfamch);

void CALLBACK DPktParam(const ip_header *ih, const BYTE *data, void* user);

bool DMyLoadCapFile(const char* pFile, PACKETRECV cv, void* pUser);

void ResultFiles(const std::string &path, std::vector<std::string> &resultFiles);

MATCHPKT bool DLoadCapFile(const char* pFile, void* pUser);

MATCHPKT void DHandleAllFile(const std::string &path, void* user);

MATCHPKT void DfaidSidMap(CGroupRes &mergedDfas, DFASIDMAPPING &didSid);




