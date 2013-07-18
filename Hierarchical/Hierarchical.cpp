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
				if (oneDfa[rows[i]][k] == oneDfa[rows[j]][k])
				{
					++nEqualCnt;
				}
			}
			double radio = (double)nEqualCnt / (double)nRow;
			if (radio > 0.5)
			{
				graph[i * nRow + j] = graph[j * nRow + i] = radio;			 
			}
		}
	}
}

int DFS_Visit(const GRAPH &graph, ROWSET &row, size_t vertex, ROWSET &flag)
{
	if (flag[vertex] == 0)
	{
		flag[vertex] = 1;
	}
	else
	{
		return 0;
	}

	for (size_t i = 0; i < row.size(); ++i)
	{
		if (graph[vertex * row.size() + i] > 0 &&
			flag[i] == 0)
		{
			DFS_Visit(graph, row, i, flag);
		}
	}

	return 1;
}

void SearchConnectSubgraph(const GRAPH &graph, ROWSET &curRow, VECROWSET &vecRows)
{
	ROWSET visit;
	visit.resize(curRow.size());
	std::fill(visit.begin(), visit.end(), 0);

	ROWSET nextRow;

	if (DFS_Visit(graph, curRow, 0, visit))
	{
		vecRows.push_back(ROWSET());
		for (size_t i = 0; i < visit.size(); ++i)
		{
			if (visit[i] != 0)
			{
				vecRows.back().push_back(curRow[i]);
			}
			else
			{
				nextRow.push_back(curRow[i]);
			}
		}
	}

	if (!nextRow.empty())
	{
		size_t nSize = nextRow.size();
		GRAPH newGraph(nSize * nSize);
		for (size_t i = 0; i < nSize; ++i)
		{
			for (size_t j = 0; j < nSize; ++j)
			{
				newGraph[i * nSize + j] = graph[nextRow[i] * curRow.size() + nextRow[j]];
			}
		}
		SearchConnectSubgraph(newGraph, nextRow, vecRows);
	}
}


size_t maxn(size_t* bary,int size)
{
	size_t n_max = 0; 
	for (int i = 1; i < size; i++)
	{
		if (bary[n_max] < bary[i])
		{
			n_max = i;
		}
	}
	return n_max;
	
}


//统计虚拟核 ,计算存储空间,每次一个行集
size_t StatisticVitualCore(const CDfa &oneDfa,ROWSET &rs)
{
	size_t n_size = rs.size(); 
	size_t n_statenum = oneDfa.Size();
	size_t* bary = new size_t[n_statenum];
	size_t* bcountary = new size_t[n_statenum];
	for (size_t bcount = 0; bcount < n_statenum; bcount++)
		{
			bcountary[bcount] = 0;
		}
	size_t n_dfacol = oneDfa[0].Size();//colnum
	VISUALROW visrow;
	for (size_t col = 0; col < n_dfacol; col++)
	{
		for (size_t ba = 0; ba < n_statenum; ba++)
		{
			bary[ba] = 0;
		}
		for (size_t i = 0; i< n_size; i++)
		{
			BYTE bt = oneDfa[(size_t)(rs[i])][col];
			bary[size_t(bt)]++;
		}
		size_t maxindex = maxn(bary, n_statenum);
		visrow.push_back((BYTE)(maxindex));
		for (size_t i = 0; i< n_size; i++)
		{
			BYTE bt = oneDfa[(size_t)(rs[i])][col];
			if (visrow[col] != bt)
			{
				bcountary[i]++;
			}
		}
	}
	size_t vsmem = n_size;
	for(size_t i = 0; i < n_size; i++)
	{
		vsmem += 2*bcountary[i];
	}
	return vsmem;
}

////计算存储空间
//void CalCulateMemory()
//{
//
//}
//
////最小割算法
//void StoreWagner()
//{
//}

//void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows)
//{
//	for (VECROWSET::iterator i = vecRows.begin(); i != vecRows.end(); ++i)
//	{
//		StatisticVitualCore();
//		//CalCulateMemory();
//		GRAPH curGraph;
//		BuildGraph(oneDfa, *i, curGraph);
//		StoreWagner();
//		StatisticVitualCore();
//		CalCulateMemory();
//		//比较两次存储空间大小是否减少
//		//如果减少，则保留分割后的分割图
//		//否则保留分割前的分割图
//	}
//
//}
