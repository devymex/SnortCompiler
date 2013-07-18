#include "Hierarchical.h"

void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph)
{
	size_t nRow = rows.size();
	size_t nCol = oneDfa.GetGroupCount();
	graph.resize(nRow * nCol);

	for (size_t i = 0; i < nRow; ++i)
	{
		for (size_t j = i + 1; j < nRow; ++j)
		{
			size_t nEqualCnt = 0;
			for (size_t k = 0; k < nCol; ++k)
			{
				if (oneDfa[rows[i]][k] == oneDfa[rows[j]][k])
				{
					++nEqualCnt;
				}
			}
			graph[i * nCol + j] = graph[j * nCol + i] = nEqualCnt / nRow;			 
		}
	}
}


void SearchConSubGraph(const GRAPH &graph, VECROWSET &vecRows)
{
}

//ͳ�������
void StatisticVitualCore()
{
}

//����洢�ռ�
void CalCulateMemory()
{
}

//��С���㷨
void StoreWagner()
{
}

void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows)
{
	for (VECROWSET::iterator i = vecRows.begin(); i != vecRows.end(); ++i)
	{
		StatisticVitualCore();
		CalCulateMemory();
		GRAPH curGraph;
		BuildGraph(oneDfa, *i, curGraph);
		StoreWagner();
		StatisticVitualCore();
		CalCulateMemory();
		//�Ƚ����δ洢�ռ��С�Ƿ����
		//������٣������ָ��ķָ�ͼ
		//�������ָ�ǰ�ķָ�ͼ
	}

}
