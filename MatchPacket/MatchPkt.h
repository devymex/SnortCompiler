#pragma once
//#define NTDDI_VERSION NTDDI_WIN7
//#define _WIN32_WINNT _WIN32_WINNT_WIN7
//#define WINVER _WIN32_WINNT_WIN7

#include <Windows.h>
#include <pcap.h>

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

#include <hwprj\dfa.h>
#include <hwprj\compiler.h>
#include <hwprj\snortrule.h>
#include <hwprj\pcrematch.h>
#include <hwprj\buildhash.h>

#define ETHDRLEN 14 
#define _TCP 6
#define _UDP 17
#define UDPHDRLEN 8

#ifndef MATCHPKT_H_
#define MATCHPKT __declspec(dllimport)
#else
#define MATCHPKT __declspec(dllexport)
#endif

/* 4字节的IP地址 */
typedef struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

/* 4字节的IP地址 */
typedef struct ip_header{
    u_char  ver_ihl;        // �汾 (4 bits) + �ײ����� (4 bits)
    u_char  tos;            // ��������(Type of service) 
    u_short tlen;           // �ܳ�(Total length) 
    u_short identification; // ��ʶ(Identification)
    u_short flags_fo;       // ��־λ(Flags) (3 bits) + ��ƫ����(Fragment offset) (13 bits)
    u_char  ttl;            // ���ʱ��(Time to live)
    u_char  proto;          // Э��(Protocol)
    u_short crc;            // �ײ�У���(Header checksum)
    ip_address  saddr;      // Դ��ַ(Source address)
    ip_address  daddr;      // Ŀ�ĵ�ַ(Destination address)
    u_int   op_pad;         // ѡ�������(Option + Padding)
}ip_header;

/* UDP �ײ�*/
typedef struct udp_header{
    u_short sport;          // Դ�˿�(Source port)
    u_short dport;          // Ŀ�Ķ˿�(Destination port)
    u_short len;            // UDP���ݰ�����(Datagram length)
    u_short crc;            // У���(Checksum)
}udp_header;

typedef struct tcp_header //����TCP�ײ�

{

u_short saddr;           //16λԴ�˿�

u_short daddr;           //16λĿ�Ķ˿�

u_int seq;               //32λ���к�

u_int ack;               //32λȷ�Ϻ�

u_char lenres;           //4λ�ײ�����/6λ������

u_char flag;             //6λ��־λ

u_short window;          //16λ���ڴ�С

u_short crc;             //16λУ���

u_short urp;            //16λ�������ƫ����

}tcp_header;

inline ulong HashFcn(SIGNATURE sig)
{
	u_char *cSig = (u_char *)&sig;

	const ulong _FNV_offset_basis = 2166136261U;
	const ulong _FNV_prime = 16777619U;

	ulong _Val = _FNV_offset_basis;	
	for(ulong i = 0; i < 4; ++i)
	{
		_Val ^= cSig[i];
		_Val *= _FNV_prime;
	}

	return (_Val);
}
typedef void (__stdcall*PACKETRECV) (const ip_header *ih, const byte *data, void* pParam);

struct PACKETPARAM
{
	void *pUser;
	PACKETRECV pFunc;
};

struct MATCHRESULT
{
	ulong pktnum;
	std::vector<ulong> matchedSids;
};

struct SIGSID
{
	ulong sid;
	SIGNATURE sig;
};


struct REGRULES
{
	ulong m_nSid;
	CRegRule regrule;
};

struct SIG_HASH
{
	ulong operator()(const SIGNATURE &str)
	{
		const ulong _FNV_offset_basis = 2166136261U;
		const ulong _FNV_prime = 16777619U;

		ulong _Val = _FNV_offset_basis;	
		for(ulong i = 0; i < 4; ++i)
		{
			_Val ^= str >> (8 * i);
			_Val *= _FNV_prime;
		}

		return (_Val);
	}
};

typedef std::unordered_map<SIGNATURE, std::vector<ulong>, SIG_HASH> SIGSMAP;
struct REGRULESMAP
{
	std::vector<REGRULES> result;
	SIGSMAP sigmap;
	std::ofstream mchresult;
	std::string resultpath;
};

void __stdcall PktParam(const ip_header *ih, const byte *data, void* user);

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
bool MyLoadCapFile(const char* pFile, PACKETRECV cv, void* pUser);
MATCHPKT bool LoadCapFile(const char* pFile, void* pUser);

void __stdcall MyProcess(const CSnortRule &rule, LPVOID lpParam);
MATCHPKT void MchCompile(LPCTSTR filename, LPVOID result);
MATCHPKT bool TradithinalMatch(std::vector<u_char> &dataSrc, CRegRule &regRule);//����pcreMATCHPKT 

void GetMchRule(const u_char *data, ulong len, void* user, std::vector<ulong> &rules);
void HdlOnePkt(const u_char *data, ulong len, void*user);
bool MyPcreMatch(const u_char *data, ulong len, CRegRule &regRule);
MATCHPKT void HandleAllFile(const std::string &path, void* user);
