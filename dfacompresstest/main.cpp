#include <iostream>
#include <hwprj\groupres.h>
#include <hwprj\dfaarray.h>
//#include <sstream>
#include <hwprj\compres.h>

int main()
{
	CGroupRes groupRes;
	groupRes.ReadFromFile("F:\\huawei\\date\\FinalResult.cdt");
	
	DfaCompress(groupRes.GetDfaTable()[5]);
	system("pause");
	return 0;
}