#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../mergedfanew/MergeDfanew.h"


	void printNfa(CNfa &nfa)
{
	for (size_t j = 0; j < nfa.Size(); ++j)
	{
		std::cout << j << ": ";
		for (size_t k = 0; k < CHARSETSIZE; ++k)
		{
			for (size_t l = 0; l < nfa[j][k].Size(); ++l)
			{
				std::cout << "(" << k << "," << nfa[j][k][l] << ")";
			}
		}
		std::cout << std::endl;
	}
}

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


	const char* a ="/^abcdefg/"
;
	CNfa nfa;

	PcreToNFA(a, nfa);
	//printNfa(nfa);
	CDfanew dfa;
	dfa.FromNFA(nfa, NULL, 0);
	outPutDfa(dfa, "F:\\cppProject\\huawei\\PreciseMatch\\input\\dfa3_after.txt");

	//dfa.printTerms();
	for (size_t i = 0; i < dfa.Size(); ++i)
	{
		std::cout << i << ":";
		for (size_t charNum = 0; charNum < 256; ++charNum)
		{
			size_t j = dfa.GetOneGroup(charNum);
			if(dfa[i][j] != BYTE(-1))
			{
			std::cout << "(" << charNum << "," << (size_t)dfa[i][j] << ")";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	dfa.Minimize();
	//for (size_t i = 0; i < dfa.Size(); ++i)
	//{
	//	std::cout << i << ":";
	//	for (size_t charNum = 0; charNum < 256; ++charNum)
	//	{
	//		size_t j = dfa.GetOneGroup(charNum);
	//		if(dfa[i][j] != BYTE(-1))
	//		{
	//		std::cout << "(" << charNum << "," << (size_t)dfa[i][j] << ")";
	//		}
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << std::endl;
	std::vector<std::vector<BYTE>> allStr;
	GetDfaSig(dfa, allStr);


	BYTE b[] = "fsdfsdkfBMdf";
	CStateSet StaSet;
	dfa.Process(b, sizeof(b), StaSet);
	CVectorNumber dfaIds;
	for (size_t i = 0; i < StaSet.Size(); ++i)
	{
		dfa.GetAcceptedId(StaSet[i], dfaIds);
		for (size_t j = 0; j < dfaIds.Size(); ++j)
		{
			std::cout << dfaIds[j] << std::endl;
		}
	}
	system("pause");
}