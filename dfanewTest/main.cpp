#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../mergedfanew/MergeDfanew.h"

void main()
{
	CNfa nfa1, nfa2, nfa3;

	CRegChain regChain;
	PcreToNFA("/^POST/s", nfa1, regChain);
	PcreToNFA("/xml version/s", nfa2, regChain);
	PcreToNFA("/<methodCall><methodName>/s", nfa3, regChain);
	//PcreToNFA("/</methodName><params><param><value><string></string></value></param><param><value><string>/s", nfa1, regChain);

	//PcreToNFA("/^POST/s", nfa2, regChain);
	//PcreToNFA("/xml version/s", nfa2, regChain);
	//PcreToNFA("/<methodCall><methodName>/s", nfa2, regChain);
	//PcreToNFA("/</methodName><params><param><value><name>/s", nfa2, regChain);
	//PcreToNFA("/'\\,''\\x29\\x29\\x3Becho '_begin_\\x0A'\\x3Becho/s", nfa2, regChain);

	//PcreToNFA("/^\\x3A[^\\r\\n]*IP\\s+Con.*X-Mailer\\x3A[^\\r\\n]*EBT\\s+Reporter.*Subjecwq\\x3A[^\\r\\n]*Vic/smi", nfa1, regChain);

	//PcreToNFA("/^POST/s", nfa1, regChain);
	//PcreToNFA("/^POST/s", nfa2, regChain);
	
	CDfanew dfa1, dfa2, dfa3;
	dfa1.SetId(1);
	dfa2.SetId(2);
	dfa3.SetId(3);

	dfa1.FromNFA(nfa1, NULL, 0);
	dfa2.FromNFA(nfa2, NULL, 0);
	dfa3.FromNFA(nfa3, NULL, 0);

	//outPutDfa(dfa1, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\dfa1.txt");
	//outPutDfa(dfa2, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\dfa2.txt");

	std::vector<CDfanew> dfas;
	dfas.push_back(dfa1);
	dfas.push_back(dfa2);
	dfas.push_back(dfa3);
	CDfanew mergeDfa;
	NOrMerge(dfas, mergeDfa);

	outPutDfa(mergeDfa, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\mergeDfa2.txt");

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