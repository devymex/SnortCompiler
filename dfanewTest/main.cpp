#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../mergedfanew/MergeDfanew.h"

void main()
{
	CNfa nfa1;
	CNfa nfa2;

	//PcreToNFA("/^POSTxml version<methodCall><methodName></methodName><params><param><value><string></string></value></param><param><value><string>/", nfa1);
	//PcreToNFA("/^POSTxml version<methodCall><methodName></methodName><params><param><value><name>'\\,''\\x29\\x29\\x3Becho '_begin_\\x0A'\\x3Becho/", nfa2);
	
	CRegChain regChain;
	PcreToNFA("/^POST/s", nfa1, regChain);
	PcreToNFA("/xml version/s", nfa1, regChain);
	PcreToNFA("/<methodCall><methodName>/s", nfa1, regChain);
	PcreToNFA("/</methodName><params><param><value><string></string></value></param><param><value><string>/s", nfa1, regChain);

	PcreToNFA("/^POST/s", nfa2, regChain);
	PcreToNFA("/xml version/s", nfa2, regChain);
	PcreToNFA("/<methodCall><methodName>/s", nfa2, regChain);
	PcreToNFA("/</methodName><params><param><value><name>/s", nfa2, regChain);
	PcreToNFA("/'\\,''\\x29\\x29\\x3Becho '_begin_\\x0A'\\x3Becho/s", nfa2, regChain);

	CDfanew dfa1;
	CDfanew dfa2;

	dfa1.FromNFA(nfa1, NULL, 0);
	dfa2.FromNFA(nfa2, NULL, 0);

	dfa1.Minimize();
	dfa2.Minimize();

	std::vector<CDfanew> dfas(2);
	dfas[0] = dfa1;
	dfas[1] = dfa2;
	//dfas.push_back(dfa1);
	//dfas.push_back(dfa2);
	//std::cout << dfas[0].GetId() << std::endl;
	//std::cout << (size_t)dfas[0].GetStartId() << std::endl;
	//std::cout << (size_t)dfas[0].GetGroupCount() << std::endl;
	//for (size_t j = 0; j < DFACOLSIZE; ++j)
	//{
	//	std::cout << (size_t)dfas[0].GetOneGroup(j) << " ";
	//}
	//std::cout << std::endl;
	//dfas[0].printTerms();
	//std::cout << dfas[1].GetId() << std::endl;
	//std::cout << (size_t)dfas[1].GetStartId() << std::endl;
	//std::cout << (size_t)dfas[1].GetGroupCount() << std::endl;
	//for (size_t j = 0; j < DFACOLSIZE; ++j)
	//{
	//	std::cout << (size_t)dfas[1].GetOneGroup(j) << " ";
	//}
	//std::cout << std::endl;
	//dfas[1].printTerms();

	//outPutDfa(dfa1, "..\\..\\output\\dfa1.txt");
	//outPutDfa(dfa2, "..\\..\\output\\dfa2.txt");

	CDfanew lastdfa;

	NOrMerge(dfas, lastdfa);

	std::cout << ((size_t)dfa1.Size()) * ((size_t)dfa1.GetGroupCount()) << std::endl;
	std::cout << ((size_t)dfa2.Size()) * ((size_t)dfa2.GetGroupCount()) << std::endl;
	std::cout << ((size_t)lastdfa.Size()) * ((size_t)lastdfa.GetGroupCount()) << std::endl;
	std::cout << ((size_t)lastdfa.Size()) << std::endl;

	system("pause");
}