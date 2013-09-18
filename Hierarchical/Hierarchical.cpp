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

const static size_t threshold = 4;

size_t Estimate(const CDfa &coreMatrix, const ROWSET &partSet)
{
	const size_t col = coreMatrix.GetGroupCount();
	const size_t row = partSet.size();
	std::vector<bool> sign(row,true);
	int temp =0;
	size_t mem = -1;
	for(size_t i = 0; i != row; ++i)
	{
		int result = 0;
		for(size_t j = 0; j != row && sign[j] == true; ++j)
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
				result = 0;
				break;
			}
			else
				result += count;
		}
		if(temp < result)
		{
			mem = partSet[i];
			temp =result;
		}
	}
	return mem;
}
void update(ROWSET &member, std::map<size_t, ROWSET> &state)
{

}

void CoreCompress(CDfaArray &corMatrixSets, std::map<size_t, ROWSET> &state)
{
	for(size_t l = 0; l != corMatrixSets.Size(); ++l)
	{
		CDfa &corMatrix = corMatrixSets[l];
		 
		GRAPH graph;
		ROWSET weightArg;
		BuildGraph(corMatrix, graph, weightArg);
		ROWSET nodes;
		for(size_t i = 0; i != corMatrix.Size(); ++i)
			nodes.push_back(i);
		for(ROWSET::reverse_iterator i = weightArg.rbegin(); i != weightArg.rend(); ++i)
		{
			VECROWSET partRows;
			SearchConnectSubgraph(graph, nodes, *i, partRows);
			ROWSET member;
			bool sign = true;
			for(VECROWSET::iterator j = partRows.begin(); j != partRows.end(); ++j)
			{
				size_t mem = Estimate(corMatrix, *j);
				if(mem = -1)
				{
					sign = false;
					break;
				}
				else
					member.push_back(mem);
			}
			if(sign = true)
				update(corMatrix, state[col]
		}
	}
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
	std::fstream  fs("SameColDfa.txt", std::ios::out);
	for(size_t i = 0; i < SameColDfa.Size(); ++i)
	{
		fs << "NEXT " << SameColDfa[i].GetColumnNum() << std::endl; 
		for(size_t j = 0; j < SameColDfa[i].Size(); ++j)
		{
			for(size_t k = 0; k < SameColDfa[i][j].Size(); ++k)
			{
				fs.width(7);
				fs << SameColDfa[i][j][k] << " ";
			}
			fs << std::endl;
		}
		
	}

	std::fstream fm("id_rowMatch.txt", std::ios::out);
	std::map<ulong, rowMatch>::iterator it_rowMatch;
	for(map_it = columnNum.begin(); map_it != columnNum.end(); ++map_it)
	{
		fm << "column" << map_it->first << std::endl;
		for(it_rowMatch = map_it->second.id_rowMatch.begin(); it_rowMatch != map_it->second.id_rowMatch.end(); ++it_rowMatch)
		{
			fm << "DfaID ";
			fm.width(7);
			fm << it_rowMatch->first;
			for(size_t i = 0; i < it_rowMatch->second.size(); ++i)
			{
				fm.width(7);
				fm << it_rowMatch->second[i];
			}
			fm << std::endl;
		}
	}
	std::fstream fc("result.txt", std::ios::out);
	size_t j;
	size_t dfa = 0;
	for(map_it = columnNum.begin(), j = 0; map_it != columnNum.end(); ++map_it, ++j)
	{
		fc << "column " << map_it->first << std::endl;
		fc << "Dfa num " << map_it->second.id_rowMatch.size() << std::endl;
		dfa += map_it->second.id_rowMatch.size();
		fc << "state num " << SameColDfa[j].Size() << std::endl;
	}
	std::cout << dfa << std::endl;
	std::cout << j << std::endl;
	fc.close();
	fs.close();
	fm.close();
}

void DiffColDfaCombine(CDfaArray &SameColDfa, std::map<ushort, Attribute> &columnNum, std::map<ushort, Attribute>::iterator lower, std::map<ushort, Attribute>::iterator upper)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////