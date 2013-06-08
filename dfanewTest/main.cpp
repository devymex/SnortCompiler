#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../mergedfanew/MergeDfanew.h"

void main()
{
	CNfa nfa1, nfa2, nfa3;

	//const char* a1 = "/^User-Agent\x3A[^\r\n]*URLBlaze/smi";
	//const char* a2 = "/^User-Agent\x3A[^\r\n]*iMeshBar/smi";
	//const char* a3 = "/^User-Agent\x3a[^\r\n]*AlertSpy/smi";
	//const char* a4 = "/^User-Agent\x3a[^\r\n]*EzReward/smi";
	//const char* a5 = "/^User-Agent\x3a[^\r\n].*USDR\d+/smi";
	const char* a1 = "/^abcdef/smi";
	const char* a2 = "/^abcijk/smi";
	CRegChain regChain;
	//CNfa nfa1, nfa2, nfa3, nfa4, nfa5;
	//PcreToNFA("/xml/s", nfa2, regChain);
	//PcreToNFA("/<method>/s", nfa3, regChain);
	//PcreToNFA(a5, nfa5, regChain);

	PcreToNFA("/^POST/s", nfa1, regChain);
	PcreToNFA("/^POST/s", nfa2, regChain);
	PcreToNFA("/^POST/s", nfa3, regChain);
	//dfa5.SetId(5);
	
	CDfaNew dfa1, dfa2, dfa3;
	dfa1.SetId(1);
	dfa2.SetId(2);
	dfa3.SetId(3);

	dfa1.FromNFA(nfa1);
	dfa2.FromNFA(nfa2);
	dfa3.FromNFA(nfa3);
	//dfa5.Minimize();
	outPutDfa(dfa1, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\dfa1.txt");
	outPutDfa(dfa2, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\dfa2.txt");
	outPutDfa(dfa3, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\dfa3.txt");

	CDfaNew mergeDfa;
	std::vector<CDfaNew> dfas;
	dfas.push_back(dfa1);
	dfas.push_back(dfa2);
	dfas.push_back(dfa3);
	MergeMultipleDfas(dfas, mergeDfa);
	mergeDfa.printTerms();
	outPutDfa(mergeDfa, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\mergeDfa2.txt");

	//dfas[0] = dfa1;
	//dfas[1] = dfa2;
	//MergeMultipleDfas(dfas, mergeDfa);
	//dfas[0] = mergeDfa;
	//dfas[1] = dfa3;
	//MergeMultipleDfas(dfas, mergeDfa);
	//mergeDfa.printTerms();
	//outPutDfa(mergeDfa, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\mergeDfa_1_2_3.txt");
	for (size_t i = 0; i < dfa1.Size(); ++i)
	{
		std::cout << dfa1[i].GetFlag() << std::endl;
	}

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