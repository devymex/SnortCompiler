#include "stdafx.h"
#include "densityCpress.h"

ushort GetNextObj1(std::vector<ushort> &orderSeeds, ROWOBJ *allObj)
{			
	// µ›‘ˆ≈≈–Ú
	std::sort(orderSeeds.begin(), orderSeeds.end(), SeedsSort(allObj));

	ushort minIdx = *orderSeeds.begin();
	orderSeeds.erase(orderSeeds.begin());
	return minIdx;
}