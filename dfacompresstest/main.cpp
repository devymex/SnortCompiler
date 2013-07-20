#include <iostream>
#include <hwprj\groupres.h>
#include <hwprj\dfaarray.h>
#include <hwprj\dfacompress.h>

int main()
{
	CGroupRes groupRes;
	groupRes.ReadFromFile("F:\\huawei\\date\\FinalResult.cdt");
	ulong sumBytes = 0;
	DfaCompress(groupRes.GetDfaTable()[65], sumBytes);
	//for(int i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	//{
	//	DfaCompress(groupRes.GetDfaTable()[i], sumBytes);
	//}
	system("pause");
	return 0;
}