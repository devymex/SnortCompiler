#include <iostream>
#include <hwprj\groupres.h>
#include <hwprj\dfaarray.h>
#include <hwprj\dfacompress.h>

int main()
{
	CGroupRes groupRes;
	groupRes.ReadFromFile("F:\\huawei\\date\\FinalResult.cdt");
	std::cout << groupRes.GetDfaTable()[48].GetStartState()<<std::endl;
	ulong sumBytes = 0;
	for(int i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	{
		DfaCompress(groupRes.GetDfaTable()[i], sumBytes);
	}
	system("pause");
	return 0;
}