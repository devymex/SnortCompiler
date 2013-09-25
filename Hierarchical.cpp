#include "Hierarchical.h"

size_t maxVal = 0;

////由DFA表中的行集建无向图，每一行代表图中的一个结点，边的权值为DFA表中两行中相同元素占的比率
//void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph)
//{
//	//DFA表行数，即图中的结点个数
//	size_t nRow = rows.size();
//	//DFA表列数
//	size_t nCol = oneDfa.GetGroupCount();
//
//	//申请（结点个数 * 结点个数）大小的矩阵，一维数组存储
//	graph.resize(nRow * nRow);
//	std::fill(graph.begin(), graph.end(), (float)0.0);
//
//	
//	//size_t	nThreshold = limit * nCol;
//	//for (; nThreshold < (float(nCol) / 2.0f); ++nThreshold);
//
//	//任意两行都需要计算相似比率，无向图只需计算下（上）三角
//	for (size_t i = 0; i < nRow; ++i)
//	{
//		for (size_t j = i + 1; j < nRow; ++j)
//		{
//			//DFA表中两行相同元素的个数
//			size_t nEqualCnt = 0;
//			for (size_t k = 0; k < nCol; ++k)
//			{
//				if ((oneDfa[(STATEID)rows[i]][k] == oneDfa[(STATEID)rows[j]][k]))
//				{
//					++nEqualCnt;
//				}
//			}
//
//			////相同元素占的比率，比率大于0.5，则认为无向图中对应两个结点有一条边，边权值为比率值
//			//if (nEqualCnt >= nThreshold)
//			//{
//				graph[i * nRow + j] = graph[j * nRow + i] = (float)nEqualCnt / (float)nCol;			 
//			//}
//		}
//	}
//}
//
////图的深度搜索算法
//int DFS_Visit(const GRAPH &graph, size_t vex, ROWSET &flag, ROWSET &curRow)
//{
//	if (flag[vex] == 0)
//	{
//		flag[vex] = 1;
//		curRow.push_back((BYTE)vex);
//	}
//	else
//	{
//		return 0;
//	}
//
//	size_t nVexCnt = sqrt(float(graph.size()));
//	if (nVexCnt * nVexCnt != graph.size())
//	{
//		throw 0;
//	}
//
//	for (size_t i = 0; i < nVexCnt; ++i)
//	{
//		if (graph[vex * nVexCnt + i] > limit &&
//			flag[i] == 0)
//		{
//			DFS_Visit(graph, i, flag, curRow);
//		}
//	}
//
//	return 1;
//}
//
////找出无向图中的连通子图，只记录子图对应的结点编号。采用图的深度搜索算法
//void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows)
//{
//	/// 检查输入的矩阵是否为方阵，否则抛出异常
//	size_t nVexCnt = sqrt(float(graph.size()));
//	if (nVexCnt * nVexCnt != graph.size())
//	{
//		throw 0;
//	}
//
//	//访问标记，记录哪些结点已经被访问
//	ROWSET visit;
//	visit.resize(nVexCnt);
//	std::fill(visit.begin(), visit.end(), 0);
//
//	//从一个未被访问的结点开始深搜，子图结点编号集合存于vecRows
//	for (size_t i = 0; i < nVexCnt; ++i)
//	{
//		if (visit[i] == 0)
//		{
//			vecRows.push_back(ROWSET());
//			DFS_Visit(graph, i, visit, vecRows.back());
//		}
//	}
//}

////返回出现次数最多的下表
//size_t maxn(size_t *bary, int size)
//{
//	size_t n_max = 0; 
//	for (int i = 1; i < size; i++)
//	{
//		if (bary[n_max] < bary[i])
//		{
//			n_max = i;
//		}
//	}
//	return n_max;	
//}

