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
size_t maxn(size_t* bary,int size)
{
	size_t n_max = bary[0]; 
	for (int i = 1; i < size; i++)
	{
		if (n_max < bary[i])
		{
			n_max = bary[i];
		}
	}
	return n_max;
	
}


//统计虚拟核 ,计算存储空间,每次一个行集
size_t StatisticVitualCore(const CDfa oneDfa,ROWSET rs)
{
	size_t n_size = rs.size(); 
	size_t n_statenum = oneDfa.Size();
	size_t* bary = new size_t[n_statenum];
	size_t* bcountary = new size_t[n_statenum];
	for (size_t b = 0; b < n_statenum; b++)
		{
			bary[b] = 0;
		}
	size_t n_dfacol = oneDfa[0].Size();//colnum
	VISUALROW visrow;
	for (size_t c = 0; c < n_dfacol; c++)
	{
		for (size_t b = 0; b < n_statenum; b++)
		{
			bary[b] = 0;
		}
		for (size_t i = 0; i< n_size; i++)
		{
			BYTE bt = oneDfa[(size_t)(rs[i])][c];
			bary[size_t(bt)]++;
			size_t maxnum = maxn(bary, n_statenum);
			visrow.push_back((BYTE)maxnum);
			for (size_t bc = 0; bc < n_size; bc++)
			{
				if (maxnum != bt)
				{
					bcountary[bc]++;
				}
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
