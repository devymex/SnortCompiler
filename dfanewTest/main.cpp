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
	const char* a1 = "/^ab+m.{3}/";
	const char* a2 = "/^d.*ef/";
	CRegChain regChain;
	CNfa nfa1, nfa2;

	time.Reset();
	PcreToNFA(a1, nfa1, regChain);
	PcreToNFA(a2, nfa2, regChain);
	size_t nfa1size = nfa1.Size();
	//std::cout << "pcre to nfa: " << time.Reset() << std::endl;

	//outPut(nfa1, "F:\\cppProject\\huawei\\PreciseMatch\\output\\nfa1_opt.txt");
	//outPut(nfa2, "F:\\cppProject\\huawei\\PreciseMatch\\output\\nfa2_opt.txt");
	//outPut(nfa3, "F:\\cppProject\\huawei\\PreciseMatch\\output\\nfa3_opt.txt");

	CDfanew dfa1, dfa2;
	dfa1.SetId(1);
	dfa2.SetId(2);

	time.Reset();
	dfa1.FromNFA(nfa1, NULL, 0);
	size_t dfasize = dfa1.Size();
	dfa2.FromNFA(nfa2, NULL, 0);

	dfa1.Minimize();
	dfa2.Minimize();
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
	CDfanew lastdfa;

	time.Reset();
	NOrMerge(dfas, lastdfa);//已经进行了最小化操作
	std::cout << "merge dfa: " << time.Reset() << std::endl;

	outPutDfa(lastdfa, "F:\\cppProject\\huawei\\PreciseMatch\\output\\lastdfa_opt_1.txt");
	std::cout << "合并后终态：" << std::endl;
	lastdfa.printTerms();

	std::cout << ((size_t)dfa1.Size())  << std::endl;
	std::cout << ((size_t)dfa2.Size())  << std::endl;
	std::cout << ((size_t)lastdfa.Size())  << std::endl;
	std::cout << ((size_t)lastdfa.Size()) << std::endl;

	system("pause");
}