#include "stdafx.h"
#include "densityCpress.h"

//void GetNeighbors(ushort dfasize, double *disMatrix, double eps, ushort minPts,
//	double *coreDis, std::vector<std::vector<ushort>> &neighbors)
//{	
//	for(size_t i = 1; i < dfasize; ++i)
//	{
//		for(size_t j = 0; j < i; ++j)
//		{
//			size_t temp = (i - 1) * i / 2 + j;
//			if(disMatrix[temp] < eps)
//			{
//				neighbors[i].push_back(j);
//				neighbors[j].push_back(i);
//			}
//		}
//	}
//	std::vector<double> order; 
//	for(size_t i = 0; i < neighbors.size(); ++i)
//	{
//		order.clear();
//		if(neighbors[i].size() >= minPts - 1)
//		{
//			for(size_t j = 0; j < neighbors[i].size(); ++j)
//			{
//				size_t temp;
//				if(i > neighbors[i][j])
//				{
//					temp = i * (i - 1) / 2 + neighbors[i][j];
//				}
//				else if(i < neighbors[i][j])
//				{
//					temp = neighbors[i][j] * (neighbors[i][j] - 1) / 2 + i;
//				}
//				
//				order.push_back(disMatrix[i]);
//			}
//			std::sort(order.begin(), order.end());
//			coreDis[i] = order[minPts - 2];
//		}
//		else
//		{
//			coreDis[i] = 0;
//		}
//	}
//}

