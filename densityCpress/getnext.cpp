#include "stdafx.h"
#include "densityCpress.h"

ushort GetNextObj1(std::vector<ushort> &orderSeeds, ROWOBJ *arrObj)
{		
	
	// �洢allObj����Ĵ�С,�˴�С��orderSeeds��С�ȼ�
	size_t sizeArr = orderSeeds.size();

	// �ݼ�����
	std::sort(orderSeeds.begin(), orderSeeds.end(), SeedsSort(arrObj));

	ushort minIdx = *orderSeeds.begin();
	orderSeeds.erase(orderSeeds.begin());
	return minIdx;
}