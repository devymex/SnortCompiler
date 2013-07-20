#include "Hierarchical.h"
#include <map>
#include <algorithm>

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
//MA排序,removeSet为已移除的状态集，matrix为带全矩阵，oriSet为输出的排序状态集
void MinArray(ROWSET removeSet,GRAPH matrix, ROWSET &oriSet)
{
	int n = sqrt(matrix.size());												//n表示矩阵的维数
	ROWSET oriSet, termiSet;													//oriSet为初态集，termiSet为终态集
	for(int i = 0; i != n; ++i)													//对termiSet初始化，将状态放入其中
	{
		int sign = 0;															//sign为标记位
		if(removeSet.size() != 0)												//判断removeSet是否为空
		{
			sign = SignState(i, removeSet);										//若存在，则标记该状态是否已经被移除
		}
		if(sign == 0)															//如果未在removeSet中，即未移除，则放入终态集
		{
			termiSet.push_back(i);
		}
	}
	byte startState = termiSet.front();											//startState为初始状态
	oriSet.push_back(startState);												//将初始状态放入oriSet
	termiSet.erase(termiSet.begin());
	ROWSET::iterator signState;
	while (oriSet.size() == n)													//循环，直到所有的点都放入初态集中
	{										
		double weight = 0;														//result表示最大的权值
		ROWSET::iterator i = termiSet.begin();									//signState用来标记最大的权值点
		for (; i != termiSet.end(); ++i)										//循环遍历终态集
		{
			double tmp = 0;														//tmp为临时变量，表示当前权值
			for (ROWSET::iterator j = oriSet.begin(); j != oriSet.end(); ++j)	//循环遍历初态集
			{
				if(matrix[(*i - 1) * n + *j] != 0)								//如果标记为0并且边存在，则相加并保存在tmp
				{
					tmp += matrix[(*i - 1)* n + *j];
				}
			}
			if(tmp > weight)													//如果tmp大于result，则更新
			{
				weight = tmp;
				signState = i;
			}
		}
		oriSet.push_back(*signState);											//将该状态放入初态集中，作为最终的排序
		termiSet.erase(signState);
	}
}

//最大流最小割算法
void StoreWagner(GRAPH matrix, VECROWSET &result)
{
	ROWSET arySet, removeSet, first, second;			//arySet表示排好序的状态集，removeSet表示已经移除的状态集
	VECROWSET sets;									//vecSets用来表示状态的聚类集，result用来存储最终结果
	double minCutTmp, minCutResult = sqrt(matrix.size());	//minCutTmp用来表示当前的最小割权值，minCutResult表示当前为止的最小割的最小值
	int n = sqrt(matrix.size());						//n为状态个数
	while(removeSet.size() != n)							//循环对图进行分割
	{
		MinArray(removeSet, matrix, arySet);			//MA算法，对状态进行排序
		byte t = arySet.back();							//t表示最后一个状态，是要删除的状态
		byte s = *(arySet.end()-2);						//s表示倒数第二个状态，是要代表的状态
		double minCutTmp = MergeState(s, t, removeSet, matrix, sets);			//最小割算法，输出最小割权值，并对图进行更新
		//输出结果，如果t状态出现在vecSets.front()，表示该t状态为一个状态集，而该列数组为其状态的集合
		if(minCutTmp < minCutResult)					//如果当前的最小割权值为极小值
		{
			for(VECROWSET::size_type i = 0; i != sets.size(); ++i)					//循环遍历vecSets状态集
			{
				if(t == sets[i].front())				//如果t出现在vecSets.front()
				{
					first.assign(sets[i].begin(), sets[i].end());				//则将结果保存在result[1]中，即第一个行集
				}
				else
				{
					first.push_back(t);				//如果没有，则直接将t保存在result[1]中，即第一个行集
				}
			}
			for(int i = 0; i != n; ++i)					//循环保存result[2]，即第二个行集
			{
				int sign = SignState(i, first);			//sign标记该状态是否出现在第一个行集中
				if(sign == 0)							//如果没有出现，则保存在第二行集中
				{
					second.push_back(i);
				}
			}
			result.push_back(first);
			result.push_back(second);
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
//状态合并，removeSet表示移除的状态集，需要更新，s和t状态为最后两个状态，brokenMartix记录带权矩阵的更新，vecSets记录状态集的更新
int MergeState(byte s, byte t, ROWSET &removeSet, GRAPH &matrix, VECROWSET &sets)
{
	int n = sqrt(matrix.size());						//n表示状态个数
	int tCount = (int)t, sCount = (int)s;					//tCount表示t的行列号，sCount表示s的行列号
	double minCutTmp = 0;									//minCutTmp用来表示当前的最小割权值
	//更新带权矩阵，只需要更新s状态的行和列
	for(int i = 0; i != n; ++i)
	{
		int sign = SignState(i, removeSet);					//sign用来标记该状态是否在removeSet里面
		int tNumber = (tCount - 1) * n + i;					//tNumber表示t行i列的元素
		if(sign == 0 && matrix[tNumber] != 0)			//如果该状态在当前状态集中，并且t行i列元素不为0，则更新s行i列元素
		{
			minCutTmp += matrix[tNumber];				//累加求最小割权值
			matrix[(sCount - 1) * n + i] += matrix[tNumber];							//更新s行
			matrix[(i - 1) * n + sCount] = matrix[(sCount - 1) * n + i];					//更新s列
		}
	}
	removeSet.push_back(t);									//更新removeSet，将t状态放入其中
	//更新vecSets状态集，只需要更新s和t状态
	ROWSET tmpVec;											//tmpVec表示临时变量
	if(sets.size() == 0)									//如果vecSets为空，则初始化
	{
		tmpVec.resize(2);
		sets.push_back(tmpVec);
	}
	tmpVec.push_back(s);									//将s放入tmpVec
	tmpVec.push_back(t);									//将t放入tmpVec
	VECROWSET::size_type i = 0;								//i为计数器
	for(; i != sets.size(); ++i)							//遍历vecSets
	{
		if(s == sets[i].front())							//如果s在vecSets中，表示s之前代表过其他状态
		{
			sets[i].push_back(t);							//将t放入s所在的数组中
			break;
		}
	}
	if(i == sets.size())									//如果s之前没有代表过其他状态
	{
		sets.push_back(tmpVec);								//则将s，t放入vecSets
	}
	return minCutTmp;
}
//标记函数，用来标记该状态是否在状态集中出现
int SignState(byte state, ROWSET stateSet)
{
	int sign = 0;											//sign用来标记该状态是否在状态集中
	for(ROWSET::size_type i= 0; i != stateSet.size(); ++i)					//循环遍历状态集
	{
		if(state == stateSet[i])							//如果该点在状态集中
		{
			sign = 1;										//标记位置为1
		}
	}
	return sign;											//返回标记，1代表出现，0代表未出现
}