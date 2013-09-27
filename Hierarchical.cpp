#include "Hierarchical.h"

size_t maxVal = 0;

////��DFA���е��м�������ͼ��ÿһ�д���ͼ�е�һ����㣬�ߵ�ȨֵΪDFA������������ͬԪ��ռ�ı���
//void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph)
//{
//	//DFA����������ͼ�еĽ�����
//	size_t nRow = rows.size();
//	//DFA������
//	size_t nCol = oneDfa.GetGroupCount();
//
//	//���루������ * ����������С�ľ���һά����洢
//	graph.resize(nRow * nRow);
//	std::fill(graph.begin(), graph.end(), (float)0.0);
//
//	
//	//size_t	nThreshold = limit * nCol;
//	//for (; nThreshold < (float(nCol) / 2.0f); ++nThreshold);
//
//	//�������ж���Ҫ�������Ʊ��ʣ�����ͼֻ������£��ϣ�����
//	for (size_t i = 0; i < nRow; ++i)
//	{
//		for (size_t j = i + 1; j < nRow; ++j)
//		{
//			//DFA����������ͬԪ�صĸ���
//			size_t nEqualCnt = 0;
//			for (size_t k = 0; k < nCol; ++k)
//			{
//				if ((oneDfa[(STATEID)rows[i]][k] == oneDfa[(STATEID)rows[j]][k]))
//				{
//					++nEqualCnt;
//				}
//			}
//
//			////��ͬԪ��ռ�ı��ʣ����ʴ���0.5������Ϊ����ͼ�ж�Ӧ���������һ���ߣ���ȨֵΪ����ֵ
//			//if (nEqualCnt >= nThreshold)
//			//{
//				graph[i * nRow + j] = graph[j * nRow + i] = (float)nEqualCnt / (float)nCol;			 
//			//}
//		}
//	}
//}
//
////ͼ����������㷨
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
////�ҳ�����ͼ�е���ͨ��ͼ��ֻ��¼��ͼ��Ӧ�Ľ���š�����ͼ����������㷨
//void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows)
//{
//	/// �������ľ����Ƿ�Ϊ���󣬷����׳��쳣
//	size_t nVexCnt = sqrt(float(graph.size()));
//	if (nVexCnt * nVexCnt != graph.size())
//	{
//		throw 0;
//	}
//
//	//���ʱ�ǣ���¼��Щ����Ѿ�������
//	ROWSET visit;
//	visit.resize(nVexCnt);
//	std::fill(visit.begin(), visit.end(), 0);
//
//	//��һ��δ�����ʵĽ�㿪ʼ���ѣ���ͼ����ż��ϴ���vecRows
//	for (size_t i = 0; i < nVexCnt; ++i)
//	{
//		if (visit[i] == 0)
//		{
//			vecRows.push_back(ROWSET());
//			DFS_Visit(graph, i, visit, vecRows.back());
//		}
//	}
//}

////���س��ִ��������±�
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

////ͳ�������,����洢�ռ�,ÿ��һ���м�
//size_t StatisticVitualCore(const CDfa &oneDfa, ROWSET &rs, ROWSET &virtualRow)
//{
//	size_t n_size = rs.size();   //�м���С
//	size_t n_statenum = oneDfa.Size();  //dfa״̬��
//	size_t n_dfacol = oneDfa.GetGroupCount();//colnum
//
//	size_t *bcountary = new size_t[n_statenum]; //�洢��ת״̬��ͬ�ĸ���
//	std::fill(bcountary, bcountary + n_statenum, 0);
//
//	for (size_t col = 0; col < n_dfacol; col++) //dfa��
//	{
//		std::map<STATEID, size_t> bary;
//
//		for (size_t i = 0; i< n_size; i++) //ͳ�Ƴ��ִ���
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
//		virtualRow.push_back(maxindex); //���������
//
//		for (size_t i = 0; i < n_size; i++)   //�洢��ת״̬��ͬ�ĸ���
//		{
//			size_t bt = oneDfa[(STATEID)(rs[i])][col];
//			if (virtualRow[col] != bt)
//			{
//				bcountary[i]++;
//			}
//		}
//	}
//	//�����ڴ��С,n_size��ʾÿһ�ж��洢��Ӧ�����еı�ţ�n_dfacol��ʾ�����д�С
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

