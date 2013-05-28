#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../mergedfanew/MergeDfanew.h"

void main()
{
	//CNfa nfa1;

	//const char* a1 = "/^User-Agent\x3A[^\r\n]*URLBlaze/smi";
	//const char* a2 = "/^User-Agent\x3A[^\r\n]*iMeshBar/smi";
	//const char* a3 = "/^User-Agent\x3a[^\r\n]*AlertSpy/smi";
	//const char* a4 = "/^User-Agent\x3a[^\r\n]*EzReward/smi";
	//const char* a5 = "/^User-Agent\x3a[^\r\n].*USDR\d+/smi";
	const char* a1 = "/^abcdef/smi";
	const char* a2 = "/^abcijk/smi";
	CRegChain regChain;
	CNfa nfa1, nfa2;//, nfa3, nfa4, nfa5;
	//PcreToNFA("/xml version/s", nfa1, regChain);
	//PcreToNFA("/<methodCall><methodName>/s", nfa1, regChain);
	//PcreToNFA("/</methodName><params><param><value><string></string></value></param><param><value><string>/s", nfa1, regChain);

	//time.Reset();
	//PcreToNFA("/xml version/s", nfa2, regChain);
	//PcreToNFA("/<methodCall><methodName>/s", nfa2, regChain);
	//PcreToNFA(a3, nfa3, regChain);
	//PcreToNFA(a4, nfa4, regChain);
	//PcreToNFA(a5, nfa5, regChain);

	//PcreToNFA("/^\\x3A[^\\r\\n]*IP\\s+Con.*X-Mailer\\x3A[^\\r\\n]*EBT\\s+Reporter.*Subjecwq\\x3A[^\\r\\n]*Vic/smi", nfa1, regChain);

	CDfanew dfa1, dfa2;//, dfa3, dfa4, dfa5;
	//dfa3.SetId(3);
	//dfa4.SetId(4);
	//dfa5.SetId(5);

	std::cout << nfa1.Size() << std::endl;
	//dfa3.FromNFA(nfa3, NULL, 0);
	//dfa4.FromNFA(nfa4, NULL, 0);
	//dfa5.FromNFA(nfa5, NULL, 0);

	//CDfanew dfa1;
	//dfa3.Minimize();
	//dfa4.Minimize();
	//dfa5.Minimize();

	dfa1.FromNFA(nfa1, NULL, 0);

	outPutDfa(dfa1, "..\\..\\output\\dfa.txt");
	//dfas.push_back(dfa3);
	//dfas.push_back(dfa4);
	//dfas.push_back(dfa5);

	dfa1.Minimize();

	std::cout << (size_t)dfa1.Size() << std::endl;

	outPutDfa(dfa1, "..\\..\\output\\dfa_after.txt");

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