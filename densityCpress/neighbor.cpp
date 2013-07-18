#include "stdafx.h"
#include "densityCpress.h"

void GetNeighbors(ushort dfasize, double *disMatrix, double eps, ushort minPts,
	double *coreDis, std::vector<std::vector<ushort>> &neighbors)
{
	size_t i = 0, j = 0, temp = 0;
	
	for(i = 1; i < dfasize; ++i)
	{
		for(j = 0; j < i; ++j)
		{
			temp = (i - 1) * i / 2 + j;
			if(disMatrix[temp] < eps)
			{
				neighbors[i].push_back(j);
				neighbors[j].push_back(i);
			}
			/*if(i > j)
			{
				temp = i * (i - 1) / 2 + j;
			}
			else
			{
				temp = j * (j - 1) / 2 + i;
			}

			if(i != j)
			{
				if(disMatrix[temp] < eps)
				{
					neighbors[i].push_back(j);
				}
			}*/
		}
	}
	std::vector<double> order; 
	for(i = 0; i < neighbors.size(); ++i)
	{
		order.clear();
		if(neighbors[i].size() >= minPts - 1)
		{
			for(j = 0; j < neighbors[i].size(); ++j)
			{
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

