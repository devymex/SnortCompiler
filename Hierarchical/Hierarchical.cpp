#include "Hierarchical.h"

size_t maxVal = 0;


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
//void MinAry(GRAPH matrix, VECROWSET &result)
//{
//	VECROWSET ary;
//	const int N = sqrt(matrix.size());
//	ary.push_back(result.front());
//	result.erase(result.begin());
//	VECROWSET::iterator signState;
//
//	while (result.size() != 0)
//	{
//		double weight = 0;
//		for (VECROWSET::iterator i = result.begin(); i != result.end(); ++i)
//		{
//			double tmp = 0;	
//			for (VECROWSET::iterator j = ary.begin(); j != ary.end(); ++j)
//			{
//				int number = (int)(i->front()) * N + (int)(j->front());
//				if(matrix[number] != 0)
//				{
//					tmp += matrix[number];
//				}
//			}
//			if(tmp > weight)
//			{
//				weight = tmp;
//				signState = i;	
//			}
//		}
//		ary.push_back(*signState);
//		result.erase(signState);
//	}
//	result.assign(ary.begin(),ary.end());
//}
//
//void StoreWagner(GRAPH &matrix, ROWSET &resultSet)
//{
//	const int N = sqrt(matrix.size());
//	VECROWSET result;
//	for(int i = 0; i != N; ++i)
//	{
//		result.push_back(ROWSET(1,i));
//	}
//	double minCutResult = sqrt(matrix.size());
//
//	while(result.size() != 1)
//	{
//		MinAry(matrix, result);
//		byte t = (result.end()-1)->front();
//		byte s = (result.end()-2)->front();
//		double minCutTmp = matrix[(int)t * N + (int)s];
//
//		VECROWSET::iterator i = result.begin();
//		for(; i != result.end() - 2; ++i)
//		{
//			int tCount = (int)t * N + (int)(i->front());
//			int sCount = (int)s * N + (int)(i->front());
//			if(matrix[tCount] != 0)
//			{
//				minCutTmp += matrix[tCount];
//				matrix[sCount] += matrix[tCount];
//				matrix[(int)(i->front()) * N + (int)s] = matrix[sCount];
//			}
//		}
//		if(minCutTmp < minCutResult)
//		{
//			minCutResult = minCutTmp;
//			resultSet.assign((i + 1)->begin(), (i + 1)->end());
//		}
//		i->insert(i->end(),(i + 1)->begin(),(i + 1)->end());
//		result.erase(i + 1);
//	}
//}


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
		if (graph[vex * nVexCnt + nodes[i]] >= limit &&
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

const static size_t threshold = 2;
const static size_t column = 4;

//struct stateTable{
//	ROWSET state;
//	std::map<size_t, size_t> *sp;
//};


int Estimate(const CDfa &coreMatrix, const ROWSET &partSet)
{
	const size_t col = coreMatrix.GetGroupCount();
	const size_t row = partSet.size();
	std::vector<bool> sign(row,true);
	int temp =0;
	int mem = -1;
	for(size_t i = 0; i != row; ++i)
	{
		int result = 0;
		if(sign[i] == true)
		{
			for(size_t j = 0; j != row; ++j)
			{	
				int count = 0;
				for(size_t k = 0; k != col; ++k)
				{
					if(coreMatrix[partSet[i]][k] != coreMatrix[partSet[j]][k])
						++count;
				}
				if(count > threshold)
				{
					sign[i] = sign[j] = false;
					result = -1;
					break;
				}
				else
					result += count;
			}
			if(temp <= result)
			{
				mem = partSet[i];
				temp =result;
			}
		}
	}
	return mem;
}

void update(const CDfa &corMatrix, const VECROWSET &partRows, const ROWSET &member, Attribute &dfaID)
{
	std::map<ulong, rowMatch> &state = dfaID.id_rowMatch;
	const size_t col = corMatrix[0].Size();
	bool sign = false;
	for(std::map<ulong, rowMatch>::iterator k = state.begin(); k != state.end(); ++k)
	{
		std::vector<std::map<size_t, size_t> > skipTable;
		for(std::vector<ushort>::iterator l = k->second.begin(); l != k->second.end(); ++l)
		{
			std::map<size_t, size_t> value;
			for(size_t i = 0; i != member.size(); ++i)
			{
				for(size_t j = 0; j != partRows[i].size(); ++j)
				{
					if(partRows[i][j] == static_cast<size_t>(*l))
					{
						for(size_t m = 0; m != col; ++m)
						{
							if(corMatrix[member[i]][m] != corMatrix[*l][m])
							{
								std::pair<size_t, size_t> v(m, corMatrix[*l][m]);
								value[m];
							}
						}
						*l = i;
						sign = true;
						break;
					}
				}
				if(sign == true)
					break;
			}
			if(sign == true)
				skipTable.push_back(value);
		}

	}
}

void CoreCompress(CDfaArray &corMatrixSets, std::map<ushort, Attribute> &state)
{
	std::map<ushort, Attribute>::iterator dfaIdSet = state.begin();
	for(ushort l = 0; l != corMatrixSets.Size(); ++l, ++dfaIdSet)
	{
		if(corMatrixSets[l].GetColumnNum() == column)
		{
			CDfa &corMatrix = corMatrixSets[l];
			Attribute dfaID = dfaIdSet->second;
			const ushort col = dfaIdSet->first;
			GRAPH graph;
			ROWSET weightArg;
			BuildGraph(corMatrix, graph, weightArg);

			ROWSET nodes;
			for(size_t i = 0; i != corMatrix.Size(); ++i)
				nodes.push_back(i);
			const size_t weight = n - threshold;
			{
				VECROWSET partRows;
				SearchConnectSubgraph(graph, nodes, weight, partRows);
				ROWSET member;
				bool sign = true;
				for(VECROWSET::iterator j = partRows.begin(); j != partRows.end(); ++j)
				{
					int mem = Estimate(corMatrix, *j);
					if(mem == -1)
					{
						sign = false;
						member.clear();
						break;
					}
					else
						member.push_back(mem);
				}
				if(sign == true)
				{
					update(corMatrix, partRows, member, dfaID);
					break;
				}
			}
		}
	}
}