////统计虚拟核,计算存储空间,每次一个行集
//size_t StatisticVitualCore(const CDfa &oneDfa, ROWSET &rs, ROWSET &virtualRow)
//{
//	size_t n_size = rs.size();   //行集大小
//	size_t n_statenum = oneDfa.Size();  //dfa状态数
//	size_t n_dfacol = oneDfa.GetGroupCount();//colnum
//
//	size_t *bcountary = new size_t[n_statenum]; //存储跳转状态不同的个数
//	std::fill(bcountary, bcountary + n_statenum, 0);
//
//	for (size_t col = 0; col < n_dfacol; col++) //dfa列
//	{
//		std::map<STATEID, size_t> bary;
//
//		for (size_t i = 0; i< n_size; i++) //统计出现次数
//		{
//			++bary[oneDfa[STATEID(rs[i])][col]];
//		}
//
//		STATEID maxindex = bary.begin()->first;
//		size_t max = bary.begin()->second;
//		for (std::map<STATEID, size_t>::iterator i = bary.begin(); i != bary.end(); ++i)
//		{
//			if (i->second > max)
//			{
//				max = i->second;
//				maxindex = i->first;
//			}
//		}
//		virtualRow.push_back(maxindex); //该列虚拟核
//
//		for (size_t i = 0; i < n_size; i++)   //存储跳转状态不同的个数
//		{
//			size_t bt = oneDfa[(STATEID)(rs[i])][col];
//			if (virtualRow[col] != bt)
//			{
//				bcountary[i]++;
//			}
//		}
//	}
//	//计算内存大小,n_size表示每一行都存储对应虚拟行的编号，n_dfacol表示虚拟行大小
//	size_t vsmem = n_size + n_dfacol;
//	for(size_t i = 0; i < n_size; i++)
//	{
//		vsmem += 2 * bcountary[i];
//	}
//
//	delete[] bcountary;
//
//	return vsmem;
//}

//// 执行MA排序。adjMat为节点的赋权邻接矩阵，nodes为合并节点的集合。
//// 每一个合并节点包括构成的所有原节点的编号，第一个元素为该合并节点在矩阵中的行号。
//float MaxAdjSort(const GRAPH &adjMat, std::vector<ROWSET> &nodes)
//{
//	/// 检查输入的参数是否为空，否则抛出异常
//	if (adjMat.empty() || nodes.empty())
//	{
//		throw 0;
//	}
//
//	/// 检查输入的矩阵是否为方阵，否则抛出异常
//	size_t nRowCnt = sqrt(float(adjMat.size()));
//	if (nRowCnt * nRowCnt != adjMat.size() || nRowCnt < nodes.size())
//	{
//		throw 0;
//	}
//
//	/// 如果nodes中的第一节点为空，则抛出异常
//	if (nodes.front().empty())
//	{
//		throw 0;
//	}
//
//	/// sorted为排好序的节点集合，nodes为未进入排序集合的其它节点。
//	/// 依次将nodes中的节点存入sorted即可。
//	VECROWSET sorted;
//
//	/// 先将nodes中的第一节点存入sorted
//	sorted.push_back(nodes.front());
//	nodes.erase(nodes.begin());
//
//	/// fMaxWeight用来保存iMaxNode到sorted的边权重和
//	float fMaxWeight = -1;
//
//	/// 循环处理nodes中的所有节点，直到nodes为空
//	for (; !nodes.empty(); )
//	{
//		/// iMaxNode用来保存nodes中与sorted边权重和最大的节点
//		NODEARRAY_ITER iMaxNode;
//
//		fMaxWeight = -1;
//
//		/// 遍例nodes，找出入sorted边权重和最大的节点
//		for (NODEARRAY_ITER i = nodes.begin(); i != nodes.end(); ++i)
//		{
//			/// 如果nodes中有节点为空，则抛出异常
//			if (i->empty())
//			{
//				throw 0;
//			}
//
//			/// 对于当前nodes中的一个节点i，便例sorted，求出i与sorted的边权重之和。
//			float fWeiSum = 0;
//			for (NODEARRAY_ITER j = sorted.begin(); j != sorted.end(); ++j)
//			{
//				size_t iRow = i->front(), jRow = j->front();
//				/// 在矩阵中找出从节点j到节点i的边权重，累加到fWeiSum中
//				fWeiSum += adjMat[iRow * nRowCnt + jRow];
//			}
//
//			/// 如果节点i与sorted的边权重比当前最大值大，则更新最大值
//			if (fWeiSum > fMaxWeight)
//			{
//				iMaxNode = i;
//				fMaxWeight = fWeiSum;
//			}
//		}
//
//		/// 将在nodes中找到的与sorted边权重和最大的节点加入sorted，并从nodes中删除
//		sorted.push_back(*iMaxNode);
//		nodes.erase(iMaxNode);
//	}
//	nodes = sorted;
//	return fMaxWeight;
//}
//
//void StoreWagner(GRAPH &adjMat, ROWSET &part)
//{
//	/// 检查输入的矩阵是否为空，否则抛出异常
//	if (adjMat.empty())
//	{
//		throw 0;
//	}
//
//	/// 检查输入的矩阵是否为方阵，否则抛出异常
//	size_t nRowCnt = sqrt(float(adjMat.size()));
//	if (nRowCnt * nRowCnt != adjMat.size())
//	{
//		throw 0;
//	}
//
//	/// 构造并初始化节点集合。每个节点均为合并节点，可存放一个或多个原节点。
//	/// 合并节点中的第一个节点即为该合并节点在矩阵中的行号和列号。
//	/// 最初情形为：每个合并节点都只有一个节点。
//	VECROWSET nodes;
//	for (size_t i = 0; i < nRowCnt; ++i)
//	{
//		nodes.push_back(ROWSET(1, i));
//	}
//
//	float fMinWeight = FLT_MAX;
//	for (; nodes.size() > 1; )
//	{
//		/// 对当前节点进行MA排序，获取最后一个节点的权重和
//		float fLastWeight = MaxAdjSort(adjMat, nodes);
//
//		/// 如果这一割的权重和比当前最小的权重和小，则更新最小值
//		if (fLastWeight < fMinWeight)
//		{
//			part.clear();
//			for (NODEARRAY_ITER i = nodes.begin(); i != nodes.end() - 1; ++i)
//			{
//				part.insert(part.end(), i->begin(), i->end());
//			}
//			fMinWeight = fLastWeight;
//		}
//
//		/// 取出倒数第二个节点和最后一个节点的代表行号
//		NODEARRAY_ITER iLast = nodes.end() - 1;
//		NODEARRAY_ITER iLbo = iLast - 1;
//		size_t nLastRow = (size_t)iLast->front();
//		size_t nLboRow = (size_t)iLbo->front();
//
//		/// 从Last到另一个节点i的边要加入到从Lbo到i的边权重中去
//		for (size_t i = 0; i < nRowCnt; ++i)
//		{
//			/// 注意i不能等于Last也不能等于Lbo
//			if (i != nLastRow && i != nLboRow)
//			{
//				adjMat[nLboRow * nRowCnt + i] += adjMat[nLastRow * nRowCnt + i];
//				adjMat[i * nRowCnt + nLboRow] = adjMat[nLboRow * nRowCnt + i];
//			}
//		}
//
//		/// 将最后一个节点并入倒数第二个节点，并将最后一个节点删除
//		iLbo->insert(iLbo->end(), iLast->begin(), iLast->end());
//		nodes.erase(iLast);
//	}
//}

