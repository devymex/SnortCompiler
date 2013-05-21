#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../mergedfanew/MergeDfanew.h"

void main()
{
	CNfa nfa1;
	CNfa nfa2;

	CRegChain regChain;
	//PcreToNFA("/^POST/s", nfa1, regChain);
	//PcreToNFA("/xml version/s", nfa1, regChain);
	//PcreToNFA("/<methodCall><methodName>/s", nfa1, regChain);
	//PcreToNFA("/</methodName><params><param><value><string></string></value></param><param><value><string>/s", nfa1, regChain);

	//PcreToNFA("/^POST/s", nfa2, regChain);
	//PcreToNFA("/xml version/s", nfa2, regChain);
	//PcreToNFA("/<methodCall><methodName>/s", nfa2, regChain);
	//PcreToNFA("/</methodName><params><param><value><name>/s", nfa2, regChain);
	//PcreToNFA("/'\\,''\\x29\\x29\\x3Becho '_begin_\\x0A'\\x3Becho/s", nfa2, regChain);

	PcreToNFA("/^abcd/", nfa1, regChain);
	PcreToNFA("/^xbcd/", nfa2, regChain);

	CDfanew dfa1;
	CDfanew dfa2;

	dfa1.FromNFA(nfa1, NULL, 0);
	dfa2.FromNFA(nfa2, NULL, 0);

	dfa1.Minimize();
	dfa2.Minimize();

	std::vector<CDfanew> dfas(2);
	dfas[0] = dfa1;
	dfas[1] = dfa2;

	CDfanew lastdfa;

	NOrMerge(dfas, lastdfa);
	lastdfa.Minimize();

	CNfa nfa3;
	PcreToNFA("/^ybcd/", nfa3, regChain);
	CDfanew dfa3;
	dfa3.FromNFA(nfa3, NULL, 0);
	dfa3.Minimize();

	dfas[0] = lastdfa;
	dfas[1] = dfa3;
	CDfanew finaldfa;
	NOrMerge(dfas, finaldfa);
	finaldfa.Minimize();

	std::cout << (size_t)finaldfa.GetStartId() << std::endl;
	for (size_t i = 0; i < finaldfa.Size(); ++i)
	{
		if (finaldfa[i].GetFlag() & CDfaRow::TERMINAL)
		{
			std::cout << i << std::endl;
		}
	}
	outPutDfa(finaldfa, "..\\..\\output\\test.txt");

	system("pause");
}