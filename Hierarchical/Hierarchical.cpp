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

//返回出现次数最多的下表
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

//统计虚拟核,计算存储空间,每次一个行集
size_t StatisticVitualCore(const CDfa &oneDfa, ROWSET &rs)
{
	size_t n_size = rs.size();   //行集大小
	size_t n_statenum = oneDfa.Size();  //dfa状态数
	size_t* bary = new size_t[n_statenum]; //统计次数
	size_t* bcountary = new size_t[n_statenum]; //存储跳转状态不同的个数
	//for (size_t bcount = 0; bcount < n_statenum; bcount++) //init
	//{
	//	bcountary[bcount] = 0;
	//}
	std::fill(bcountary, bcountary + n_statenum, 0);

	//size_t n_dfacol = oneDfa[0].Size();//colnum
	size_t n_dfacol = oneDfa.GetGroupCount();//colnum

	ROWSET visrow;
	for (size_t col = 0; col < n_dfacol; col++) //dfa列
	{
		//for (size_t ba = 0; ba < n_statenum; ba++) //init
		//{
		//	bary[ba] = 0;
		//}
		std::fill(bary, bary + n_statenum, 0);

		for (size_t i = 0; i< n_size; i++) //统计出现次数
		{
			STATEID bt = oneDfa[(size_t)(rs[i])][col];
			bary[size_t(bt)]++;
		}
		STATEID maxindex = maxn(bary, n_statenum); //最多次数下标
		visrow.push_back((BYTE)(maxindex)); //该列虚拟核
		for (size_t i = 0; i< n_size; i++)   //存储跳转状态不同的个数
		{
			BYTE bt = oneDfa[(size_t)(rs[i])][col];
			if (visrow[col] != bt)
			{
				bcountary[i]++;
			}
		}
	}
	//计算内存大小
	size_t vsmem = n_size;
	for(size_t i = 0; i < n_size; i++)
	{
		vsmem += 2*bcountary[i];
	}

	delete[] bary;
	delete[] bcountary;

	return vsmem;

}

//最小割算法
void StoreWagner()
{
}

void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows)
{
	for (size_t i = 0; i < vecRows.size(); ++i)
	{
		size_t curRowval = StatisticVitualCore(oneDfa, vecRows[i]);

		GRAPH curGraph;
		BuildGraph(oneDfa, vecRows[i], curGraph);

		VECROWSET vecTmp;
		StoreWagner();

		size_t partRowval = StatisticVitualCore(oneDfa, vecTmp[0])
			+ StatisticVitualCore(oneDfa, vecTmp[1]);

		//比较两次存储空间大小是否减少
		//如果减少，则保留分割后的分割图
		//否则保留分割前的分割图
		if (curRowval > partRowval)
		{
			vecRows.push_back(vecTmp[1]);
			--i;
		}
	}

}
