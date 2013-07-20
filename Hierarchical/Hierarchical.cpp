#include "Hierarchical.h"

//由DFA表中的行集建无向图，每一行代表图中的一个结点，边的权值为DFA表中两行中相同元素占的比率
void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph)
{
	//DFA表行数，即图中的结点个数
	size_t nRow = rows.size();
	//DFA表列数
	size_t nCol = oneDfa.GetGroupCount();

	//申请（结点个数 * 结点个数）大小的矩阵，一维数组存储
	graph.resize(nRow * nRow);
	std::fill(graph.begin(), graph.end(), (float)0.0);

	//任意两行都需要计算相似比率，无向图只需计算下（上）三角
	for (size_t i = 0; i < nRow; ++i)
	{
		for (size_t j = i + 1; j < nRow; ++j)
		{
			//DFA表中两行相同元素的个数
			size_t nEqualCnt = 0;
			for (size_t k = 0; k < nCol; ++k)
			{
				if ((oneDfa[(STATEID)rows[i]][k] == oneDfa[(STATEID)rows[j]][k]))
				{
					++nEqualCnt;
				}
			}

			//相同元素占的比率，比率大于0.5，则认为无向图中对应两个结点有一条边，边权值为比率值
			float radio = (float)nEqualCnt / (float)nCol;
			if (radio > 0.5)
			{
				graph[i * nRow + j] = graph[j * nRow + i] = radio;			 
			}
		}
	}
}

//图的深度搜索算法
int DFS_Visit(const GRAPH &graph, size_t vex, ROWSET &flag, ROWSET &curRow)
{
	if (flag[vex] == 0)
	{
		flag[vex] = 1;
		curRow.push_back((BYTE)vex);
	}
	else
	{
		return 0;
	}

	size_t nVexCnt = sqrt(float(graph.size()));
	if (nVexCnt * nVexCnt != graph.size())
	{
		throw 0;
	}

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

//找出无向图中的连通子图，只记录子图对应的结点编号。采用图的深度搜索算法
void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows)
{
	/// 检查输入的矩阵是否为方阵，否则抛出异常
	size_t nVexCnt = sqrt(float(graph.size()));
	if (nVexCnt * nVexCnt != graph.size())
	{
		throw 0;
	}

	//访问标记，记录哪些结点已经被访问
	ROWSET visit;
	visit.resize(nVexCnt);
	std::fill(visit.begin(), visit.end(), 0);

	//从一个未被访问的结点开始深搜，子图结点编号集合存于vecRows
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
size_t maxn(size_t *bary, int size)
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
	size_t n_dfacol = oneDfa.GetGroupCount();//colnum

	size_t *bary = new size_t[n_statenum]; //统计次数
	size_t *bcountary = new size_t[n_statenum]; //存储跳转状态不同的个数
	std::fill(bcountary, bcountary + n_statenum, 0);


	ROWSET visrow;
	for (size_t col = 0; col < n_dfacol; col++) //dfa列
	{
		std::fill(bary, bary + n_statenum, 0);

		for (size_t i = 0; i< n_size; i++) //统计出现次数
		{
			size_t bt = oneDfa[(STATEID)(rs[i])][col];
			bary[bt]++;
		}

		BYTE maxindex = BYTE(maxn(bary, n_statenum)); //最多次数下标

		visrow.push_back(maxindex); //该列虚拟核

		for (size_t i = 0; i < n_size; i++)   //存储跳转状态不同的个数
		{
			BYTE bt = (BYTE)oneDfa[(STATEID)(rs[i])][col];
			if (visrow[col] != bt)
			{
				bcountary[i]++;
			}
		}
	}
	//计算内存大小,n_size表示每一行都存储对应虚拟行的编号，n_dfacol表示虚拟行大小
	size_t vsmem = n_size + n_dfacol;
	for(size_t i = 0; i < n_size; i++)
	{
		vsmem += 2 * bcountary[i];
	}

	delete[] bary;
	delete[] bcountary;

	return vsmem;
}

