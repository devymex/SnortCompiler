#include "Hierarchical.h"

//��DFA���е��м�������ͼ��ÿһ�д���ͼ�е�һ����㣬�ߵ�ȨֵΪDFA������������ͬԪ��ռ�ı���
void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph)
{
	//DFA����������ͼ�еĽ�����
	size_t nRow = rows.size();
	//DFA������
	size_t nCol = oneDfa.GetGroupCount();

	//���루������ * ����������С�ľ���һά����洢
	graph.resize(nRow * nRow);
	std::fill(graph.begin(), graph.end(), (float)0.0);

	//�������ж���Ҫ�������Ʊ��ʣ�����ͼֻ������£��ϣ�����
	for (size_t i = 0; i < nRow; ++i)
	{
		for (size_t j = i + 1; j < nRow; ++j)
		{
			//DFA����������ͬԪ�صĸ���
			size_t nEqualCnt = 0;
			for (size_t k = 0; k < nCol; ++k)
			{
				if ((oneDfa[(STATEID)rows[i]][k] == oneDfa[(STATEID)rows[j]][k]))
				{
					++nEqualCnt;
				}
			}

			//��ͬԪ��ռ�ı��ʣ����ʴ���0.5������Ϊ����ͼ�ж�Ӧ���������һ���ߣ���ȨֵΪ����ֵ
			float radio = (float)nEqualCnt / (float)nCol;
			if (radio > 0.5)
			{
				graph[i * nRow + j] = graph[j * nRow + i] = radio;			 
			}
		}
	}
}

//ͼ����������㷨
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

