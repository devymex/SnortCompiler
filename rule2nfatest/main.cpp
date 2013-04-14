#include "../pcre2nfa/pcre2nfa.h"
#include "../rule2nfa/rule2nfa.h"
#include <iostream>

void printNfa(CNfa &nfa)
{
	for (size_t j = 0; j < nfa.size(); ++j)
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
	CNfaRow nfarow1, nfarow2;
	nfarow1[0].PushBack(0);
	nfarow1[1].PushBack(1);
	nfarow2[1].PushBack(1);
	nfarow2[2].PushBack(2);
	CNfa nfa1, nfa2;
	nfa1.push_back(nfarow1);
	nfa1.push_back(nfarow2);
	nfa2.push_back(nfarow1);
	nfa2.push_back(nfarow2);
	
	printNfa(nfa1);
	printNfa(nfa2);

	CNfaChain nfaChain;
	nfaChain.PushBack(nfa1);
	nfaChain.PushBack(nfa2);

	CNfa lastnfa;
	SerializeNfa(nfaChain, lastnfa);

	printNfa(lastnfa);
	system("pause");

}