#include "stdafx.h"
#include "densityCpress.h"


class seedsSort
{
public:
	ROWOBJ *parrobj;
	seedsSort (ROWOBJ *p ) : parrobj(p)
	{}
	
	// �ݼ�����
	bool operator () (ushort arg1, ushort arg2)
	{
		return parrobj[arg1].reachDis > parrobj[arg2].reachDis;
	}
};

void GetNextObj(std::vector<ushort> &orderSeeds, ROWOBJ *arrObj, ROWOBJ &curobj)
{		
	
	// �洢allObj����Ĵ�С,�˴�С��orderSeeds��С�ȼ�
	size_t sizeArr = orderSeeds.size();

	// �ݼ�����
	std::sort(orderSeeds.begin(), orderSeeds.end(), seedsSort(arrObj));

	size_t minIdx = orderSeeds[sizeArr - 1];
	orderSeeds.pop_back();

	curobj = arrObj[minIdx];
	
}