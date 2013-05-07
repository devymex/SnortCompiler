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

	const char* a = "/^(abcd|cd)fe/";
	CNfa nfa;
	PcreToNFA(a, nfa);
	CDfanew dfa;
	dfa.FromNFA(nfa, NULL, 0);
	for (size_t i = 0; i < dfa.Size(); ++i)
	{
		std::cout << i << ":";
		for (size_t j = 0; j < dfa.GetGroupCount(); ++j)
		{
			if(dfa[i][j] != BYTE(-1))
			{
			std::cout << "(" << j << "," << (size_t)dfa[i][j] << ")";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	dfa.Minimize();
	for (size_t i = 0; i < dfa.Size(); ++i)
	{
		std::cout << i << ":";
		for (size_t j = 0; j < dfa.GetGroupCount(); ++j)
		{
			if(dfa[i][j] != BYTE(-1))
			{
			std::cout << "(" << j << "," << (size_t)dfa[i][j] << ")";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	GetDfaSig(dfa);


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