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
	BYTE b[] = "fsdfsdkfBMdf";
	CStateSet StaSet;
	dfa.Process(b, sizeof(b), StaSet);
	CVectorNumber dfaIds;
	dfa.GetAcceptedId(2, dfaIds);
	system("pause");
}