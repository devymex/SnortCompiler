#include "../MatchPacket/MatchPkt.h"
#include "../PMatchPkt/PMatchPkt.h"
#include <fstream>

#include <iostream>
#include <tchar.h>

void GetMchRule(std::vector<u_char> &pkt, REGRULESMAP &rulesmap, std::vector<size_t> &rules)
{
	SIGSMAP sigmap = rulesmap.sigmap;
	SIGNATURE sig;
	u_char csig[4];
	for(std::vector<u_char>::iterator iter = pkt.begin(); iter + 3 != pkt.end(); ++iter)
	{
		for(size_t i = 0; i < 4; ++i)
		{
			csig[i] = tolower(*(iter + i));
		}
		sig = *(SIGNATURE *)csig;
		if(sigmap.count(sig))
		{
			rules.insert(rules.end(), sigmap[sig].begin(), sigmap[sig].end());
		}
	}
}

void main(size_t argc, char* argv[])
{
	std::vector<std::vector<u_char>> allPkt;
	//LoadCapFile("F:\\cppProject\\huawei\\PreciseMatch\\PacketMatch\\2_71_u_2.cap00_up.cap", &allPkt);

	REGRULESMAP rulesmap;
	MchCompile(_T("../../input/test.rules"), &rulesmap);

	u_char p[] = {0, 0, 'f', 'g', 0, 'a', 'b', 'C', 'd', 0, 'd', 'e', 235, 0, 42, 'A', 123, 'B', 40, '1', '2', 93, 63, 'a', 'b', 'c', 'd', 'e', 'a'};
	std::vector<u_char> tmpvec(p, p + sizeof(p));
	allPkt.push_back (tmpvec);

	std::ofstream matchresult("C:\\test\\matchresult.txt");
	for(std::vector<std::vector<u_char>>::iterator allIter = allPkt.begin(); allIter != allPkt.end(); ++allIter)
	{
		matchresult << allIter - allPkt.begin();
		std::vector<size_t> rules;
		GetMchRule(*allIter, rulesmap, rules);
		std::vector<size_t> matchedSid;//保存匹配上的规则的sid
		for(std::vector<size_t>::iterator rIt = rules.begin(); rIt != rules.end(); ++rIt)
		{
			bool flag = TradithinalMatch(*allIter, rulesmap.result[*rIt].regrule);
			if(flag)
			{
				//匹配成功

				matchresult << rulesmap.result[*rIt].m_nSid << "  ";
				matchedSid.push_back(rulesmap.result[*rIt].m_nSid);
			}
		}

		matchresult << "\n";
	}
	matchresult.close();
	std::cout << rulesmap.sigmap.size();
}