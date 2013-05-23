#include "stdafx.h"
#include "hashmapping.h"
#include "../grouping/grouping.h"

size_t hash(const SIGNATURE &oneSig)
{
	return oneSig % 15991;
}

HASHMAPPINGSC void Mapping()
{
	CGROUPRes groupRes;
	groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut(2).cdt"));
	groupRes.GetGroups().Sort();

	size_t count = 0;
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		if (groupRes.GetGroups()[i].vecSigs.size() == 1)
			++count;
	}
	std::cout << count << std::endl;

	std::map<size_t, std::vector<size_t>> hashMap;
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		if (groupRes.GetGroups()[i].vecSigs.size() == 1)
		{
			hashMap[groupRes.GetGroups()[i].vecSigs[0]].push_back(i);
		}
	}
	std::cout << hashMap.size() << std::endl;
}
