#pragma once
//#define NTDDI_VERSION NTDDI_WIN7
//#define _WIN32_WINNT _WIN32_WINNT_WIN7
//#define WINVER _WIN32_WINNT_WIN7
#define WIN32
#include <pcap.h>
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <Windows.h>

#define ETHDRLEN 14 

#ifndef MATCHPKT_H_
#define MATCHPKT __declspec(dllimport)
#else
#define MATCHPKT __declspec(dllexport)
#endif

#define _TCP 6
#define _UDP 17
#define UDPHDRLEN 8

/* 4�ֽڵ�IP��ַ */
typedef struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

/* IPv4 �ײ� */
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

typedef struct tcp_header             //����TCP�ײ�

{

u_short saddr;               //16λԴ�˿�

u_short daddr;               //16λĿ�Ķ˿�

u_int seq;         //32λ���к�

u_int ack;         //32λȷ�Ϻ�

u_char lenres;        //4λ�ײ�����/6λ������

u_char flag;            //6λ��־λ

u_short window;                 //16λ���ڴ�С

u_short crc;                 //16λУ���

u_short urp;                 //16λ��������ƫ����

}tcp_header;


typedef void (CALLBACK *PACKETRECV) (const ip_header *ih, const BYTE *data, void* user);

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
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
bool MyLoadCapFile(const char* pFile, PACKETRECV cv, void* pUser);
MATCHPKT bool LoadCapFile(const char* pFile, void* pUser);

//void FindSig(SIGNATURE sig, std::map<size_t, std::vector<SIGSID>> &hashtable, std::vector<size_t> &matchedSids);
//MATCHPKT void MatchPkt(std::vector<std::vector<u_char>> &allPkt, std::map<size_t, std::vector<SIGSID>> &hashtable, std::vector<CDfanew> &alldfas, std::vector<MATCHRESULT> &matchresult);
//	