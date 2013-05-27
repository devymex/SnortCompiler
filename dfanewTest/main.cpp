#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../mergedfanew/MergeDfanew.h"

void main()
{
	CNfa nfa1;

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

	//PcreToNFA("/^\\x3A[^\\r\\n]*IP\\s+Con.*X-Mailer\\x3A[^\\r\\n]*EBT\\s+Reporter.*Subjecwq\\x3A[^\\r\\n]*Vic/smi", nfa1, regChain);

	//PcreToNFA("/^\\x3A[^\\r\\n]*IP\\s+Con.*X-Mailer\\x3A[^\\r\\n]*EBT\\s+Reporter.*Subjecwq\\x3A[^\\r\\n]*Vic/s", nfa1, regChain);\

	nfa1.Resize(40);

	nfa1[0].AddDest(58, 1);
	for (size_t i = 0; i < 256; ++i)
	{
		nfa1[1].AddDest(i, 1);
	}
	nfa1[1].AddDest(73, 2);
	nfa1[2].AddDest(83, 3);

	outPut(nfa1, "..\\..\\output\\nfa.txt");

	std::cout << nfa1.Size() << std::endl;

	CDfanew dfa1;

	dfa1.FromNFA(nfa1, NULL, 0);

	dfa1.Minimize();

	std::cout << (size_t)dfa1.Size() << std::endl;

	outPutDfa(dfa1, "..\\..\\output\\dfa.txt");

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