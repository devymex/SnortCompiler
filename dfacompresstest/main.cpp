#include <iostream>
#include <hwprj\groupres.h>
#include <hwprj\dfaarray.h>
#include <hwprj\dfacompress.h>

int main()
{
	CGroupRes groupRes;
	groupRes.ReadFromFile("F:\\cppProject\\huawei\\PreciseMatch\\CompressTest\\FinalResult.cdt");
	
	DfaCompress(groupRes.GetDfaTable()[5]);
	system("pause");
	return 0;
}