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
    CNfa nfa;
	CDfanew dfa;
	nfa.Resize(3);
	nfa[0]['a'].PushBack(1);
	nfa[0]['b'].PushBack(2);
	nfa[1][EMPTY].PushBack(2);
	nfa[1]['c'].PushBack(3);
	nfa[2][EMPTY].PushBack(1);
	nfa[2]['c'].PushBack(3);

	dfa.FromNFA(nfa, NULL, false);
	system("pause");
}