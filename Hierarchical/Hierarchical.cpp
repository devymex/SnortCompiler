#include "Hierarchical.h"
#include <map>

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

void MinArray(ROWSET &rows,GRAPH matrix, ROWSET &aryState)
{
	int n = sqrt(matrix.size());
	byte startState;
	std::vector<byte> oriState;
	oriState.push_back(startState);
	std::vector<byte> termiState = ;
	std::vector<byte>::iterator iter = find(termiState.begin(), termiState.end(), startState);
	termiState.erase(iter);

	for (std::vector<byte>::size_type i = 0; i != termiState.size(); ++i)
	{	
		std::map<double, byte> mapStaResult;

		for (std::vector<byte>::size_type j =0; j != termiState.size(); ++j)
		{
			byte staNum = termiState[j];
			double result = 0;

			for (std::vector<byte>::size_type k = 0; k != oriState.size(); ++k)
			{
				int rownum = (int)termiState[j];
				int colnum = (int)oriState[k];

				result += matrix[rownum * n + colnum];
			}

			std::pair<double, byte> pairResult(result, staNum);
			mapStaResult.insert(pairResult);
		}

		int newState = mapStaResult.end()->second;
		oriState.push_back(newState);
		std::vector<byte>::iterator jter = find(termiState.begin(), termiState.end(), newState);
		termiState.erase(jter);
	}
	aryState = oriState;
}


void SearchConSubGraph(const GRAPH &graph, VECROWSET &vecRows)
{
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
void StoreWagner(VECROWSET &vecRows, GRAPH matrix)
{
	int n = sqrt(matrix.size());
	ROWSET rows = vecRows[0];
	ROWSET aryState;
	VECROWSET vecResult;
	GRAPH finalMatrix = matrix;
	std::vector<double> finalResult;
	for(int i = 0; i != n; ++i)
	{
		MinArray(rows, matrix, aryState);
		
		byte t = aryState.back();
		byte s = *(aryState.end()-1);
		int rowNum1 = (int)t;
		int rowNum2 = (int)s;
		double minCutWeight = 0;
		for(int j = 0; j != n; ++j)
		{
			if(finalMatrix[rowNum1 * n + j] != 0)
			{
				minCutWeight += finalMatrix[rowNum1 * n + j];
				if(finalMatrix[rowNum2 * n + j] != 0)
				{
					finalMatrix[rowNum2 * n + j] += finalMatrix[rowNum1 * n + j];
				}
			finalMatrix[rowNum1 * n + j] = 0;
			}
		}

		int current = finalResult.back();
		finalResult.push_back(minCutWeight);
		int now = finalResult.back();
		if(current > now)
		{
			matrix = finalMatrix;
		}
		if(vecResult.size() == 0)
		{
			vecResult[0].push_back(t);
		}
		for(VECROWSET::iterator k = vecResult.begin(); k != vecResult.end(); ++k)
		{
			for(ROWSET::iterator l = k->begin(); l != k->end(); ++l)
			{
				vecResult[k].push_back(t);
				vecResult[k].push_back(s);
	
			}
	}
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