////层次聚类过程，存储空间减少则将图划分（最小割方法），自适应划分
//size_t HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows, VECROWSET &vecVirtual)
//{
//	ulong TotalMem = 0;
//	//遍历每个连通子图
//	for (NODEARRAY_ITER i = vecRows.begin(); i != vecRows.end();)
//	{
//		//计算当前连通子图的DFA表存储空间
//		ROWSET virtualRow;
//		size_t curRowval = StatisticVitualCore(oneDfa, *i, virtualRow);
		
		////由连通子图的节点编号建无向图
		//GRAPH curGraph;
		//BuildGraph(oneDfa, *i, curGraph);

		////最小割方法将连通子图划分，划分的子图节点编号存于partIdx
		//ROWSET partIdx;
		//StoreWagner(curGraph, partIdx);
		////VECROWSET partIdx;
		////StoreWagner(curGraph, partIdx);

		////得到连通子图的划分节点编号，对应于行集的下标，将对应行集安下标划分
		//ROWSET partRow1, partRow2, virtualRow1, virtualRow2;
		//for (size_t j = 0; j < i->size(); ++j)
		//{
		//	if (std::find(partIdx.begin(), partIdx.end(), j) != partIdx.end())
		//	{
		//		partRow1.push_back((*i)[j]);
		//	}
		//	else
		//	{
		//		partRow2.push_back((*i)[j]);
		//	}
		//}

		////计算划分后的DFA表存储空间
		//size_t partRowval = 0;
		//bool flag = false;
		//if (!partRow1.empty() && !partRow2.empty())
		//{
		//	partRowval = StatisticVitualCore(oneDfa, partRow1, virtualRow1)
		//		+ StatisticVitualCore(oneDfa, partRow2, virtualRow1);
		//	flag = true;
		//}

		////如果划分后存储空间减少，则用划分行集替换当前行集
		//if ((curRowval > partRowval) && flag)
		//{
		//	size_t nIdx = i - vecRows.begin();
		//	vecRows.erase(i);
		//	vecRows.push_back(partRow1);
		//	vecRows.push_back(partRow2);
		//	i = vecRows.begin() + nIdx;
		//}
		//else
		//{