//// ִ��MA����adjMatΪ�ڵ�ĸ�Ȩ�ڽӾ���nodesΪ�ϲ��ڵ�ļ��ϡ�
//// ÿһ���ϲ��ڵ�������ɵ�����ԭ�ڵ�ı�ţ���һ��Ԫ��Ϊ�úϲ��ڵ��ھ����е��кš�
//float MaxAdjSort(const GRAPH &adjMat, std::vector<ROWSET> &nodes)
//{
//	/// �������Ĳ����Ƿ�Ϊ�գ������׳��쳣
//	if (adjMat.empty() || nodes.empty())
//	{
//		throw 0;
//	}
//
//	/// �������ľ����Ƿ�Ϊ���󣬷����׳��쳣
//	size_t nRowCnt = sqrt(float(adjMat.size()));
//	if (nRowCnt * nRowCnt != adjMat.size() || nRowCnt < nodes.size())
//	{
//		throw 0;
//	}
//
//	/// ���nodes�еĵ�һ�ڵ�Ϊ�գ����׳��쳣
//	if (nodes.front().empty())
//	{
//		throw 0;
//	}
//
//	/// sortedΪ�ź���Ľڵ㼯�ϣ�nodesΪδ�������򼯺ϵ������ڵ㡣
//	/// ���ν�nodes�еĽڵ����sorted���ɡ�
//	VECROWSET sorted;
//
//	/// �Ƚ�nodes�еĵ�һ�ڵ����sorted
//	sorted.push_back(nodes.front());
//	nodes.erase(nodes.begin());
//
//	/// fMaxWeight��������iMaxNode��sorted�ı�Ȩ�غ�
//	float fMaxWeight = -1;
//
//	/// ѭ������nodes�е����нڵ㣬ֱ��nodesΪ��
//	for (; !nodes.empty(); )
//	{
//		/// iMaxNode��������nodes����sorted��Ȩ�غ����Ľڵ�
//		NODEARRAY_ITER iMaxNode;
//
//		fMaxWeight = -1;
//
//		/// ����nodes���ҳ���sorted��Ȩ�غ����Ľڵ�
//		for (NODEARRAY_ITER i = nodes.begin(); i != nodes.end(); ++i)
//		{
//			/// ���nodes���нڵ�Ϊ�գ����׳��쳣
//			if (i->empty())
//			{
//				throw 0;
//			}
//
//			/// ���ڵ�ǰnodes�е�һ���ڵ�i������sorted�����i��sorted�ı�Ȩ��֮�͡�
//			float fWeiSum = 0;
//			for (NODEARRAY_ITER j = sorted.begin(); j != sorted.end(); ++j)
//			{
//				size_t iRow = i->front(), jRow = j->front();
//				/// �ھ������ҳ��ӽڵ�j���ڵ�i�ı�Ȩ�أ��ۼӵ�fWeiSum��
//				fWeiSum += adjMat[iRow * nRowCnt + jRow];
//			}
//
//			/// ����ڵ�i��sorted�ı�Ȩ�رȵ�ǰ���ֵ����������ֵ
//			if (fWeiSum > fMaxWeight)
//			{
//				iMaxNode = i;
//				fMaxWeight = fWeiSum;
//			}
//		}
//
//		/// ����nodes���ҵ�����sorted��Ȩ�غ����Ľڵ����sorted������nodes��ɾ��
//		sorted.push_back(*iMaxNode);
//		nodes.erase(iMaxNode);
//	}
//	nodes = sorted;
//	return fMaxWeight;
//}
//
//void StoreWagner(GRAPH &adjMat, ROWSET &part)
//{
//	/// �������ľ����Ƿ�Ϊ�գ������׳��쳣
//	if (adjMat.empty())
//	{
//		throw 0;
//	}
//
//	/// �������ľ����Ƿ�Ϊ���󣬷����׳��쳣
//	size_t nRowCnt = sqrt(float(adjMat.size()));
//	if (nRowCnt * nRowCnt != adjMat.size())
//	{
//		throw 0;
//	}
//
//	/// ���첢��ʼ���ڵ㼯�ϡ�ÿ���ڵ��Ϊ�ϲ��ڵ㣬�ɴ��һ������ԭ�ڵ㡣
//	/// �ϲ��ڵ��еĵ�һ���ڵ㼴Ϊ�úϲ��ڵ��ھ����е��кź��кš�
//	/// �������Ϊ��ÿ���ϲ��ڵ㶼ֻ��һ���ڵ㡣
//	VECROWSET nodes;
//	for (size_t i = 0; i < nRowCnt; ++i)
//	{
//		nodes.push_back(ROWSET(1, i));
//	}
//
//	float fMinWeight = FLT_MAX;
//	for (; nodes.size() > 1; )
//	{
//		/// �Ե�ǰ�ڵ����MA���򣬻�ȡ���һ���ڵ��Ȩ�غ�
//		float fLastWeight = MaxAdjSort(adjMat, nodes);
//
//		/// �����һ���Ȩ�غͱȵ�ǰ��С��Ȩ�غ�С���������Сֵ
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
//		/// ȡ�������ڶ����ڵ�����һ���ڵ�Ĵ����к�
//		NODEARRAY_ITER iLast = nodes.end() - 1;
//		NODEARRAY_ITER iLbo = iLast - 1;
//		size_t nLastRow = (size_t)iLast->front();
//		size_t nLboRow = (size_t)iLbo->front();
//
//		/// ��Last����һ���ڵ�i�ı�Ҫ���뵽��Lbo��i�ı�Ȩ����ȥ
//		for (size_t i = 0; i < nRowCnt; ++i)
//		{
//			/// ע��i���ܵ���LastҲ���ܵ���Lbo
//			if (i != nLastRow && i != nLboRow)
//			{
//				adjMat[nLboRow * nRowCnt + i] += adjMat[nLastRow * nRowCnt + i];
//				adjMat[i * nRowCnt + nLboRow] = adjMat[nLboRow * nRowCnt + i];
//			}
//		}
//
//		/// �����һ���ڵ㲢�뵹���ڶ����ڵ㣬�������һ���ڵ�ɾ��
//		iLbo->insert(iLbo->end(), iLast->begin(), iLast->end());
//		nodes.erase(iLast);
//	}
//}

