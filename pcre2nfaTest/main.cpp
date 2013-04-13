#include <iostream>
#include "../pcre2nfa/pcre2nfa.h"

void main()
{
	const char* a = "/^BM/sm";
	CNfa nfa;
	PcreToNFA(a, nfa);

	system("pause");
}
