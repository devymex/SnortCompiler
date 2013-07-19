#include <iostream>
#include <hwprj\groupres.h>
#include <hwprj\dfaarray.h>
#include <hwprj\dfacompress.h>

int main()
{
	CGroupRes groupRes;
	groupRes.ReadFromFile("F:\\cppProject\\huawei\\PreciseMatch\\CompressTest\\FinalResult.cdt");
	ulong sumBytes = 0;
	DfaCompress(groupRes.GetDfaTable()[1529], sumBytes);
	//for(int i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	//{
	//	DfaCompress(groupRes.GetDfaTable()[i], sumBytes);
	//}
	system("pause");
	return 0;
}