#include <hwprj/dfa.h>
#include "densityCpress.h"

//struct ROWOBJ
//{
//	double coreDis;
//	double reachDis;
//	byte dfaRowInd;
//};
//
void SetDistance(CDfa dfa, std::vector<std::vector<double>> &disMatrix)
{
	//给距离矩阵分配空间
	disMatrix.resize(dfa.Size());
	for (size_t i = 0; i < dfa.Size(); ++i)
	{
		disMatrix[i].resize(dfa.Size());
	}

	for (size_t i = 0; i < dfa.Size(); ++i)
	{
		for (size_t j = i + 1; j < dfa.Size(); ++j)
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

			disMatrix[i][j] = diffnum / dfa.Size();
			disMatrix[j][i] = diffnum / dfa.Size();
		}
	}
}

//求得各个对象的邻居，并标记该对象是否为核心对象
void GetNeighbors(std::vector<std::vector<double>> &disMatrix, double eps, byte minPts,
	byte *coreDis, std::vector<std::vector<byte>> &neighbors)
{

}

void Update(std::vector<byte> &neignbors, ROWOBJ obj, std::vector<ROWOBJ>)
{
}

void GetNextObj(std::vector<ROWOBJ> &orderSeeds, ROWOBJ &curobj)
{
}

void ExpandClusterOrder(ROWOBJ &obj, double eps, byte minPts,byte *pProcessed, 
						byte *coreDis, std::vector<std::vector<byte>> &neighbors, std::vector<ROWOBJ> &orderObj)
{
	pProcessed[obj.dfaRowInd] = 1;
	if (coreDis[obj.dfaRowInd] == 1)
	{
		std::vector<byte> &curNeis = neighbors[obj.dfaRowInd];
		orderObj.push_back(obj);
		obj.coreDis = coreDis[obj.dfaRowInd];

		std::vector<ROWOBJ> orderSeeds;
		Update(curNeis, obj, orderSeeds);

		while (!orderSeeds.empty())
		{
			ROWOBJ curobj;
			GetNextObj(orderSeeds, curobj);
		}
	}
}

void OPTICS(CDfa &dfa, std::vector<std::vector<double>> &disMatrix,
	double eps, byte minPts, std::vector<ROWOBJ> &orderObj)
{
	byte coreDis[256];
	std::memset(coreDis, 0, sizeof(coreDis));

	std::vector<std::vector<byte>> neighbors;
	GetNeighbors(disMatrix, eps, minPts, coreDis, neighbors);

	byte pProcessed[256];
	for (size_t rownum = 0; rownum < dfa.Size(); ++rownum)
	{
		if (pProcessed[rownum] == 1)
		{
			continue;
		}


	}
}