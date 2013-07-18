#include <hwprj/dfa.h>
#include "densityCpress.h"

//struct ROWOBJ
//{
//	double coreDis;
//	double reachDis;
//	byte dfaRowInd;
//};
//
void SetDistance(CDfa dfa, double *disMatrix)
{
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

}

double Distance (ushort row1, ushort row2, double *disMatrix)
{
	ushort pos = 0;
	if (row1 > row2)
	{
		pos = (row1 * (row1 - 1)) / 2 + row2;
		return disMatrix[pos];
	}
	else if (row1 < row2)
	{
		pos = (row2 * (row2 - 1)) / 2 + row1;
		return disMatrix[pos];
	}
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
			double *disMatrix, std::vector<ROWOBJ> &orderSeeds)
{
	double cdis = curobj.coreDis;
	for (std::vector<ushort>::iterator iter = neighbors.begin(); iter != neighbors.end(); ++iter)
	{
		if (pProcessed[*iter] == 0)
		{
			double newCdis = Max(cdis, Distance(curobj.dfaRowInd, *iter, disMatrix));
		}
	}
}

void GetNextObj(std::vector<ROWOBJ> &orderSeeds, ROWOBJ &curobj)
{
}

void ExpandClusterOrder(ROWOBJ &obj, double eps, ushort minPts,byte *pProcessed, 
						double *coreDis, std::vector<std::vector<ushort>> &neighbors, std::vector<ROWOBJ> &orderObj)
{
	pProcessed[obj.dfaRowInd] = 1;
	orderObj.push_back(obj);
	if (coreDis[obj.dfaRowInd] != 1)
	{
		std::vector<ushort> &curNeis = neighbors[obj.dfaRowInd];
		//obj.coreDis = coreDis[obj.dfaRowInd];

		std::vector<ROWOBJ> orderSeeds;
		Update(curNeis, obj, orderSeeds);

		while (!orderSeeds.empty())
		{
			ROWOBJ curobj;
			GetNextObj(orderSeeds, curobj);
			pProcessed[curobj.dfaRowInd] = 1;
			orderObj.push_back(curobj);

			if (coreDis[curobj.dfaRowInd] != 0)
			{
				curobj.coreDis = coreDis[curobj.dfaRowInd];
				curNeis = neighbors[curobj.dfaRowInd];
				//Update(curNeis, curobj, orderSeeds);
			}
		}
	}
}

void OPTICS(CDfa &dfa, double *disMatrix,
	double eps, ushort minPts, std::vector<ROWOBJ> &orderObj)
{
	double coreDis[256];
	std::memset(coreDis, 0, sizeof(coreDis));

	std::vector<std::vector<ushort>> neighbors;
	GetNeighbors(dfa.Size(), disMatrix, eps, minPts, coreDis, neighbors);

	byte pProcessed[256];
	for (size_t rownum = 0; rownum < dfa.Size(); ++rownum)
	{
		if (pProcessed[rownum] == 1)
		{
			continue;
		}

		ROWOBJ obj;
		obj.dfaRowInd = rownum;
		obj.coreDis = coreDis[rownum];
		ExpandClusterOrder(obj, eps, minPts, pProcessed, coreDis, neighbors, orderObj);
	}
}