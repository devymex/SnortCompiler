#include "../PMatchPkt/PMatchPkt.h"
#include <iostream>
#include <tchar.h>

void main(size_t argc, char* argv[])
{
	REGRULESMAP rulesmap;
	MchCompile(_T("../../input/allrules.rule"), &rulesmap);

	std::cout << rulesmap.sigmap.size();
}