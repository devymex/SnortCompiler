#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../mergedfanew/MergeDfanew.h"

void main()
{
	CTimer time;

	//const char* a1 = "/^(ab|bc)d(ef|g)/";
	//const char* a2 = "/^.{2}.*ab/si";
	//const char* a3 = "/^(a|b)abb/";
	//const char* a1 = "/^User-Agent\x3A[^\r\n]*URLBlaze/smi";
	//const char* a2 = "/^User-Agent\x3A[^\r\n]*iMeshBar/smi";
	//const char* a3 = "/^User-Agent\x3a[^\r\n]*AlertSpy/smi";
	//const char* a4 = "/^User-Agent\x3a[^\r\n]*EzReward/smi";
	//const char* a5 = "/^User-Agent\x3a[^\r\n].*USDR\d+/smi";
	const char* a1 = "/^abcdef/smi";
	const char* a2 = "/^abcijk/smi";
	CRegChain regChain;
	CNfa nfa1, nfa2;//, nfa3, nfa4, nfa5;

	//time.Reset();
	PcreToNFA(a1, nfa1, regChain);
	PcreToNFA(a2, nfa2, regChain);
	//PcreToNFA(a3, nfa3, regChain);
	//PcreToNFA(a4, nfa4, regChain);
	//PcreToNFA(a5, nfa5, regChain);
	//std::cout << "pcre to nfa: " << time.Reset() << std::endl;

	//outPut(nfa1, "F:\\cppProject\\huawei\\PreciseMatch\\output\\nfa1_opt.txt");
	//outPut(nfa2, "F:\\cppProject\\huawei\\PreciseMatch\\output\\nfa2_opt.txt");
	//outPut(nfa3, "F:\\cppProject\\huawei\\PreciseMatch\\output\\nfa3_opt.txt");

	CDfanew dfa1, dfa2;//, dfa3, dfa4, dfa5;
	dfa1.SetId(1);
	dfa2.SetId(2);
	//dfa3.SetId(3);
	//dfa4.SetId(4);
	//dfa5.SetId(5);

	time.Reset();
	dfa1.FromNFA(nfa1, NULL, 0);
	dfa2.FromNFA(nfa2, NULL, 0);
	//dfa3.FromNFA(nfa3, NULL, 0);
	//dfa4.FromNFA(nfa4, NULL, 0);
	//dfa5.FromNFA(nfa5, NULL, 0);
	//std::cout << "nfa to dfa: " << time.Reset() << std::endl;

	dfa1.Minimize();
	dfa2.Minimize();
	//dfa3.Minimize();
	//dfa4.Minimize();
	//dfa5.Minimize();
	//std::cout << "dfa min: " << time.Reset() << std::endl;

	//outPutDfa(dfa1, "F:\\cppProject\\huawei\\PreciseMatch\\output\\dfa1_opt.txt");
	//outPutDfa(dfa2, "F:\\cppProject\\huawei\\PreciseMatch\\output\\dfa2_opt.txt");
	//outPutDfa(dfa3, "F:\\cppProject\\huawei\\PreciseMatch\\output\\dfa3_opt.txt");
	//dfa1.printTerms();
	//dfa2.printTerms();
	//dfa3.printTerms();

	std::vector<CDfanew> dfas;
	dfas.push_back(dfa1);
	dfas.push_back(dfa2);
	//dfas.push_back(dfa3);
	//dfas.push_back(dfa4);
	//dfas.push_back(dfa5);
	CDfanew lastdfa;

	time.Reset();
	NOrMerge(dfas, lastdfa);//已经进行了最小化操作
	std::cout << "merge dfa: " << time.Reset() << std::endl;

	outPutDfa(lastdfa, "F:\\cppProject\\huawei\\PreciseMatch\\output\\lastdfa_opt_1.txt");
	std::cout << "合并后终态：" << std::endl;
	lastdfa.printTerms();

	std::cout << ((size_t)dfa1.Size()) * ((size_t)dfa1.GetGroupCount()) << std::endl;
	std::cout << ((size_t)dfa2.Size()) * ((size_t)dfa2.GetGroupCount()) << std::endl;
	std::cout << ((size_t)lastdfa.Size()) * ((size_t)lastdfa.GetGroupCount()) << std::endl;
	std::cout << ((size_t)lastdfa.Size()) << std::endl;

	system("pause");
}