//�ҳ�����ͼ�е���ͨ��ͼ��ֻ��¼��ͼ��Ӧ�Ľ���š�����ͼ����������㷨
void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows)
{
	/// �������ľ����Ƿ�Ϊ���󣬷����׳��쳣
	size_t nVexCnt = sqrt(float(graph.size()));
	if (nVexCnt * nVexCnt != graph.size())
	{
		throw 0;
	}

	//���ʱ�ǣ���¼��Щ����Ѿ�������
	ROWSET visit;
	visit.resize(nVexCnt);
	std::fill(visit.begin(), visit.end(), 0);

	//��һ��δ�����ʵĽ�㿪ʼ���ѣ���ͼ����ż��ϴ���vecRows
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

//ͳ�������,����洢�ռ�,ÿ��һ���м�
size_t StatisticVitualCore(const CDfa &oneDfa, ROWSET &rs)
{
	size_t n_size = rs.size();   //�м���С
	size_t n_statenum = oneDfa.Size();  //dfa״̬��
	size_t n_dfacol = oneDfa.GetGroupCount();//colnum

	size_t *bary = new size_t[n_statenum]; //ͳ�ƴ���
	size_t *bcountary = new size_t[n_statenum]; //�洢��ת״̬��ͬ�ĸ���
	std::fill(bcountary, bcountary + n_statenum, 0);


	ROWSET visrow;
	for (size_t col = 0; col < n_dfacol; col++) //dfa��
	{
		std::fill(bary, bary + n_statenum, 0);

		for (size_t i = 0; i< n_size; i++) //ͳ�Ƴ��ִ���
		{
			size_t bt = oneDfa[(STATEID)(rs[i])][col];
			bary[bt]++;
		}

		BYTE maxindex = BYTE(maxn(bary, n_statenum)); //�������±�

		visrow.push_back(maxindex); //���������

		for (size_t i = 0; i < n_size; i++)   //�洢��ת״̬��ͬ�ĸ���
		{
			BYTE bt = (BYTE)oneDfa[(STATEID)(rs[i])][col];
			if (visrow[col] != bt)
			{
				bcountary[i]++;
			}
		}
	}
	//�����ڴ��С,n_size��ʾÿһ�ж��洢��Ӧ�����еı�ţ�n_dfacol��ʾ�����д�С
	size_t vsmem = n_size + n_dfacol;
	for(size_t i = 0; i < n_size; i++)
	{
		vsmem += 2 * bcountary[i];
	}

	delete[] bary;
	delete[] bcountary;

	return vsmem;
}

/// ִ��MA����adjMatΪ�ڵ�ĸ�Ȩ�ڽӾ���nodesΪ�ϲ��ڵ�ļ��ϡ�
/// ÿһ���ϲ��ڵ�������ɵ�����ԭ�ڵ�ı�ţ���һ��Ԫ��Ϊ�úϲ��ڵ��ھ����е��кš�
float MaxAdjSort(const GRAPH &adjMat, std::vector<ROWSET> &nodes)
{
	/// �������Ĳ����Ƿ�Ϊ�գ������׳��쳣
	if (adjMat.empty() || nodes.empty())
	{
		throw 0;
	}

	/// �������ľ����Ƿ�Ϊ���󣬷����׳��쳣
	size_t nRowCnt = sqrt(float(adjMat.size()));
	if (nRowCnt * nRowCnt != adjMat.size() || nRowCnt < nodes.size())
	{
		throw 0;
	}

	/// ���nodes�еĵ�һ�ڵ�Ϊ�գ����׳��쳣
	if (nodes.front().empty())
	{
		throw 0;
	}

	/// sortedΪ�ź���Ľڵ㼯�ϣ�nodesΪδ�������򼯺ϵ������ڵ㡣
	/// ���ν�nodes�еĽڵ����sorted���ɡ�
	VECROWSET sorted;

	/// �Ƚ�nodes�еĵ�һ�ڵ����sorted
	sorted.push_back(nodes.front());
	nodes.erase(nodes.begin());

	/// fMaxWeight��������iMaxNode��sorted�ı�Ȩ�غ�
	float fMaxWeight = -1;

	/// ѭ������nodes�е����нڵ㣬ֱ��nodesΪ��
	for (; !nodes.empty(); )
	{
		/// iMaxNode��������nodes����sorted��Ȩ�غ����Ľڵ�
		NODEARRAY_ITER iMaxNode;

		fMaxWeight = -1;

		/// ����nodes���ҳ���sorted��Ȩ�غ����Ľڵ�
		for (NODEARRAY_ITER i = nodes.begin(); i != nodes.end(); ++i)
		{
			/// ���nodes���нڵ�Ϊ�գ����׳��쳣
			if (i->empty())
			{
				throw 0;
			}

			/// ���ڵ�ǰnodes�е�һ���ڵ�i������sorted�����i��sorted�ı�Ȩ��֮�͡�
			float fWeiSum = 0;
			for (NODEARRAY_ITER j = sorted.begin(); j != sorted.end(); ++j)
			{
				size_t iRow = i->front(), jRow = j->front();
				/// �ھ������ҳ��ӽڵ�j���ڵ�i�ı�Ȩ�أ��ۼӵ�fWeiSum��
				fWeiSum += adjMat[iRow * nRowCnt + jRow];
			}

			/// ����ڵ�i��sorted�ı�Ȩ�رȵ�ǰ���ֵ����������ֵ
			if (fWeiSum > fMaxWeight)
			{
				iMaxNode = i;
				fMaxWeight = fWeiSum;
			}
		}

		/// ����nodes���ҵ�����sorted��Ȩ�غ����Ľڵ����sorted������nodes��ɾ��
		sorted.push_back(*iMaxNode);
		nodes.erase(iMaxNode);
	}
	nodes = sorted;
	return fMaxWeight;
}

void StoreWagner(GRAPH &adjMat, ROWSET &part)
{
	/// �������ľ����Ƿ�Ϊ�գ������׳��쳣
	if (adjMat.empty())
	{
		throw 0;
	}

	/// �������ľ����Ƿ�Ϊ���󣬷����׳��쳣
	size_t nRowCnt = sqrt(float(adjMat.size()));
	if (nRowCnt * nRowCnt != adjMat.size())
	{
		throw 0;
	}

	/// ���첢��ʼ���ڵ㼯�ϡ�ÿ���ڵ��Ϊ�ϲ��ڵ㣬�ɴ��һ������ԭ�ڵ㡣
	/// �ϲ��ڵ��еĵ�һ���ڵ㼴Ϊ�úϲ��ڵ��ھ����е��кź��кš�
	/// �������Ϊ��ÿ���ϲ��ڵ㶼ֻ��һ���ڵ㡣
	VECROWSET nodes;
	for (size_t i = 0; i < nRowCnt; ++i)
	{
		nodes.push_back(ROWSET(1, i));
	}

	float fMinWeight = FLT_MAX;
	for (; nodes.size() > 1; )
	{
		/// �Ե�ǰ�ڵ����MA���򣬻�ȡ���һ���ڵ��Ȩ�غ�
		float fLastWeight = MaxAdjSort(adjMat, nodes);

		/// �����һ���Ȩ�غͱȵ�ǰ��С��Ȩ�غ�С���������Сֵ
		if (fLastWeight < fMinWeight)
		{
			part.clear();
			for (NODEARRAY_ITER i = nodes.begin(); i != nodes.end() - 1; ++i)
			{
				part.insert(part.end(), i->begin(), i->end());
			}
			fMinWeight = fLastWeight;
		}

		/// ȡ�������ڶ����ڵ�����һ���ڵ�Ĵ����к�
		NODEARRAY_ITER iLast = nodes.end() - 1;
		NODEARRAY_ITER iLbo = iLast - 1;
		size_t nLastRow = (size_t)iLast->front();
		size_t nLboRow = (size_t)iLbo->front();

		/// ��Last����һ���ڵ�i�ı�Ҫ���뵽��Lbo��i�ı�Ȩ����ȥ
		for (size_t i = 0; i < nRowCnt; ++i)
		{
			/// ע��i���ܵ���LastҲ���ܵ���Lbo
			if (i != nLastRow && i != nLboRow)
			{
				adjMat[nLboRow * nRowCnt + i] += adjMat[nLastRow * nRowCnt + i];
				adjMat[i * nRowCnt + nLboRow] = adjMat[nLboRow * nRowCnt + i];
			}
		}

		/// �����һ���ڵ㲢�뵹���ڶ����ڵ㣬�������һ���ڵ�ɾ��
		iLbo->insert(iLbo->end(), iLast->begin(), iLast->end());
		nodes.erase(iLast);
	}
}

//��ξ�����̣��洢�ռ������ͼ���֣���С�����������Ӧ����
void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows)
{
	//����ÿ����ͨ��ͼ
	for (NODEARRAY_ITER i = vecRows.begin(); i != vecRows.end(); ++i)
	{
		//���㵱ǰ��ͨ��ͼ��DFA��洢�ռ�
		size_t curRowval = StatisticVitualCore(oneDfa, *i);
		
		//����ͨ��ͼ�Ľڵ��Ž�����ͼ
		GRAPH curGraph;
		BuildGraph(oneDfa, *i, curGraph);

		//��С�������ͨ��ͼ���֣����ֵ���ͼ�ڵ��Ŵ���partIdx
		ROWSET partIdx;
		StoreWagner(curGraph, partIdx);

		//�õ���ͨ��ͼ�Ļ��ֽڵ��ţ���Ӧ���м����±꣬����Ӧ�м����±껮��
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

		//���㻮�ֺ��DFA��洢�ռ�
		size_t partRowval = StatisticVitualCore(oneDfa, partRow1)
			+ StatisticVitualCore(oneDfa, partRow2);

		//������ֺ�洢�ռ���٣����û����м��滻��ǰ�м�
		if (curRowval > partRowval)
		{
			vecRows.erase(i);
			vecRows.push_back(partRow1);
			vecRows.push_back(partRow2);
			--i;
		}
	}

}