////��ξ�����̣��洢�ռ������ͼ���֣���С�����������Ӧ����
//size_t HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows, VECROWSET &vecVirtual)
//{
//	ulong TotalMem = 0;
//	//����ÿ����ͨ��ͼ
//	for (NODEARRAY_ITER i = vecRows.begin(); i != vecRows.end();)
//	{
//		//���㵱ǰ��ͨ��ͼ��DFA��洢�ռ�
//		ROWSET virtualRow;
//		size_t curRowval = StatisticVitualCore(oneDfa, *i, virtualRow);
		
		////����ͨ��ͼ�Ľڵ��Ž�����ͼ
		//GRAPH curGraph;
		//BuildGraph(oneDfa, *i, curGraph);

		////��С�������ͨ��ͼ���֣����ֵ���ͼ�ڵ��Ŵ���partIdx
		//ROWSET partIdx;
		//StoreWagner(curGraph, partIdx);
		////VECROWSET partIdx;
		////StoreWagner(curGraph, partIdx);

		////�õ���ͨ��ͼ�Ļ��ֽڵ��ţ���Ӧ���м����±꣬����Ӧ�м����±껮��
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

		////���㻮�ֺ��DFA��洢�ռ�
		//size_t partRowval = 0;
		//bool flag = false;
		//if (!partRow1.empty() && !partRow2.empty())
		//{
		//	partRowval = StatisticVitualCore(oneDfa, partRow1, virtualRow1)
		//		+ StatisticVitualCore(oneDfa, partRow2, virtualRow1);
		//	flag = true;
		//}

		////������ֺ�洢�ռ���٣����û����м��滻��ǰ�м�
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

