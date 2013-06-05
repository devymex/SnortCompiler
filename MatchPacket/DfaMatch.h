#include "MatchPkt.h"
#include <fstream>
#include "../hashmapping/hashmapping.h"

struct DFAMCH
{
	HASHRES &hashtable;
	CGROUPRes &mergedDfas;
	CResNew &dfas;
	std::ofstream &matchresult;
};