//			TotalMem += curRowval;
//			vecVirtual.push_back(virtualRow);
//			++i;
//			//g_TotalMem += curRowval;
//		//}
//	}
//	return TotalMem;
//}

////标记函数，用来标记该状态是否在状态集中出现
//int SignState(byte state, ROWSET stateSet)
//{
//	int sign = 0;											//sign用来标记该状态是否在状态集中
//	for(ROWSET::size_type i= 0; i != stateSet.size(); ++i)					//循环遍历状态集
//	{
//		if(state == stateSet[i])							//如果该点在状态集中
//		{
//			sign = 1;										//标记位置为1
//		}
//	}
//	return sign;											//返回标记，1代表出现，0代表未出现
//}
////MA排序,removeSet为已移除的状态集，matrix为带全矩阵，oriSet为输出的排序状态集
//void MinArray(ROWSET removeSet, GRAPH matrix, ROWSET &oriSet)
//{
//	const int N = sqrt(matrix.size());											//N表示总状态数
//	ROWSET termiSet;															//termiSet为终态集
//	for(int i = 0; i != N; ++i)													//对termiSet初始化，将状态放入其中
//	{
//		int sign = 0;															//sign为标记位
//		if(removeSet.size() != 0)												//判断removeSet是否为空
//		{
//			sign = SignState(i, removeSet);										//若存在，则标记该状态是否已经被移除
//		}
//		if(sign == 0)															//如果未在removeSet中，即未移除，则放入终态集
//		{
//			termiSet.push_back(i);
//		}
//	}
//	int n = termiSet.size();													//n表示矩阵的维数
//	byte startState = termiSet.front();											//startState为初始状态
//	oriSet.push_back(startState);												//将初始状态放入oriSet
//	termiSet.erase(termiSet.begin());
//	ROWSET::iterator signState;
//	while (oriSet.size() != n)													//循环，直到所有的点都放入初态集中
//	{										
//		double weight = 0;														//result表示最大的权值
//		ROWSET::iterator i = termiSet.begin();									//signState用来标记最大的权值点
//		for (; i != termiSet.end(); ++i)										//循环遍历终态集
//		{
//			double tmp = 0;														//tmp为临时变量，表示当前权值
//			for (ROWSET::iterator j = oriSet.begin(); j != oriSet.end(); ++j)	//循环遍历初态集
//			{
//				if(matrix[(*i) * N + *j] != 0)								//如果标记为0并且边存在，则相加并保存在tmp
//				{
//					tmp += matrix[(*i) * N + *j];
//				}
//			}
//			if(tmp > weight)													//如果tmp大于result，则更新
//			{
//				weight = tmp;
//				signState = i;
//			}
//		}
//		oriSet.push_back(*signState);											//将该状态放入初态集中，作为最终的排序
//		termiSet.erase(signState);
//	}
//}
////状态合并，removeSet表示移除的状态集，需要更新，s和t状态为最后两个状态，brokenMartix记录带权矩阵的更新，vecSets记录状态集的更新
//int MergeState(byte s, byte t, ROWSET &removeSet, GRAPH &matrix, VECROWSET &sets)
//{
//	const int N = sqrt(matrix.size());						//n表示状态个数
//	int tCount = (int)t, sCount = (int)s;					//tCount表示t的行列号，sCount表示s的行列号
//	double minCutTmp = 0;									//minCutTmp用来表示当前的最小割权值
//	//更新带权矩阵，只需要更新s状态的行和列
//	for(int i = 0; i != N; ++i)
//	{
//		int sign = SignState(i, removeSet);					//sign用来标记该状态是否在removeSet里面
//		int tNumber = (tCount) * N + i;					//tNumber表示t行i列的元素
//		if(sign == 0 && matrix[tNumber] != 0)			//如果该状态在当前状态集中，并且t行i列元素不为0，则更新s行i列元素
//		{
//			minCutTmp += matrix[tNumber];				//累加求最小割权值
//			if(i != sCount)
//			{
//				matrix[sCount * N + i] += matrix[tNumber];							//更新s行
//				matrix[i * N + sCount] = matrix[sCount * N + i];					//更新s列
//			}
//		}
//	}
//	removeSet.push_back(t);									//更新removeSet，将t状态放入其中
//	//更新vecSets状态集，只需要更新s和t状态
//	ROWSET tmpVec;											//tmpVec表示临时变量
//	tmpVec.push_back(s);									//将s放入tmpVec
//	tmpVec.push_back(t);									//将t放入tmpVec
//	if(sets.size() == 0)									//如果vecSets为空，则初始化
//	{
//		sets.push_back(tmpVec);
//	}
//	else
//	{
//		VECROWSET::size_type i = 0;								//i为计数器
//		for(; i != sets.size(); ++i)							//遍历vecSets
//		{
//			if(s == sets[i].front())							//如果s在vecSets中，表示s之前代表过其他状态
//			{
//				sets[i].push_back(t);							//将t放入s所在的数组中
//				break;
//			}
//		}
//		if(i == sets.size())									//如果s之前没有代表过其他状态
//		{
//			sets.push_back(tmpVec);								//则将s，t放入vecSets
//		}
//	}
//	return minCutTmp;
//}
////最大流最小割算法
//void StoreWagner(GRAPH matrix, VECROWSET &result)
//{
//	ROWSET removeSet;								//removeSet表示已经移除的状态集
//	VECROWSET sets;									//vecSets用来表示状态的聚类集，result用来存储最终结果
//	double minCutTmp, minCutResult = sqrt(matrix.size());	//minCutTmp用来表示当前的最小割权值，minCutResult表示当前为止的最小割的最小值
//	const int N = sqrt(matrix.size());						//n为状态个数
//	while(removeSet.size() != N - 1)							//循环对图进行分割
//	{
//		ROWSET arySet;
//		MinArray(removeSet, matrix, arySet);			//MA算法，对状态进行排序
//		byte t = arySet.back();							//t表示最后一个状态，是要删除的状态
//		byte s = *(arySet.end()-2);						//s表示倒数第二个状态，是要代表的状态
//		double minCutTmp = MergeState(s, t, removeSet, matrix, sets);			//最小割算法，输出最小割权值，并对图进行更新
//		//输出结果，如果t状态出现在vecSets.front()，表示该t状态为一个状态集，而该列数组为其状态的集合
//		if(minCutTmp < minCutResult)					//如果当前的最小割权值为极小值
//		{
//			result.clear();
//			minCutResult = minCutTmp;
//			ROWSET first, second;
//			VECROWSET::size_type i = 0;
//			while(i != sets.size())											//循环遍历vecSets状态集
//			{
//				if(t == sets[i].front())									//如果t出现在vecSets.front()
//				{
//					first.assign(sets[i].begin(), sets[i].end());			//则将结果保存在result[1]中，即第一个行集
//					break;
//				}
//				++i;
//			}
//			if(i == sets.size())
//			{
//				first.push_back(t);
//			}
//			ROWSET::size_type m = first.size();
//			for(ROWSET::size_type j = 1; j != m; ++j)
//			{
//				for(ROWSET::size_type k = 0; k != sets.size(); ++k)
//				{
//					if(first[j] == sets[k].front())
//					{
//						first.insert(first.end(), sets[k].begin() + 1, sets[k].end());
//						break;
//					}
//				}
//			}
//			for(int i = 0; i != N; ++i)					//循环保存result[2]，即第二个行集
//			{
//				int sign = SignState(i, first);			//sign标记该状态是否出现在第一个行集中
//				if(sign == 0)							//如果没有出现，则保存在第二行集中
//				{
//					second.push_back(i);
//				}
//			}
//			result.push_back(first);
//			result.push_back(second);
//			++i;
//			g_TotalMem += curRowval;
//		}
//	}
//}

