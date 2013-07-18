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

//���س��ִ��������±�
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

//ͳ�������,����洢�ռ�,ÿ��һ���м�
size_t StatisticVitualCore(const CDfa &oneDfa, ROWSET &rs)
{
	size_t n_size = rs.size();   //�м���С
	size_t n_statenum = oneDfa.Size();  //dfa״̬��
	size_t* bary = new size_t[n_statenum]; //ͳ�ƴ���
	size_t* bcountary = new size_t[n_statenum]; //�洢��ת״̬��ͬ�ĸ���
	//for (size_t bcount = 0; bcount < n_statenum; bcount++) //init
	//{
	//	bcountary[bcount] = 0;
	//}
	std::fill(bcountary, bcountary + n_statenum, 0);

	//size_t n_dfacol = oneDfa[0].Size();//colnum
	size_t n_dfacol = oneDfa.GetGroupCount();//colnum

	ROWSET visrow;
	for (size_t col = 0; col < n_dfacol; col++) //dfa��
	{
		//for (size_t ba = 0; ba < n_statenum; ba++) //init
		//{
		//	bary[ba] = 0;
		//}
		std::fill(bary, bary + n_statenum, 0);

		for (size_t i = 0; i< n_size; i++) //ͳ�Ƴ��ִ���
		{
			STATEID bt = oneDfa[(size_t)(rs[i])][col];
			bary[size_t(bt)]++;
		}
		STATEID maxindex = maxn(bary, n_statenum); //�������±�
		visrow.push_back((BYTE)(maxindex)); //���������
		for (size_t i = 0; i< n_size; i++)   //�洢��ת״̬��ͬ�ĸ���
		{
			BYTE bt = oneDfa[(size_t)(rs[i])][col];
			if (visrow[col] != bt)
			{
				bcountary[i]++;
			}
		}
	}
	//�����ڴ��С
	size_t vsmem = n_size;
	for(size_t i = 0; i < n_size; i++)
	{
		vsmem += 2*bcountary[i];
	}

	delete[] bary;
	delete[] bcountary;

	return vsmem;

}

//��С���㷨
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

		//�Ƚ����δ洢�ռ��С�Ƿ����
		//������٣������ָ��ķָ�ͼ
		//�������ָ�ǰ�ķָ�ͼ
		if (curRowval > partRowval)
		{
			vecRows.push_back(vecTmp[1]);
			--i;
		}
	}

}
