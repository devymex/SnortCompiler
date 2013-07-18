#include "stdafx.h"
#include "densityCpress.h"


class seedsSort
{
public:
	ROWOBJ *parrobj;
	seedsSort (ROWOBJ *p ) : parrobj(p)
	{}
	
	// 递减排序
	bool operator () (ushort arg1, ushort arg2)
	{
		return parrobj[arg1].reachDis > parrobj[arg2].reachDis;
	}
};

void GetNextObj(std::vector<ushort> &orderSeeds, ROWOBJ *arrObj, ROWOBJ &curobj)
{		
	
	// 存储allObj数组的大小,此大小与orderSeeds大小等价
	size_t sizeArr = orderSeeds.size();

	// 递减排序
	std::sort(orderSeeds.begin(), orderSeeds.end(), seedsSort(arrObj));

	size_t minIdx = orderSeeds[sizeArr - 1];
	orderSeeds.pop_back();

	curobj = arrObj[minIdx];
	
}