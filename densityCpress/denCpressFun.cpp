#include "stdafx.h"
#include <hwprj/dfa.h>
#include "densityCpress.h"

void SetDistance(CDfa dfa, double *disMatrix)
{
	// len干啥用？
	ushort len = 0;
	for (size_t i = 0; i < dfa.Size(); ++i)
	{
		for (size_t j = 0; j < i; ++j)
		{
			CDfaRow &rowi = dfa[i];
			CDfaRow &rowj = dfa[j];
			size_t diffnum = 0;

			for (size_t ranknum = 0; ranknum < rowi.Size(); ++ranknum)
			{
				if (rowi[ranknum] != rowj[ranknum])
				{
					++diffnum;
				}
			}

			disMatrix[len] = diffnum / dfa.Size();
			++len;
		}
	}
}

//求得各个对象的邻居，并标记该对象是否为核心对象
void GetNeighbors(ushort dfasize, double *disMatrix, double eps, ushort minPts,
				double *coreDis, std::vector<std::vector<ushort>> &neighbors)
{	
	for(size_t i = 1; i < dfasize; ++i)
	{
		for(size_t j = 0; j < i; ++j)
		{
			size_t temp = (i - 1) * i / 2 + j;
			if(disMatrix[temp] < eps)
			{
				neighbors[i].push_back(j);
				neighbors[j].push_back(i);
			}
		}
	}
	std::vector<double> order; 
	for(size_t i = 0; i < neighbors.size(); ++i)
	{
		order.clear();
		if(neighbors[i].size() >= minPts - 1)
		{
			for(size_t j = 0; j < neighbors[i].size(); ++j)
			{
				size_t temp;
				if(i > neighbors[i][j])
				{
					temp = i * (i - 1) / 2 + neighbors[i][j];
				}
				else if(i < neighbors[i][j])
				{
					temp = neighbors[i][j] * (neighbors[i][j] - 1) / 2 + i;
				}
				
				order.push_back(disMatrix[i]);
			}
			std::sort(order.begin(), order.end());
			coreDis[i] = order[minPts - 2];
		}
		else
		{
			coreDis[i] = 0;
		}
	}
}

double Distance (ushort row1, ushort row2, double *disMatrix)
{
	ushort pos = 0;
	if (row1 > row2)
	{
		pos = (row1 * (row1 - 1)) / 2 + row2;
	}
	else if (row1 < row2)
	{
		pos = (row2 * (row2 - 1)) / 2 + row1;
	}
	return disMatrix[pos];
}

template <typename T>
double Max(T d1, T d2)
{
	if (d1 >= d2)
	{
		return d1;
	}
	else
	{
		return d2;
	}
}

void Update(std::vector<ushort> &neighbors, ROWOBJ &curobj, byte *pProcessed, 
			double *disMatrix, ROWOBJ *allObjs, std::vector<ushort> &orderSeeds)
{
	double cdis = curobj.coreDis;
	for (std::vector<ushort>::iterator iter = neighbors.begin(); iter != neighbors.end(); ++iter)
	{
		if (pProcessed[*iter] == 0)
		{
			ROWOBJ &neiObj = allObjs[*iter];
			double newRdis = Max(cdis, Distance(curobj.dfaRowInd, *iter, disMatrix));
			if (neiObj.reachDis == 0)
			{
				neiObj.reachDis = newRdis;
				orderSeeds.push_back(*iter);
			}
			else
			{
				if (newRdis < neiObj.reachDis)
				{
					neiObj.reachDis = newRdis;
				}
			}
		}
	}
}

ushort GetNextObj(std::vector<ushort> &orderSeeds, ROWOBJ *arrObj)
{		
	
	// 存储allObj数组的大小,此大小与orderSeeds大小等价
	size_t sizeArr = orderSeeds.size();

	// 递减排序
	std::sort(orderSeeds.begin(), orderSeeds.end(), SeedsSort(arrObj));

	ushort minIdx = *orderSeeds.begin();
	orderSeeds.erase(orderSeeds.begin());
	return minIdx;
}

void ExpandClusterOrder(ROWOBJ &obj, double eps, ushort minPts,byte *pProcessed, double *disMatrix, double *coreDis, 
						ROWOBJ *allObjs, std::vector<std::vector<ushort>> &neighbors, std::vector<ushort> &orderObj)
{
	pProcessed[obj.dfaRowInd] = 1;
	orderObj.push_back(obj.dfaRowInd);
	if (coreDis[obj.dfaRowInd] != 0)
	{
		std::vector<ushort> &curNeis = neighbors[obj.dfaRowInd];

		std::vector<ushort> orderSeeds;
		Update(curNeis, obj, pProcessed, disMatrix, allObjs, orderSeeds);

		while (!orderSeeds.empty())
		{
			ROWOBJ &curobj = allObjs[GetNextObj(orderSeeds, allObjs)];
			pProcessed[curobj.dfaRowInd] = 1;
			orderObj.push_back(curobj.dfaRowInd);

			if (coreDis[curobj.dfaRowInd] != 0)
			{
				curobj.coreDis = coreDis[curobj.dfaRowInd];
				std::vector<ushort> &neis = neighbors[curobj.dfaRowInd];
				Update(neis, curobj, pProcessed, disMatrix, allObjs, orderSeeds);
			}
		}
	}
}

void OPTICS(CDfa &dfa, double *disMatrix, double eps, ushort minPts, std::vector<ushort> &orderObj)
{
	double coreDis[256];
	std::memset(coreDis, 0, sizeof(coreDis));

	std::vector<std::vector<ushort>> neighbors;
	neighbors.resize(dfa.Size());

	GetNeighbors(dfa.Size(), disMatrix, eps, minPts, coreDis, neighbors);

	byte pProcessed[256];
	ROWOBJ allObjs[256];
	for (ushort i= 0; i < 256; ++i)
	{
		allObjs[i].coreDis = 0;
		allObjs[i].reachDis = 0;
		allObjs[i].dfaRowInd = i;
	}

	for (size_t rownum = 0; rownum < dfa.Size(); ++rownum)
	{
		if (pProcessed[rownum] == 1)
		{
			continue;
		}

		ROWOBJ &obj = allObjs[rownum];
		obj.coreDis = coreDis[rownum];
		ExpandClusterOrder(obj, eps, minPts, pProcessed, disMatrix, coreDis, allObjs, neighbors, orderObj);
	}
}