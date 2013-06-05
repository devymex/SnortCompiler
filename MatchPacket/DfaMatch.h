#include "MatchPkt.h"
#include <fstream>
#include "../hashmapping/hashmapping.h"

struct DFAMCH
{
	HASHRES &hashtable;
	CGROUPRes &mergedDfas;
	std::ofstream &matchresult;
};

void CALLBACK DPktPara(const ip_header *ih, const BYTE *data, void* user);
void MchDfaHdler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
void GetMchDfas(const u_char *data, size_t len, HASHRES &hashtable, std::vector<size_t> &matchdfas);
//MATCHPKT void DfaMatchPkt(const u_char *data, size_t len, DFAMCH dfamch);


