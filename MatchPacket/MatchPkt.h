#define NTDDI_VERSION NTDDI_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define WINVER _WIN32_WINNT_WIN7

#define _WINSOCKAPI_
#include <Windows.h>
#include <pcap.h>


#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Packet.lib")
#pragma comment(lib, "wpcap.lib")

#define ETHDRLEN 14 

#ifndef MATCHPKT_H_
#define MATCHPKT __declspec(dllimport)
#else
#define MATCHPKT __declspec(dllexport)
#endif

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



typedef void (*PACKETRECV) (const ip_header *ih, const BYTE *data, void* pParam);

struct PACKETPARAM
{
	void *pUser;
	PACKETRECV pFunc;
};
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
MATCHPKT bool LoadCapFile(const char* pFile, PACKETRECV cv, void* pUser);