//将DFA字符映射表展开至256列
void UnflodDFA(CDfa &flodDfa, CDfa &unflodDfa)
{
	BYTE group[SC_DFACOLCNT];
	for (int i = 0; i < SC_DFACOLCNT; ++i)
	{
		group[i] = (BYTE)i;
	}
	unflodDfa.SetGroups(group);

	unflodDfa.Resize(flodDfa.Size(), SC_DFACOLCNT);
	for (ULONG i = 0; i < flodDfa.Size(); ++i)
	{
		for (ULONG j = 0; j < SC_DFACOLCNT; ++j)
		{
			BYTE z = flodDfa.Char2Group((BYTE)j);
			unflodDfa[i][j] = flodDfa[i][z];
		}
	}

	unflodDfa.SetId(flodDfa.GetId());
	unflodDfa.SetStartState(flodDfa.GetStartState());
	unflodDfa.GetFinalStates() = flodDfa.GetFinalStates();
}


//由DFA表中的行集建无向图，每一行代表图中的一个结点，边的权值为DFA表中两行中相同元素占的比率
void BuildGraph(const CDfa &oneDfa, GRAPH &graph, ROWSET &weightArg)
{
	//DFA表行数，即图中的结点个数
	size_t nRow = oneDfa.Size();

	//申请（结点个数 * 结点个数）大小的矩阵，一维数组存储
	graph.resize(nRow * nRow);
	std::fill(graph.begin(), graph.end(), 0);

	//任意两行都需要计算相似比率，无向图只需计算下（上）三角
	for (size_t i = 0; i < nRow; ++i)
	{
		for (size_t j = i + 1; j < nRow; ++j)
		{
			//DFA表中两行相同元素的个数
			size_t nEqualCnt = 0;
			for (size_t k = 0; k < oneDfa.GetGroupCount(); ++k)
			{
				if ((oneDfa[i][k] == oneDfa[j][k]))
				{
					++nEqualCnt;
				}
			}
			graph[i * nRow + j] = graph[j * nRow + i] = nEqualCnt;
			weightArg.push_back(nEqualCnt);
		}
	}

	std::sort(weightArg.begin(), weightArg.end());
	weightArg.erase(std::unique(weightArg.begin(), weightArg.end()), weightArg.end());
}

