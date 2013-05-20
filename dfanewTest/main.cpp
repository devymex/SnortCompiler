#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../mergedfanew/MergeDfanew.h"

void main()
{
	/*CNfa nfa1,nfa2;
	std::vector<CDfanew> dfas;
	CDfanew dfa1, dfa2, lastdfa;
	const char* a1 = "/^abcd/";
	const char* a2 = "/^efgh/";
	PcreToNFA(a1, nfa1);
	PcreToNFA(a2, nfa2);
	dfa1.FromNFA(nfa1, NULL, 0, false);
	dfa2.FromNFA(nfa2, NULL, 0, false);
	dfas.push_back(dfa1);
	dfas.push_back(dfa2);
	NOrMerge(dfas,lastdfa);

	for (size_t i = 0; i < lastdfa.Size(); ++i)
	{
		std::cout << i << ":";
		for (size_t j = 0; j < lastdfa.GetGroupCount(); ++j)
		{
			if(lastdfa[i][j] != BYTE(-1))
			{
				std::cout << "(" << j << "," << (size_t)lastdfa[i][j] << ")";
			}
		}
		std::cout << std::endl;
	}

	system("pause");*/

	//const char* a1 = "/^(ab|bc)def(ab|cd)/";
	const char* a1 = "/^(ab|bc)d(ef|g)/";
	const char* a2 = "/^.{2}.*ab/si";
	const char* a3 = "/^(a|b)abb/";
	CNfa nfa1;
	CNfa nfa2;
	CNfa nfa3;
	CRegChain regchain1;
	CRegChain regchain2;
	CRegChain regchain3;
	PcreToNFA(a1, nfa1, regchain1);
	PcreToNFA(a2, nfa2, regchain2);
	PcreToNFA(a3, nfa3, regchain3);

	outPut(nfa1, "F:\\cppProject\\huawei\\PreciseMatch\\output\\nfa1_origin.txt");
	outPut(nfa2, "F:\\cppProject\\huawei\\PreciseMatch\\output\\nfa2_origin.txt");
	outPut(nfa3, "F:\\cppProject\\huawei\\PreciseMatch\\output\\nfa3_origin.txt");


	CDfanew dfa1;
	CDfanew dfa2;
	CDfanew dfa3;
	
	CTimer nfa2dfatime;//用于测试
	nfa2dfatime.Reset();//用于测试
	dfa1.FromNFA(nfa1, NULL, 0);
	dfa2.FromNFA(nfa2, NULL, 0);
	dfa3.FromNFA(nfa3, NULL, 0);
	std::cout << "nfa2dfastime: " << nfa2dfatime.Reset() << std::endl;//用于测试
	//outPutDfa(dfa1, "F:\\cppProject\\huawei\\PreciseMatch\\output\\dfa1_test.txt");

	dfa1.Minimize();
	dfa2.Minimize();
	dfa3.Minimize();
	std::cout << "dfasMin: " << nfa2dfatime.Reset() << std::endl;//用于测试

	outPutDfa(dfa1, "F:\\cppProject\\huawei\\PreciseMatch\\output\\dfa1_origin.txt");
	outPutDfa(dfa2, "F:\\cppProject\\huawei\\PreciseMatch\\output\\dfa2_origin.txt");
	outPutDfa(dfa3, "F:\\cppProject\\huawei\\PreciseMatch\\output\\dfa3_origin.txt");

	std::vector<CDfanew> dfas;
	dfas.push_back(dfa1);
	dfas.push_back(dfa2);
	dfas.push_back(dfa3);
	CDfanew lastdfa;

	nfa2dfatime.Reset();//用于测试
	NOrMerge(dfas, lastdfa);
	std::cout << "merge dfas time: " << nfa2dfatime.Reset() << std::endl;//用于测试

	lastdfa.Minimize();
	outPutDfa(lastdfa, "F:\\cppProject\\huawei\\PreciseMatch\\output\\lastdfa_origin.txt");


	//std::vector<std::vector<BYTE>> allStr;
	//GetDfaSig(dfa, allStr);


	//BYTE b[] = "fsdfsdkfBMdf";
	//CStateSet StaSet;
	//dfa.Process(b, sizeof(b), StaSet);
	//CVectorNumber dfaIds;
	//for (size_t i = 0; i < StaSet.Size(); ++i)
	//{
	//	dfa.GetAcceptedId(StaSet[i], dfaIds);
	//	for (size_t j = 0; j < dfaIds.Size(); ++j)
	//	{
	//		std::cout << dfaIds[j] << std::endl;
	//	}
	//}
	system("pause");
}