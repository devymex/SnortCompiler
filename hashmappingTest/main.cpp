#include <iostream>
#include <tchar.h>
#include "../hashmapping/hashmapping.h"
#include "../grouping/grouping.h"

void main()
{
	CGROUPRes groupRes;
	HASHRES HashResMap;
	groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	HashMapping(groupRes, HashResMap);
	system("pause");
}