//图的深度搜索算法
int DFS_Visit(const GRAPH &graph, size_t vex, size_t limit, ROWSET &nodes, ROWSET &flag, ROWSET &curRow)
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
	if (nVexCnt * nVexCnt != graph.size())
	{
		throw 0;
	}

	for (size_t i = 0; i < nodes.size(); ++i)
	{
		if (graph[vex * nVexCnt + nodes[i]] > limit &&
			flag[nodes[i]] == 0)
		{
			DFS_Visit(graph, nodes[i], limit, nodes, flag, curRow);
		}
	}

	return 1;
}

//找出无向图中的连通子图，只记录子图对应的结点编号。采用图的深度搜索算法
void SearchConnectSubgraph(const GRAPH &graph, ROWSET &nodes, size_t limit, VECROWSET &partRows)
{
	size_t nVexCnt = sqrt(graph.size());
	if (nVexCnt * nVexCnt != graph.size())
	{
		throw 0;
	}

	//访问标记，记录哪些结点已经被访问
	ROWSET visit;
	visit.resize(nVexCnt);
	std::fill(visit.begin(), visit.end(), 0);

	//从一个未被访问的结点开始深搜，子图结点编号集合存于vecRows
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		if (visit[nodes[i]] == 0)
		{
			partRows.push_back(ROWSET());
			DFS_Visit(graph, nodes[i], limit, nodes, visit, partRows.back());
		}
	}
}

//统计虚拟核,每次一个行集
void StatisticVitualCore(const CDfa &oneDfa, const ROWSET &rs, ROWSET &virtualRow)
{
	size_t n_size = rs.size();   //行集大小
	size_t n_statenum = oneDfa.Size();  //dfa状态数
	size_t n_dfacol = oneDfa.GetGroupCount();//colnum

	for (size_t col = 0; col < n_dfacol; col++) //dfa列
	{
		std::map<STATEID, size_t> bary;

		for (size_t i = 0; i< n_size; i++) //统计出现次数
		{
			++bary[oneDfa[STATEID(rs[i])][col]];
		}

		STATEID maxindex = bary.begin()->first;
		size_t max = bary.begin()->second;
		for (std::map<STATEID, size_t>::iterator i = bary.begin(); i != bary.end(); ++i)
		{
			if (i->second > max)
			{
				max = i->second;
				maxindex = i->first;
			}
		}
		virtualRow.push_back(maxindex); //该列虚拟核
	}
}

