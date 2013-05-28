#include "stdafx.h"
#include "hashmapping.h"
#include "../grouping/grouping.h"
#include "../compilernew/compilernew.h"
#include "../dfanew/dfanew.h"
#include "../mergedfanew/MergeDfanew.h"

size_t hash(const SIGNATURE &oneSig)
{
	return oneSig % 16001;
}

HASHMAPPINGSC void Mapping()
{
	CGROUPRes groupRes;
	groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));

	std::ofstream fout("..\\..\\output\\test.txt");
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		if (groupRes.GetGroups()[i].vecSigs.size() == 1)
		{
			fout << groupRes.GetGroups()[i].vecSigs[0] << std::endl;
		}
	}
	fout.clear();
	fout.close();

	std::map<size_t, std::vector<SIGNATURE>> hashMap;
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		size_t min = groupRes.GetGroups().Size();
		bool flag = false;
		size_t idx;
		for (size_t j = 0; j < groupRes.GetGroups()[i].vecSigs.size(); ++j)
		{
			if (hashMap[hash(groupRes.GetGroups()[i].vecSigs[j])].size() == 0)
			{
				flag = true;
				hashMap[hash(groupRes.GetGroups()[i].vecSigs[j])].push_back(groupRes.GetGroups()[i].vecSigs[j]);
				break;
			}
			else
			{
				if (min > hashMap[hash(groupRes.GetGroups()[i].vecSigs[j])].size())
				{
					min = hashMap[hash(groupRes.GetGroups()[i].vecSigs[j])].size();
					idx = j;
				}
			}
		}
		if (!flag)
		{
			hashMap[hash(groupRes.GetGroups()[i].vecSigs[idx])].push_back(groupRes.GetGroups()[i].vecSigs[idx]);
		}
	}
	std::cout << groupRes.GetGroups().Size() << std::endl;
	std::cout << hashMap.size() << std::endl;
}