////��Ǻ�����������Ǹ�״̬�Ƿ���״̬���г���
//int SignState(byte state, ROWSET stateSet)
//{
//	int sign = 0;											//sign������Ǹ�״̬�Ƿ���״̬����
//	for(ROWSET::size_type i= 0; i != stateSet.size(); ++i)					//ѭ������״̬��
//	{
//		if(state == stateSet[i])							//����õ���״̬����
//		{
//			sign = 1;										//���λ��Ϊ1
//		}
//	}
//	return sign;											//���ر�ǣ�1������֣�0����δ����
//}
////MA����,removeSetΪ���Ƴ���״̬����matrixΪ��ȫ����oriSetΪ���������״̬��
//void MinArray(ROWSET removeSet, GRAPH matrix, ROWSET &oriSet)
//{
//	const int N = sqrt(matrix.size());											//N��ʾ��״̬��
//	ROWSET termiSet;															//termiSetΪ��̬��
//	for(int i = 0; i != N; ++i)													//��termiSet��ʼ������״̬��������
//	{
//		int sign = 0;															//signΪ���λ
//		if(removeSet.size() != 0)												//�ж�removeSet�Ƿ�Ϊ��
//		{
//			sign = SignState(i, removeSet);										//�����ڣ����Ǹ�״̬�Ƿ��Ѿ����Ƴ�
//		}
//		if(sign == 0)															//���δ��removeSet�У���δ�Ƴ����������̬��
//		{
//			termiSet.push_back(i);
//		}
//	}
//	int n = termiSet.size();													//n��ʾ�����ά��
//	byte startState = termiSet.front();											//startStateΪ��ʼ״̬
//	oriSet.push_back(startState);												//����ʼ״̬����oriSet
//	termiSet.erase(termiSet.begin());
//	ROWSET::iterator signState;
//	while (oriSet.size() != n)													//ѭ����ֱ�����еĵ㶼�����̬����
//	{										
//		double weight = 0;														//result��ʾ����Ȩֵ
//		ROWSET::iterator i = termiSet.begin();									//signState�����������Ȩֵ��
//		for (; i != termiSet.end(); ++i)										//ѭ��������̬��
//		{
//			double tmp = 0;														//tmpΪ��ʱ��������ʾ��ǰȨֵ
//			for (ROWSET::iterator j = oriSet.begin(); j != oriSet.end(); ++j)	//ѭ��������̬��
//			{
//				if(matrix[(*i) * N + *j] != 0)								//������Ϊ0���ұߴ��ڣ�����Ӳ�������tmp
//				{
//					tmp += matrix[(*i) * N + *j];
//				}
//			}
//			if(tmp > weight)													//���tmp����result�������
//			{
//				weight = tmp;
//				signState = i;
//			}
//		}
//		oriSet.push_back(*signState);											//����״̬�����̬���У���Ϊ���յ�����
//		termiSet.erase(signState);
//	}
//}
////״̬�ϲ���removeSet��ʾ�Ƴ���״̬������Ҫ���£�s��t״̬Ϊ�������״̬��brokenMartix��¼��Ȩ����ĸ��£�vecSets��¼״̬���ĸ���
//int MergeState(byte s, byte t, ROWSET &removeSet, GRAPH &matrix, VECROWSET &sets)
//{
//	const int N = sqrt(matrix.size());						//n��ʾ״̬����
//	int tCount = (int)t, sCount = (int)s;					//tCount��ʾt�����кţ�sCount��ʾs�����к�
//	double minCutTmp = 0;									//minCutTmp������ʾ��ǰ����С��Ȩֵ
//	//���´�Ȩ����ֻ��Ҫ����s״̬���к���
//	for(int i = 0; i != N; ++i)
//	{
//		int sign = SignState(i, removeSet);					//sign������Ǹ�״̬�Ƿ���removeSet����
//		int tNumber = (tCount) * N + i;					//tNumber��ʾt��i�е�Ԫ��
//		if(sign == 0 && matrix[tNumber] != 0)			//�����״̬�ڵ�ǰ״̬���У�����t��i��Ԫ�ز�Ϊ0�������s��i��Ԫ��
//		{
//			minCutTmp += matrix[tNumber];				//�ۼ�����С��Ȩֵ
//			if(i != sCount)
//			{
//				matrix[sCount * N + i] += matrix[tNumber];							//����s��
//				matrix[i * N + sCount] = matrix[sCount * N + i];					//����s��
//			}
//		}
//	}
//	removeSet.push_back(t);									//����removeSet����t״̬��������
//	//����vecSets״̬����ֻ��Ҫ����s��t״̬
//	ROWSET tmpVec;											//tmpVec��ʾ��ʱ����
//	tmpVec.push_back(s);									//��s����tmpVec
//	tmpVec.push_back(t);									//��t����tmpVec
//	if(sets.size() == 0)									//���vecSetsΪ�գ����ʼ��
//	{
//		sets.push_back(tmpVec);
//	}
//	else
//	{
//		VECROWSET::size_type i = 0;								//iΪ������
//		for(; i != sets.size(); ++i)							//����vecSets
//		{
//			if(s == sets[i].front())							//���s��vecSets�У���ʾs֮ǰ���������״̬
//			{
//				sets[i].push_back(t);							//��t����s���ڵ�������
//				break;
//			}
//		}
//		if(i == sets.size())									//���s֮ǰû�д��������״̬
//		{
//			sets.push_back(tmpVec);								//��s��t����vecSets
//		}
//	}
//	return minCutTmp;
//}
////�������С���㷨
//void StoreWagner(GRAPH matrix, VECROWSET &result)
//{
//	ROWSET removeSet;								//removeSet��ʾ�Ѿ��Ƴ���״̬��
//	VECROWSET sets;									//vecSets������ʾ״̬�ľ��༯��result�����洢���ս��
//	double minCutTmp, minCutResult = sqrt(matrix.size());	//minCutTmp������ʾ��ǰ����С��Ȩֵ��minCutResult��ʾ��ǰΪֹ����С�����Сֵ
//	const int N = sqrt(matrix.size());						//nΪ״̬����
//	while(removeSet.size() != N - 1)							//ѭ����ͼ���зָ�
//	{
//		ROWSET arySet;
//		MinArray(removeSet, matrix, arySet);			//MA�㷨����״̬��������
//		byte t = arySet.back();							//t��ʾ���һ��״̬����Ҫɾ����״̬
//		byte s = *(arySet.end()-2);						//s��ʾ�����ڶ���״̬����Ҫ�����״̬
//		double minCutTmp = MergeState(s, t, removeSet, matrix, sets);			//��С���㷨�������С��Ȩֵ������ͼ���и���
//		//�����������t״̬������vecSets.front()����ʾ��t״̬Ϊһ��״̬��������������Ϊ��״̬�ļ���
//		if(minCutTmp < minCutResult)					//�����ǰ����С��ȨֵΪ��Сֵ
//		{
//			result.clear();
//			minCutResult = minCutTmp;
//			ROWSET first, second;
//			VECROWSET::size_type i = 0;
//			while(i != sets.size())											//ѭ������vecSets״̬��
//			{
//				if(t == sets[i].front())									//���t������vecSets.front()
//				{
//					first.assign(sets[i].begin(), sets[i].end());			//�򽫽��������result[1]�У�����һ���м�
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
//			for(int i = 0; i != N; ++i)					//ѭ������result[2]�����ڶ����м�
//			{
//				int sign = SignState(i, first);			//sign��Ǹ�״̬�Ƿ�����ڵ�һ���м���
//				if(sign == 0)							//���û�г��֣��򱣴��ڵڶ��м���
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