//计算存储空间,每次一行
size_t CalculateMemory(const CDfa &oneDFA, const ROWSET &core, const ROWSET &row, ROWSET &bCntary)
{
	size_t n_size = row.size();
	size_t n_dfacol = oneDFA.GetGroupCount(); //dfa列

	bCntary.resize(n_size);
	std::fill(bCntary.begin(), bCntary.end(), 0);

	for (size_t col = 0; col < n_dfacol; col++) 
	{
		for (size_t i = 0; i < n_size; i++)   //存储跳转状态不同的个数
		{
			size_t bt = oneDFA[(STATEID)(row[i])][col];
			if (core[col] != bt)
			{
				bCntary[i]++;
			}
		}
	}

	//计算内存大小,n_size表示每一行都存储对应虚拟行的编号，n_dfacol表示虚拟行大小
	size_t vsmem = n_size + n_dfacol;
	for(size_t i = 0; i < n_size; i++)
	{
		vsmem += 2 * bCntary[i];
	}

	return vsmem;
}

//根据不同权值划分图，若划分后的存储空间小于划分前，则进行划分，并限制“特殊跳转”个数 < 8
void SplitGraph(CDfa &oneDFA, GRAPH &graph, ROWSET &weightArg, std::vector<BLOCK> &blocks)
{
	for (std::vector<BLOCK>::iterator i = blocks.begin(); i != blocks.end(); )
	{
		if (i->weightIdx < weightArg.size())
		{
			ROWSET curCore;
			ROWSET bCntary;
			StatisticVitualCore(oneDFA, i->nodes, curCore);
			//计算当前图的存储空间
			size_t curMem = CalculateMemory(oneDFA, curCore, i->nodes, bCntary);

			//划分子图，partRows中保存与子图对应的状态集合
			VECROWSET partRows;
			SearchConnectSubgraph(graph, i->nodes, weightArg[i->weightIdx], partRows);

			//计算所有划分子图的存储空间
			size_t partMem = 0;
			VECROWSET partCnt;
			for (NODEARRAY_ITER j = partRows.begin(); j != partRows.end() && !j->empty(); ++j)
			{
				ROWSET partCore;
				partCnt.push_back(ROWSET());
				StatisticVitualCore(oneDFA, *j, partCore);
				partMem += CalculateMemory(oneDFA, partCore, *j, partCnt.back());
			}

			//获取下一个用于划分子图的权值
			++i->weightIdx;

			//统计划分子图中“特殊跳转”的最大个数
			size_t partMax = 0;
			for (NODEARRAY_ITER j = partCnt.begin(); j != partCnt.end(); ++j)
			{
				size_t tmp = *(std::max_element(j->begin(),j->end()));
				if (partMax < tmp)
				{
					partMax = tmp;
				}
			}

			//统计当前图中“特殊跳转”的最大个数
			size_t curMax = *(std::max_element(bCntary.begin(),bCntary.end()));

			//判断条件：当划分的存储空间比当前存储空间小，
			//或者划分的“特殊跳转”个数比当前的个数少且少于8个，则进行划分
			if (curMem > partMem ||
				(curMax > partMax && curMax >= 8))   //yww (curMax > partMax && 8 > partMax)
			{
				std::vector<BLOCK> partBlocks;
				for (NODEARRAY_ITER j = partRows.begin(); j != partRows.end(); ++j)
				{
					partBlocks.push_back(BLOCK());
					partBlocks.back().weightIdx = i->weightIdx; 
					partBlocks.back().nodes = *j;
				}

				size_t Idx = i - blocks.begin();
				i = blocks.erase(i);
				blocks.insert(i, partBlocks.begin(), partBlocks.end());
				i = blocks.begin() + Idx;
			}
		}
		else
		{
			//直到每一个划分都遍历完所有的权值，再进行下一个子图的划分
			++i;
		}
	}
}

