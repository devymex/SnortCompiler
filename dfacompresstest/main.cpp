#include <iostream>
#include <hwprj\groupres.h>
#include <hwprj\dfaarray.h>

int main()
{
	CGroupRes groupRes;
	groupRes.ReadFromFile("F:\\cppProject\\huawei\\PreciseMatch\\CompressTest\\FinalResult.cdt");
	std::cout << groupRes.GetDfaTable().Size() << std::endl;

	system("pause");
	return 0;
}