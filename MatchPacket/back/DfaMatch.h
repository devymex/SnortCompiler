#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <hwprj\buildhash.h>
#include <hwprj\unsary.h>
#include <hwprj\siddfaids.h>
#include "MatchPkt.h"

extern ulong g_dpktnum;

struct DFASIDMAPPING
{
	std::unordered_map<ulong, ulong> dId_sId;
	std::unordered_map<ulong, CUnsignedArray> sId_dIdVec;
};

struct DFAMCH
{
	const char* pfilepath;
	std::string resultFolder;
	std::string resultPath;
	HASHRES hashtable;
	CGroupRes mergedDfas;
	DFASIDMAPPING dIdSId;
};

struct PCREDFA
{
	DFAMCH dfamch;
	REGRULESMAP rulesmap;
};

void MatchOnedfa(const u_char * &data, size_t len, CDfa &dfa, std::vector<size_t> &matchedDids);

void MchDfaHdler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

void GetMchDfas(const u_char *data, size_t len, HASHRES &hashtable, std::vector<size_t> &matchdfas);

MATCHPKT void DfaMatchPkt(const u_char *data, size_t len, DFAMCH &dfamch);

void CALLBACK DPktParam(const ip_header *ih, const BYTE *data, void* user);

bool DMyLoadCapFile(const char* pFile, PACKETRECV cv, void* pUser);

void ResultFiles(const std::string &path, std::vector<std::string> &resultFiles);
void CompareResult(const char* path1, const char* path2);

MATCHPKT bool DLoadCapFile(const char* pFile, void* pUser);

MATCHPKT void PDHandleAllFile(const std::string &path, void* user);

MATCHPKT void DfaidSidMap(CGroupRes &mergedDfas, DFASIDMAPPING &didSid);


typedef unsigned long long ulong64;
extern ulong64 g_ulAllDp;
extern ulong64 g_ulHashed;