/// 执行MA排序。adjMat为节点的赋权邻接矩阵，nodes为合并节点的集合。
/// 每一个合并节点包括构成的所有原节点的编号，第一个元素为该合并节点在矩阵中的行号。
float MaxAdjSort(const GRAPH &adjMat, std::vector<ROWSET> &nodes)
{
	/// 检查输入的参数是否为空，否则抛出异常
	if (adjMat.empty() || nodes.empty())
	{
		throw 0;
	}

	/// 检查输入的矩阵是否为方阵，否则抛出异常
	size_t nRowCnt = sqrt(float(adjMat.size()));
	if (nRowCnt * nRowCnt != adjMat.size() || nRowCnt < nodes.size())
	{
		throw 0;
	}

	/// 如果nodes中的第一节点为空，则抛出异常
	if (nodes.front().empty())
	{
		throw 0;
	}

	/// sorted为排好序的节点集合，nodes为未进入排序集合的其它节点。
	/// 依次将nodes中的节点存入sorted即可。
	VECROWSET sorted;

	/// 先将nodes中的第一节点存入sorted
	sorted.push_back(nodes.front());
	nodes.erase(nodes.begin());

	/// fMaxWeight用来保存iMaxNode到sorted的边权重和
	float fMaxWeight = -1;

	/// 循环处理nodes中的所有节点，直到nodes为空
	for (; !nodes.empty(); )
	{
		/// iMaxNode用来保存nodes中与sorted边权重和最大的节点
		NODEARRAY_ITER iMaxNode;

		fMaxWeight = -1;

		/// 遍例nodes，找出入sorted边权重和最大的节点
		for (NODEARRAY_ITER i = nodes.begin(); i != nodes.end(); ++i)
		{
			/// 如果nodes中有节点为空，则抛出异常
			if (i->empty())
			{
				throw 0;
			}

			/// 对于当前nodes中的一个节点i，便例sorted，求出i与sorted的边权重之和。
			float fWeiSum = 0;
			for (NODEARRAY_ITER j = sorted.begin(); j != sorted.end(); ++j)
			{
				size_t iRow = i->front(), jRow = j->front();
				/// 在矩阵中找出从节点j到节点i的边权重，累加到fWeiSum中
				fWeiSum += adjMat[iRow * nRowCnt + jRow];
			}

			/// 如果节点i与sorted的边权重比当前最大值大，则更新最大值
			if (fWeiSum > fMaxWeight)
			{
				iMaxNode = i;
				fMaxWeight = fWeiSum;
			}
		}

		/// 将在nodes中找到的与sorted边权重和最大的节点加入sorted，并从nodes中删除
		sorted.push_back(*iMaxNode);
		nodes.erase(iMaxNode);
	}
	nodes = sorted;
	return fMaxWeight;
}

void StoreWagner(GRAPH &adjMat, ROWSET &part)
{
	/// 检查输入的矩阵是否为空，否则抛出异常
	if (adjMat.empty())
	{
		throw 0;
	}

	/// 检查输入的矩阵是否为方阵，否则抛出异常
	size_t nRowCnt = sqrt(float(adjMat.size()));
	if (nRowCnt * nRowCnt != adjMat.size())
	{
		throw 0;
	}

	/// 构造并初始化节点集合。每个节点均为合并节点，可存放一个或多个原节点。
	/// 合并节点中的第一个节点即为该合并节点在矩阵中的行号和列号。
	/// 最初情形为：每个合并节点都只有一个节点。
	VECROWSET nodes;
	for (size_t i = 0; i < nRowCnt; ++i)
	{
		nodes.push_back(ROWSET(1, i));
	}

	float fMinWeight = FLT_MAX;
	for (; nodes.size() > 1; )
	{
		/// 对当前节点进行MA排序，获取最后一个节点的权重和
		float fLastWeight = MaxAdjSort(adjMat, nodes);

		/// 如果这一割的权重和比当前最小的权重和小，则更新最小值
		if (fLastWeight < fMinWeight)
		{
			part.clear();
			for (NODEARRAY_ITER i = nodes.begin(); i != nodes.end() - 1; ++i)
			{
				part.insert(part.end(), i->begin(), i->end());
			}
			fMinWeight = fLastWeight;
		}

		/// 取出倒数第二个节点和最后一个节点的代表行号
		NODEARRAY_ITER iLast = nodes.end() - 1;
		NODEARRAY_ITER iLbo = iLast - 1;
		size_t nLastRow = (size_t)iLast->front();
		size_t nLboRow = (size_t)iLbo->front();

		/// 从Last到另一个节点i的边要加入到从Lbo到i的边权重中去
		for (size_t i = 0; i < nRowCnt; ++i)
		{
			/// 注意i不能等于Last也不能等于Lbo
			if (i != nLastRow && i != nLboRow)
			{
				adjMat[nLboRow * nRowCnt + i] += adjMat[nLastRow * nRowCnt + i];
				adjMat[i * nRowCnt + nLboRow] = adjMat[nLboRow * nRowCnt + i];
			}
		}

		/// 将最后一个节点并入倒数第二个节点，并将最后一个节点删除
		iLbo->insert(iLbo->end(), iLast->begin(), iLast->end());
		nodes.erase(iLast);
	}
}

//层次聚类过程，存储空间减少则将图划分（最小割方法），自适应划分
void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows)
{
	//遍历每个连通子图
	for (NODEARRAY_ITER i = vecRows.begin(); i != vecRows.end(); ++i)
	{
		//计算当前连通子图的DFA表存储空间
		size_t curRowval = StatisticVitualCore(oneDfa, *i);
		
		//由连通子图的节点编号建无向图
		GRAPH curGraph;
		BuildGraph(oneDfa, *i, curGraph);

		//最小割方法将连通子图划分，划分的子图节点编号存于partIdx
		ROWSET partIdx;
		StoreWagner(curGraph, partIdx);

		//得到连通子图的划分节点编号，对应于行集的下标，将对应行集安下标划分
		ROWSET partRow1, partRow2;
		for (size_t j = 0, k = 0; j < i->size(); ++j)
		{
			if (k < partIdx.size() && j == partIdx[k] )
			{
				partRow1.push_back((*i)[j]);
				++k;
			}
			else
			{
				partRow2.push_back((*i)[j]);
			}
		}

		//计算划分后的DFA表存储空间
		size_t partRowval = StatisticVitualCore(oneDfa, partRow1)
			+ StatisticVitualCore(oneDfa, partRow2);

		//如果划分后存储空间减少，则用划分行集替换当前行集
		if (curRowval > partRowval)
		{
			vecRows.erase(i);
			vecRows.push_back(partRow1);
			vecRows.push_back(partRow2);
			--i;
		}
	}

}

