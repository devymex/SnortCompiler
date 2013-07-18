#include <hwprj/dfa.h>

struct ROWOBJ
{
	double coreDis;
	double reachDis;
	byte dfaRowInd;
};

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
	byte *coreObjFlag, std::vector<std::vector<byte>> &neighbors)
{

}

void SetCoreDis(std::vector<std::vector<byte>> &neighbors, double eps, ROWOBJ &obj)
{

}

void Update(std::vector<byte> &neignbors, ROWOBJ obj, std::vector<ROWOBJ>)
{
}

ROWOBJ GetNextObj()
{
}
void ExpandClusterOrder(std::vector<std::vector<double>> &disMatrix, ROWOBJ &obj, double eps, 
	byte minPts, byte *coreObjFlag, byte *pProcessed, std::vector<std::vector<byte>> &neighbors, std::vector<byte> &orderObj)
{
	pProcessed[obj.dfaRowInd] = 1;
	if (coreObjFlag[obj.dfaRowInd] == 1)
	{
		std::vector<byte> &curNeis = neighbors[obj.dfaRowInd];
		orderObj.push_back(obj.dfaRowInd);
		SetCoreDis(neighbors, eps, obj);

		std::vector<ROWOBJ> orderSeeds;
		Update(curNeis, obj, orderSeeds);

		while (!orderSeeds.empty())
		{
			ROWOBJ curobj = GetNextObj(orderSeeds);
		}
	}
}

void OPTICS(std::vector<std::vector<double>> &disMatrix, byte *pProcessed,
	double eps, byte minPts, std::vector<ROWOBJ> &orderObj)
{
	byte coreObjFlag[256];
	std::memset(coreObjFlag, 0, sizeof(coreObjFlag));
	std::vector<std::vector<byte>> neighbors;
	GetNeighbors(disMatrix, eps, minPts, coreObjFlag, neighbors);

	for (size_t rownum = 0; rownum < disMatrix.size(); ++rownum)
	{
		if (pProcessed[rownum] == 1)
		{
			continue;
		}


	}
}