//��DFA�ַ�ӳ���չ����256��
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


//��DFA���е��м�������ͼ��ÿһ�д���ͼ�е�һ����㣬�ߵ�ȨֵΪDFA������������ͬԪ��ռ�ı���
void BuildGraph(const CDfa &oneDfa, GRAPH &graph, ROWSET &weightArg)
{
	//DFA����������ͼ�еĽ�����
	size_t nRow = oneDfa.Size();

	//���루������ * ����������С�ľ���һά����洢
	graph.resize(nRow * nRow);
	std::fill(graph.begin(), graph.end(), 0);

	//�������ж���Ҫ�������Ʊ��ʣ�����ͼֻ������£��ϣ�����
	for (size_t i = 0; i < nRow; ++i)
	{
		for (size_t j = i + 1; j < nRow; ++j)
		{
			//DFA����������ͬԪ�صĸ���
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

//ͼ����������㷨
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

//�ҳ�����ͼ�е���ͨ��ͼ��ֻ��¼��ͼ��Ӧ�Ľ���š�����ͼ����������㷨
void SearchConnectSubgraph(const GRAPH &graph, ROWSET &nodes, size_t limit, VECROWSET &partRows)
{
	size_t nVexCnt = sqrt(graph.size());
	if (nVexCnt * nVexCnt != graph.size())
	{
		throw 0;
	}

	//���ʱ�ǣ���¼��Щ����Ѿ�������
	ROWSET visit;
	visit.resize(nVexCnt);
	std::fill(visit.begin(), visit.end(), 0);

	//��һ��δ�����ʵĽ�㿪ʼ���ѣ���ͼ����ż��ϴ���vecRows
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		if (visit[nodes[i]] == 0)
		{
			partRows.push_back(ROWSET());
			DFS_Visit(graph, nodes[i], limit, nodes, visit, partRows.back());
		}
	}
}

//ͳ�������,ÿ��һ���м�
void StatisticVitualCore(const CDfa &oneDfa, const ROWSET &rs, ROWSET &virtualRow)
{
	size_t n_size = rs.size();   //�м���С
	size_t n_statenum = oneDfa.Size();  //dfa״̬��
	size_t n_dfacol = oneDfa.GetGroupCount();//colnum

	for (size_t col = 0; col < n_dfacol; col++) //dfa��
	{
		std::map<STATEID, size_t> bary;

		for (size_t i = 0; i< n_size; i++) //ͳ�Ƴ��ִ���
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
		virtualRow.push_back(maxindex); //���������
	}
}

//����洢�ռ�,ÿ��һ��
size_t CalculateMemory(const CDfa &oneDFA, const ROWSET &core, const ROWSET &row, ROWSET &bCntary)
{
	size_t n_size = row.size();
	size_t n_dfacol = oneDFA.GetGroupCount(); //dfa��

	bCntary.resize(n_size);
	std::fill(bCntary.begin(), bCntary.end(), 0);

	for (size_t col = 0; col < n_dfacol; col++) 
	{
		for (size_t i = 0; i < n_size; i++)   //�洢��ת״̬��ͬ�ĸ���
		{
			size_t bt = oneDFA[(STATEID)(row[i])][col];
			if (core[col] != bt)
			{
				bCntary[i]++;
			}
		}
	}

	//�����ڴ��С,n_size��ʾÿһ�ж��洢��Ӧ�����еı�ţ�n_dfacol��ʾ�����д�С
	size_t vsmem = n_size + n_dfacol;
	for(size_t i = 0; i < n_size; i++)
	{
		vsmem += 2 * bCntary[i];
	}

	return vsmem;
}

//���ݲ�ͬȨֵ����ͼ�������ֺ�Ĵ洢�ռ�С�ڻ���ǰ������л��֣������ơ�������ת������ < 8
void SplitGraph(CDfa &oneDFA, GRAPH &graph, ROWSET &weightArg, std::vector<BLOCK> &blocks)
{
	for (std::vector<BLOCK>::iterator i = blocks.begin(); i != blocks.end(); )
	{
		if (i->weightIdx < weightArg.size())
		{
			ROWSET curCore;
			ROWSET bCntary;
			StatisticVitualCore(oneDFA, i->nodes, curCore);
			//���㵱ǰͼ�Ĵ洢�ռ�
			size_t curMem = CalculateMemory(oneDFA, curCore, i->nodes, bCntary);

			//������ͼ��partRows�б�������ͼ��Ӧ��״̬����
			VECROWSET partRows;
			SearchConnectSubgraph(graph, i->nodes, weightArg[i->weightIdx], partRows);

			//�������л�����ͼ�Ĵ洢�ռ�
			size_t partMem = 0;
			VECROWSET partCnt;
			for (NODEARRAY_ITER j = partRows.begin(); j != partRows.end() && !j->empty(); ++j)
			{
				ROWSET partCore;
				partCnt.push_back(ROWSET());
				StatisticVitualCore(oneDFA, *j, partCore);
				partMem += CalculateMemory(oneDFA, partCore, *j, partCnt.back());
			}

			//��ȡ��һ�����ڻ�����ͼ��Ȩֵ
			++i->weightIdx;

			//ͳ�ƻ�����ͼ�С�������ת����������
			size_t partMax = 0;
			for (NODEARRAY_ITER j = partCnt.begin(); j != partCnt.end(); ++j)
			{
				size_t tmp = *(std::max_element(j->begin(),j->end()));
				if (partMax < tmp)
				{
					partMax = tmp;
				}
			}

			//ͳ�Ƶ�ǰͼ�С�������ת����������
			size_t curMax = *(std::max_element(bCntary.begin(),bCntary.end()));

			//�ж������������ֵĴ洢�ռ�ȵ�ǰ�洢�ռ�С��
			//���߻��ֵġ�������ת�������ȵ�ǰ�ĸ�����������8��������л���
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
			//ֱ��ÿһ�����ֶ����������е�Ȩֵ���ٽ�����һ����ͼ�Ļ���
			++i;
		}
	}
}

//������ת��ͺ˾���洢�ռ��С
size_t StatisticMemory(const CDfa &oneDFA, const std::vector<BLOCK> &blocks, VECROWSET &vecCore)
{
	size_t nOneMem = 0;

	for (std::vector<BLOCK>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
	{
		vecCore.push_back(ROWSET());
		StatisticVitualCore(oneDFA, i->nodes, vecCore.back());
		
		//���ڴ洢ÿ��״̬��������ת���ĸ���
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