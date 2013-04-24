#include <iostream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"

void main()
{
	CDfanew dfa;
	BYTE pGroup[DFACOLSIZE];
	for (size_t i = 0; i < DFACOLSIZE; ++i)
	{
		pGroup[i] = 0;
	}
	pGroup[1] = 1;
	dfa.Init(pGroup);
}