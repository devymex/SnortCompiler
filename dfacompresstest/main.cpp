#include <iostream>
#include <hwprj\groupres.h>
#include <hwprj\dfaarray.h>
#include <hwprj\dfacompress.h>

int main()
{
	CGroupRes groupRes;
	groupRes.ReadFromFile("F:\\huawei\\date\\FinalResult.cdt");
	std::cout << groupRes.GetDfaTable()[48].GetStartState()<<std::endl;
	DfaCompress(groupRes.GetDfaTable()[48]);
	groupRes.GetDfaTable()[48].Dump("F:\\sun.txt");
	system("pause");
	return 0;
}