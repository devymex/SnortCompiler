#include "stdafx.h"
#include "hashmapping.h"
#include "../grouping/grouping.h"

HASHMAPPINGSC void Mapping()
{
	CGROUPRes groupRes;
	groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	std::map<size_t, std::vector<size_t>> hashMap;
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		for (size_t j = 0; j < groupRes.GetGroups()[i].vecSigs.size(); ++j)
		{
			if (hashMap[groupRes.GetGroups()[i].vecSigs[j] % 40000].size() == 0)
			{
				hashMap[groupRes.GetGroups()[i].vecSigs[j] % 40000].push_back(i);
			}
		}
	}
	std::cout << hashMap.size() << std::endl;
}
