#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../pcre2nfa/pcre2nfa.h"

void main()
{
	const char* a = "/BM/";
	CNfa nfa;
	PcreToNFA(a, nfa);
	CDfanew dfa;
	dfa.FromNFA(nfa, NULL, 0);
	for (size_t i = 0; i < dfa.Size(); ++i)
	{
		std::cout << i << ":";
		for (size_t j = 0; j < dfa.GetGroupCount(); ++j)
		{
			std::cout << "(" << j << "," << (size_t)dfa[i][j] << ")";
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
			std::cout << "(" << j << "," << (size_t)dfa[i][j] << ")";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

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