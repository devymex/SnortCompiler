#include "../MatchPacket/MatchPkt.h"
#include "../PMatchPkt/PMatchPkt.h"

#include <iostream>
#include <tchar.h>

void GetMchRule(std::vector<u_char> &pkt, REGRULESMAP &rulesmap, std::vector<size_t> &rules)
{
	SIGSMAP sigmap = rulesmap.sigmap;
	SIGNATURE sig;
	for(std::vector<u_char>::iterator iter = pkt.begin(); iter + 3 != pkt.end(); ++iter)
	{
		sig = *(SIGNATURE *)&(*iter);
		if(sigmap.count(sig))
		{
			rules.insert(rules.end(), sigmap[sig].begin(), sigmap[sig].end());
		}
	}
}

void main(size_t argc, char* argv[])
{
	std::vector<std::vector<u_char>> allPkt;
	LoadCapFile("c://test//firpack.cap", &allPkt);

	REGRULESMAP rulesmap;
	MchCompile(_T("../../input/allrules.rule"), &rulesmap);

	for(std::vector<std::vector<u_char>>::iterator allIter = allPkt.begin(); allIter != allPkt.end(); ++allIter)
	{
		std::vector<size_t> rules;
		GetMchRule(*allIter, rulesmap, rules);
		std::vector<size_t> matchedSid;//保存匹配上的规则的sid
		for(std::vector<size_t>::iterator rIt = rules.begin(); rIt != rules.end(); ++rIt)
		{
			bool flag = TradithinalMathch(*allIter, rulesmap.result[*rIt].regrule);
			if(flag == HAS_MATHCHED)
			{
				//匹配成功
				matchedSid.push_back(rulesmap.result[*rIt].m_nSid);
			}
		}
	}
	std::cout << rulesmap.sigmap.size();
}