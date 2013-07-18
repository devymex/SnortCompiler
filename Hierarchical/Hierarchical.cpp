#include "Hierarchical.h"

void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph)
{
	size_t nRow = rows.size();
	graph.resize(nRow * nRow);
	std::fill(graph.begin(), graph.end(), 0.0);

	for (size_t i = 0; i < nRow; ++i)
	{
		for (size_t j = i + 1; j < nRow; ++j)
		{
			size_t nEqualCnt = 0;
			for (size_t k = 0; k < oneDfa.GetGroupCount(); ++k)
			{
				if ((oneDfa[rows[i]][k] == oneDfa[rows[j]][k]))
				{
					++nEqualCnt;
				}
			}
			double radio = (double)nEqualCnt / (double)oneDfa.GetGroupCount();
			if (radio > 0.5)
			{
				graph[i * nRow + j] = graph[j * nRow + i] = radio;			 
			}
		}
	}
}

int DFS_Visit(const GRAPH &graph, size_t vex, ROWSET &flag, ROWSET &curRow)
{
	if (flag[vex] == 0)
	{
		flag[vex] = 1;
		curRow.push_back(vex);
	}
	else
	{
		return 0;
	}

	size_t nVexCnt = sqrt(graph.size());
	for (size_t i = 0; i < nVexCnt; ++i)
	{
		if (graph[vex * nVexCnt + i] > 0 &&
			flag[i] == 0)
		{
			DFS_Visit(graph, i, flag, curRow);
		}
	}

	return 1;
}

void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows)
{
	size_t nVexCnt = sqrt(graph.size());

	ROWSET visit;
	visit.resize(nVexCnt);
	std::fill(visit.begin(), visit.end(), 0);

	for (size_t i = 0; i < nVexCnt; ++i)
	{
		if (visit[i] == 0)
		{
			vecRows.push_back(ROWSET());
			DFS_Visit(graph, i, visit, vecRows.back());
		}
	}
}

//统计虚拟核
void StatisticVitualCore()
{
}

//计算存储空间
void CalCulateMemory()
{
}

//最小割算法
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
		//比较两次存储空间大小是否减少
		//如果减少，则保留分割后的分割图
		//否则保留分割前的分割图
	}

}