//计算跳转表和核矩阵存储空间大小
size_t StatisticMemory(const CDfa &oneDFA, const std::vector<BLOCK> &blocks, VECROWSET &vecCore)
{
	size_t nOneMem = 0;

	for (std::vector<BLOCK>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
	{
		vecCore.push_back(ROWSET());
		StatisticVitualCore(oneDFA, i->nodes, vecCore.back());
		
		//用于存储每个状态“特殊跳转”的个数
		VECROWSET vecCnt;
		vecCnt.push_back(ROWSET());
		nOneMem += CalculateMemory(oneDFA, vecCore.back(), i->nodes, vecCnt.back());
		
		//for (ROWSET::iterator i = vecCnt.back().begin(); i != vecCnt.back().end(); ++i)
		//{
		//	maxVal += *i;
		//}
		//size_t maxTmp = *(std::max_element(vecCnt.back().begin(), vecCnt.back().end()));
		//if (max > maxVal)
		//{
		//	maxVal = max;
		//}
	}

	return nOneMem;
}




////////////////////////////////////////////////////////////////////////////////////////////////////
////////
void SameColDfaCombine(CDfaArray &SameColDfa, std::map<ushort, Attribute> &columnNum)
{
	Attribute attribute;
	attribute.pushed = false;
	
	typedef std::pair<ushort, Attribute> column_Pair;
	std::map<ushort, Attribute>::iterator map_iter, map_it;


	ushort column, colLocation;
	CDfa tempDfa;
	
	//std::vector<bool> pushed;

	CGroupRes groupRes;
	groupRes.ReadFromFile("FinalResult.cdt");
	CDfaArray &CDfaSet = groupRes.GetDfaTable();
	
	VECROWSET allCharset;
	for (size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		////ulong nExtraMem = 0;
		//std::cout << i << std::endl;

		//allCharset.push_back(ROWSET());
		//for (size_t j = 0; j < SC_DFACOLCNT; ++j)
		//{
		//	allCharset.back().push_back(size_t(CDfaSet[i].Char2Group(j)));
		//}
		columnNum.insert(column_Pair(CDfaSet[i].GetColumnNum(), attribute));
		CDfaSet[i].SetId(i + 1);
	}
	//for(size_t i = 0; i < columnNum.size(); ++i)
	//{
	//	SameColDfa.PushBack(tempDfa);
	//	//pushed.push_back(false);
	//}
	for(map_it = columnNum.begin(); map_it != columnNum.end(); ++map_it)
	{
		tempDfa.SetId(map_it->first);
		SameColDfa.PushBack(tempDfa);
	}
	for(size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		column = CDfaSet[i].GetColumnNum();
		map_iter = columnNum.find(column);
		colLocation = 0;
		for(map_it = columnNum.begin(); map_it != map_iter; ++map_it, ++colLocation);
		//CDfaRow rowMatch(CDfaSet[i].Size());
		rowMatch rowChange(CDfaSet[i].Size());
		if(map_iter->second.pushed)
		{
			size_t plus = SameColDfa[colLocation].Size();
			size_t originSize = SameColDfa[colLocation].Size(), k;
			for(size_t j = 0; j < CDfaSet[i].Size(); ++j)
			{
				for(k = 0; k < originSize; ++k)
				{
					if(CDfaSet[i][j] == SameColDfa[colLocation][k])
					{
						//rowMatch[j] = k;
						rowChange[j] = k;
						break;
					}
				}
				if(k == originSize)
				{
					
					//push 
					SameColDfa[colLocation].PushBack(CDfaSet[i][j]);
					//rowMatch[j] = plus;
					rowChange[j] = plus;
					plus++;
					if(plus > 60000)
					{
						std::cout << "much" << std::endl;
					}
				}
			}
			map_iter->second.id_rowMatch[CDfaSet[i].GetId()] = rowChange;
		}
		else
		{
			map_iter->second.pushed = true;
			SameColDfa[colLocation] = CDfaSet[i];
			for(size_t j = 0; j < rowChange.size(); ++j)
			{
					rowChange[j] = j;
			}
			map_iter->second.id_rowMatch[CDfaSet[i].GetId()] = rowChange;
		}
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////