#define NTDDI_VERSION NTDDI_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define WINVER _WIN32_WINNT_WIN7

#define _WINSOCKAPI_
#include <Windows.h>
#include <pcap.h>

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "../dfanew/dfanew.h"
#include "../common/common.h"
#include "../rule2nfa/rule2nfa.h"
#include "../compilernew/compilernew.h"
#include "../pcre2nfa/match.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Packet.lib")
#pragma comment(lib, "wpcap.lib")

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

/* IPv4 首部 */
typedef struct ip_header{
    u_char  ver_ihl;        // 版本 (4 bits) + 首部长度 (4 bits)
    u_char  tos;            // 服务类型(Type of service) 
    u_short tlen;           // 总长(Total length) 
    u_short identification; // 标识(Identification)
    u_short flags_fo;       // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
    u_char  ttl;            // 存活时间(Time to live)
    u_char  proto;          // 协议(Protocol)
    u_short crc;            // 首部校验和(Header checksum)
    ip_address  saddr;      // 源地址(Source address)
    ip_address  daddr;      // 目的地址(Destination address)
    u_int   op_pad;         // 选项与填充(Option + Padding)
}ip_header;

/* UDP 首部*/
typedef struct udp_header{
    u_short sport;          // 源端口(Source port)
    u_short dport;          // 目的端口(Destination port)
    u_short len;            // UDP数据包长度(Datagram length)
    u_short crc;            // 校验和(Checksum)
}udp_header;

typedef struct tcp_header             //定义TCP首部

{

u_short saddr;               //16位源端口

u_short daddr;               //16位目的端口

u_int seq;         //32位序列号

u_int ack;         //32位确认号

u_char lenres;        //4位首部长度/6位保留字

u_char flag;            //6位标志位

u_short window;                 //16位窗口大小

u_short crc;                 //16位校验和

u_short urp;                 //16位紧急数据偏移量

}tcp_header;

inline size_t HashFcn(SIGNATURE sig)
{
	u_char *cSig = (u_char *)&sig;

	const size_t _FNV_offset_basis = 2166136261U;
	const size_t _FNV_prime = 16777619U;

	size_t _Val = _FNV_offset_basis;	
	for(size_t i = 0; i < 4; ++i)
	{
		_Val ^= cSig[i];
		_Val *= _FNV_prime;
	}

	return (_Val);
}
typedef void (CALLBACK*PACKETRECV) (const ip_header *ih, const BYTE *data, void* pParam);

struct PACKETPARAM
{
	void *pUser;
	PACKETRECV pFunc;
};

struct MATCHRESULT
{
	size_t pktnum;
	std::vector<size_t> matchedSids;
};

struct SIGSID
{
	size_t sid;
	SIGNATURE sig;
};


struct REGRULES
{
	size_t m_nSid;
	CRegRule regrule;
};

struct SIG_HASH
{
	size_t operator()(const SIGNATURE &str)
	{
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;

		size_t _Val = _FNV_offset_basis;	
		for(size_t i = 0; i < 4; ++i)
		{
			_Val ^= str >> (8 * i);
			_Val *= _FNV_prime;
		}

		return (_Val);
	}
};

typedef std::unordered_map<SIGNATURE, std::vector<size_t>, SIG_HASH> SIGSMAP;
struct REGRULESMAP
{
	std::vector<REGRULES> result;
	SIGSMAP sigmap;
	std::ofstream mchresult;
};

void CALLBACK PktParam(const ip_header *ih, const BYTE *data, void* user);

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
bool MyLoadCapFile(const char* pFile, PACKETRECV cv, void* pUser);
MATCHPKT bool LoadCapFile(const char* pFile, void* pUser);

void CALLBACK MyProcess(const CSnortRule &rule, LPVOID lpParam);
MATCHPKT void MchCompile(LPCTSTR filename, LPVOID result);
MATCHPKT bool TradithinalMatch(std::vector<u_char> &dataSrc, CRegRule &regRule);//调用pcreMATCHPKT 

void GetMchRule(const u_char *data, size_t len, void* user, std::vector<size_t> &rules);
void HdlOnePkt(const u_char *data, size_t len, void*user);
MATCHPKT void HandleAllFile(const std::string &path, void* user);
