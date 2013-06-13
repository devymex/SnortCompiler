#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../mergedfanew/MergeDfanew.h"

void MatchOnedfa(u_char *data, size_t len, CDfaNew &dfa, std::vector<size_t> &matchedDids)
{
	size_t flags[260];
	size_t size = sizeof(flags);
	std::memset(flags, 0, sizeof(flags));

	//for(size_t i = 0; i < dfa.Size(); ++i)
	//{
	//	size_t edgenum = 0;
	//	std::cout << i << " : ";
	//	for(size_t j = 0; j < 256; ++j)
	//	{
	//		size_t g = dfa.GetOneGroup(j);
	//		if(dfa[i][g] != (STATEID)-1)
	//		{
	//			std::cout << "(" << j << "," << dfa[i][g] << ")";
	//			++edgenum;
	//		}
	//	}
	//	std::cout << std::endl;
	//	std::cout << "edgenum: " << edgenum << std::endl;
	//}

	std::unordered_map<size_t, std::vector<size_t>> dfaids;
	for (size_t i = 0; i < dfa.GetTermCnt(); ++i)
	{
		TERMSET &term = dfa.GetTerm(i);

		if (dfaids.count(term.dfaSta))
		{
			dfaids[term.dfaSta].push_back(term.dfaId);
		}
		else
		{
			dfaids[term.dfaSta].reserve(10);
			dfaids[term.dfaSta].push_back(term.dfaId);
		}
	}

	STATEID curSta = dfa.GetStartId();

	for (size_t edgeiter = 0; edgeiter != len; ++edgeiter)
	{
		BYTE group = dfa.GetOneGroup(data[edgeiter]);

		if (0 == (dfa[curSta].GetFlag() & CDfaRow::TERMINAL))
		{
			if (dfa[curSta][group] != (STATEID)-1)
			{
				curSta = dfa[curSta][group];
			}
			else
			{
				return;
			}
		}
		else
		{
			if (0 == flags[curSta])
			{
				matchedDids.insert(matchedDids.end(), dfaids[curSta].begin(), dfaids[curSta].end());
				flags[curSta] = 1;
			}
			if(dfa[curSta][group] != (STATEID)-1)
			{
				curSta = dfa[curSta][group];
			}
			else 
			{
				return;
			}
		}
	}

	if((dfa[curSta].GetFlag() & CDfaRow::TERMINAL) != 0)
	{
		if(0 == flags[curSta])
		{
			matchedDids.insert(matchedDids.end(), dfaids[curSta].begin(), dfaids[curSta].end());
			flags[curSta] = 1;
		}
	}

	std::sort(matchedDids.begin(), matchedDids.end());
	matchedDids.erase(std::unique(matchedDids.begin(), matchedDids.end()), matchedDids.end());
}

void main()
{
	u_char buff[1000];
	std::ifstream ifs("..\\..\\output\\test.txt");
	ifs.read((char *)buff, 1000);
	CNfa nfa1;
	const char *a1 = "/^Accept\\x2dCharset\\x3a\\s*([^\\x3b\\x3d\\x2c]{1,36}\\s*(\\x2d|\\x3b|\\x3d|\\x2c)\\s*)*[^\\x2d\\x3b\\x2c\\x3d\\n]{37}/smi";
	CRegChain regChain;

	
	PcreToNFA(a1, nfa1, regChain);
	size_t size = nfa1.Size();
	
	CDfaNew dfa1;
	dfa1.SetId(1);


	dfa1.FromNFA(nfa1, NULL, 0);
	size = dfa1.Size();
	//dfa5.Minimize();
	outPutDfa(dfa1, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\dfa1.txt");


	CDfaNew mergeDfa;
	std::vector<CDfaNew> dfas;
	dfas.push_back(dfa1);

	for (size_t i = 0; i < dfa1.Size(); ++i)
	{
		if(dfa1[i].GetFlag() & CDfaRow::TERMINAL)
		{
		std::cout << dfa1[i].GetFlag() << std::endl;
		}
	}

	dfa1.Minimize();

	size = dfa1.Size();
	std::vector<size_t> matchedDfas;
	MatchOnedfa(buff, 745, dfa1, matchedDfas);

	std::cout << (size_t)dfa1.Size() << std::endl;

	std::cout << (size_t)dfa1.GetStartId() << std::endl;
	for (size_t i = 0; i < dfa1.Size(); ++i)
	{
		if (dfa1[i].GetFlag() & CDfaRow::TERMINAL)
		{
			std::cout << i << std::endl;
		}
	}
	outPutDfa(dfa1, "..\\..\\output\\test.txt");

	system("pause");
}