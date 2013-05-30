#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../mergedfanew/MergeDfanew.h"

void main()
{
	CNfa nfa1, nfa2, nfa3;

	CRegChain regChain;
	//PcreToNFA("/^POST/s", nfa1, regChain);
	//PcreToNFA("/xml/s", nfa2, regChain);
	//PcreToNFA("/<method>/s", nfa3, regChain);

	PcreToNFA("/^POST/s", nfa1, regChain);
	PcreToNFA("/^POST/s", nfa2, regChain);
	PcreToNFA("/^POST/s", nfa3, regChain);
	
	CDfanew dfa1, dfa2, dfa3;
	dfa1.SetId(1);
	dfa2.SetId(2);
	dfa3.SetId(3);

	dfa1.FromNFA(nfa1, NULL, 0);
	dfa2.FromNFA(nfa2, NULL, 0);
	dfa3.FromNFA(nfa3, NULL, 0);

	outPutDfa(dfa1, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\dfa1.txt");
	outPutDfa(dfa2, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\dfa2.txt");
	outPutDfa(dfa3, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\dfa3.txt");

	CDfanew mergeDfa;
	std::vector<CDfanew> dfas;
	dfas.push_back(dfa1);
	dfas.push_back(dfa2);
	dfas.push_back(dfa3);
	NOrMerge(dfas, mergeDfa);
	mergeDfa.printTerms();
	outPutDfa(mergeDfa, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\mergeDfa2.txt");

	//dfas[0] = dfa1;
	//dfas[1] = dfa2;
	//NOrMerge(dfas, mergeDfa);
	//dfas[0] = mergeDfa;
	//dfas[1] = dfa3;
	//NOrMerge(dfas, mergeDfa);
	//mergeDfa.printTerms();
	//outPutDfa(mergeDfa, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\mergeDfa_1_2_3.txt");

	std::cout << (size_t)dfa1.Size() << std::endl;

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