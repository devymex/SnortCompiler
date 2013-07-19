#include "stdafx.h"
#include "densityCpress.h"

ushort GetNextObj1(std::vector<ushort> &orderSeeds, ROWOBJ *arrObj)
{		
	
	// 存储allObj数组的大小,此大小与orderSeeds大小等价
	size_t sizeArr = orderSeeds.size();

	// 递减排序
	std::sort(orderSeeds.begin(), orderSeeds.end(), SeedsSort(arrObj));

	ushort minIdx = *orderSeeds.begin();
	orderSeeds.erase(orderSeeds.begin());
	return minIdx;
}