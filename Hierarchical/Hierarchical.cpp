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
//void SameColDfaCombine(CDfaArray &SameColDfa, std::map<ushort, Attribute> &columnNum)
//{
//	Attribute attribute;
//	attribute.pushed = false;
//	
//	typedef std::pair<ushort, Attribute> column_Pair;
//	std::map<ushort, Attribute>::iterator map_iter, map_it;
//
//
//	ushort column, colLocation;
//	CDfa tempDfa;
//	
//	//std::vector<bool> pushed;
//	std::vector<int> origin, minus, now;
//
//	CGroupRes groupRes;
//	groupRes.ReadFromFile("FinalResult.cdt");
//	CDfaArray &CDfaSet = groupRes.GetDfaTable();
//	
//	VECROWSET allCharset;
//	for (size_t i = 0; i < CDfaSet.Size(); ++i)
//	{
//		////ulong nExtraMem = 0;
//		//std::cout << i << std::endl;
//
//		//allCharset.push_back(ROWSET());
//		//for (size_t j = 0; j < SC_DFACOLCNT; ++j)
//		//{
//		//	allCharset.back().push_back(size_t(CDfaSet[i].Char2Group(j)));
//		//}
//		columnNum.insert(column_Pair(CDfaSet[i].GetColumnNum(), attribute));
//		CDfaSet[i].SetId(i + 1);
//	}
//	//for(size_t i = 0; i < columnNum.size(); ++i)
//	//{
//	//	SameColDfa.PushBack(tempDfa);
//	//	//pushed.push_back(false);
//	//}
//	for(map_it = columnNum.begin(); map_it != columnNum.end(); ++map_it)
//	{
//		tempDfa.SetId(map_it->first);
//		SameColDfa.PushBack(tempDfa);
//		origin.push_back(0);
//		minus.push_back(0);
//		now.push_back(0);
//	}
//	for(size_t i = 0; i < CDfaSet.Size(); ++i)
//	{
//		column = CDfaSet[i].GetColumnNum();
//		map_iter = columnNum.find(column);
//		colLocation = 0;
//		for(map_it = columnNum.begin(); map_it != map_iter; ++map_it, ++colLocation);
//		//CDfaRow rowMatch(CDfaSet[i].Size());
//		rowMatch rowChange(CDfaSet[i].Size());
//
//		///////////////////////////////////////////////////////////////////////////////
//		origin[colLocation] += CDfaSet[i].Size();
//
//		/*if(map_iter->second.pushed)
//		{*/
//			//size_t plus = SameColDfa[colLocation].Size();
//			size_t originSize = SameColDfa[colLocation].Size(), k;
//			for(size_t j = 0; j < CDfaSet[i].Size(); ++j)
//			{
//				originSize = SameColDfa[colLocation].Size();
//				for(k = 0; k < originSize; ++k)
//				{
//					if(CDfaSet[i][j] == SameColDfa[colLocation][k])
//					{
//						//rowMatch[j] = k;
//						rowChange[j] = k;
//						///////////////////////////////////////////////////////////////
//						minus[colLocation]++;
//						
//						break;
//					}
//				}
//				if(k == originSize)
//				{
//					
//					//push 
//					SameColDfa[colLocation].PushBack(CDfaSet[i][j]);
//					//rowMatch[j] = plus;
//					rowChange[j] = originSize;
//					/*plus++;
//					if(plus > 60000)
//					{
//						std::cout << "much" << std::endl;
//					}*/
//				}
//			}
//			map_iter->second.id_rowMatch[CDfaSet[i].GetId()] = rowChange;
//		//}
//		//else
//		//{
//		//	map_iter->second.pushed = true;
//		//	SameColDfa[colLocation] = CDfaSet[i];
//		//	for(size_t j = 0; j < rowChange.size(); ++j)
//		//	{
//		//			rowChange[j] = j;
//		//	}
//		//	map_iter->second.id_rowMatch[CDfaSet[i].GetId()] = rowChange;
//		//}
//	}
//	std::fstream  fs("SameColDfa.txt", std::ios::out);
//	for(size_t i = 0; i < SameColDfa.Size(); ++i)
//	{
//		/////////////////////////////////////////////////////////////////////////////////
//		now[i] = SameColDfa[i].Size();
//
//		fs << "NEXT " << SameColDfa[i].GetColumnNum() << std::endl; 
//		for(size_t j = 0; j < SameColDfa[i].Size(); ++j)
//		{
//			for(size_t k = 0; k < SameColDfa[i][j].Size(); ++k)
//			{
//				fs.width(7);
//				fs << SameColDfa[i][j][k] << " ";
//			}
//			fs << std::endl;
//		}
//		
//	}
//
//	std::fstream fm("id_rowMatch.txt", std::ios::out);
//	std::map<ulong, rowMatch>::iterator it_rowMatch;
//	for(map_it = columnNum.begin(); map_it != columnNum.end(); ++map_it)
//	{
//		fm << "column" << map_it->first << std::endl;
//		for(it_rowMatch = map_it->second.id_rowMatch.begin(); it_rowMatch != map_it->second.id_rowMatch.end(); ++it_rowMatch)
//		{
//			fm << "DfaID ";
//			fm.width(7);
//			fm << it_rowMatch->first;
//			for(size_t i = 0; i < it_rowMatch->second.size(); ++i)
//			{
//				fm.width(7);
//				fm << it_rowMatch->second[i];
//			}
//			fm << std::endl;
//		}
//	}
//	std::fstream fc("result.txt", std::ios::out);
//	size_t j;
//	size_t dfa = 0;
//	fc << "column\tDfa num\tstate num\torigin\tminus\tnow" << std::endl;
//	for(map_it = columnNum.begin(), j = 0; map_it != columnNum.end(); ++map_it, ++j)
//	{
//		fc << fc.width(6) << map_it->first << "\t";
//		fc << fc.width(7) << map_it->second.id_rowMatch.size() << "\t";
//		dfa += map_it->second.id_rowMatch.size();
//		fc << fc.width(9) << SameColDfa[j].Size() << "\t";
//		fc << fc.width(6) << origin[j] << "\t ";
//		fc << fc.width(6) << minus[j] << " \t";
//		fc << fc.width(7) << now[j] << std::endl;
//	}
//	std::cout << dfa << std::endl;
//	std::cout << j << std::endl;
//	fc.close();
//	fs.close();
//	fm.close();
//}

bool equal(CDfaRow &row, std::vector<ushort> vec)
{
	if(row.Size() != vec.size())
	{
		return false;
	}
	else
	{
		for(size_t i = 0; i < vec.size(); ++i)
		{
			if(row[i] != vec[i])
				return false;
		}
	}
	return true;
}

//void SameColDfaCombine(COLCOMBINEARRAY &colCombineArray)
//{
//	ushort column, colLocation;
//	
//	std::vector<int> origin, minus, now;
//
//	CGroupRes groupRes;
//	groupRes.ReadFromFile("FinalResult.cdt");
//	CDfaArray &CDfaSet = groupRes.GetDfaTable();
//	
//	std::set<ushort> allCol;
//	std::set<ushort>::iterator col_it, col;
//	COLUMNCOMBINE sameColInform;
//	for (size_t i = 0; i < CDfaSet.Size(); ++i)
//	{
//		allCol.insert(CDfaSet[i].GetColumnNum());
//		CDfaSet[i].SetId(i + 1);
//	}
//	
//	for(col_it = allCol.begin(); col_it != allCol.end(); ++col_it)
//	{
//		sameColInform.column = *col_it;
//		colCombineArray.push_back(sameColInform);
//		origin.push_back(0);
//		minus.push_back(0);
//		now.push_back(0);
//	}
//
//	for(size_t i = 0; i < CDfaSet.Size(); ++i)
//	{
//		column = CDfaSet[i].GetColumnNum();
//		col_it = allCol.find(column);
//
//		colLocation = 0;
//		for(col = allCol.begin(); col != col_it; col++, colLocation++) ;
//
//		///////////////////////////////////////////////////////////////////////////////
//		origin[colLocation] += CDfaSet[i].Size();
//
//		ROWTRANSFORM rowMatch;
//		rowMatch.dfaId = CDfaSet[i].GetId();
//		ROWNODE rownode;
//		size_t originSize, k;
//		for(size_t j = 0; j < CDfaSet[i].Size(); ++j)
//		{
//			originSize = colCombineArray[colLocation].sameColumnMatrix.size();
//			for(k = 0; k < originSize; ++k)
//			{
//				if(equal(CDfaSet[i][j], colCombineArray[colLocation].sameColumnMatrix[k]))
//				{
//					rownode.newNum = k;
//					rowMatch.rowTransform.push_back(rownode);
//					///////////////////////////////////////////////////////////////
//					minus[colLocation]++;
//						
//					break;
//				}
//			}
//			if(k == originSize)
//			{
//				std::vector<ushort> v;
//				for(size_t t = 0; t < CDfaSet[i][j].Size(); ++t)
//				{
//					v.push_back(CDfaSet[i][j][t]);
//				}
//				colCombineArray[colLocation].sameColumnMatrix.push_back(v);
//				//SameColDfa[colLocation].PushBack(CDfaSet[i][j]);
//				rownode.newNum = originSize;
//				rowMatch.rowTransform.push_back(rownode);	
//			}
//		}
//		colCombineArray[colLocation].rowTrans.push_back(rowMatch);
//	}
//	std::fstream  fs("new SameColDfa.txt", std::ios::out);
//	std::fstream fm("new id_rowMatch.txt", std::ios::out);
//	std::fstream fc("new result.txt", std::ios::out);
//	size_t dfa = 0;
//	for(size_t i = 0; i < colCombineArray.size(); ++i)
//	{
//		/////////////////////////////////////////////////////////////////////////////////
//		now[i] = colCombineArray[i].sameColumnMatrix.size();
//
//		fs << "NEXT " << colCombineArray[i].column << std::endl; 
//		for(size_t j = 0; j < colCombineArray[i].sameColumnMatrix.size(); ++j)
//		{
//			for(size_t k = 0; k < colCombineArray[i].sameColumnMatrix[j].size(); ++k)
//			{
//				fs.width(7);
//				fs << colCombineArray[i].sameColumnMatrix[j][k] << " ";
//			}
//			fs << std::endl;
//		}
//		
//		fm << "column" << colCombineArray[i].column << std::endl;
//		for(size_t j = 0; j < colCombineArray[i].rowTrans.size(); ++j)/*it_rowMatch = map_it->second.id_rowMatch.begin(); it_rowMatch != map_it->second.id_rowMatch.end(); ++it_rowMatch)*/
//		{
//			fm << "DfaID ";
//			fm.width(7);
//			fm << colCombineArray[i].rowTrans[j].dfaId;
//			for(size_t k = 0; k < colCombineArray[i].rowTrans[j].rowTransform.size(); ++k)
//			{
//				fm.width(7);
//				fm << colCombineArray[i].rowTrans[j].rowTransform[k].newNum;
//			}
//			fm << std::endl;
//		}
//
//		fc << "column\tDfa num\tstate num\torigin\tminus\tnow" << std::endl;
//		fc << fc.width(6) <<colCombineArray[i].column << "\t";
//		fc << fc.width(7) << colCombineArray[i].rowTrans.size() << "\t";
//		dfa += colCombineArray[i].rowTrans.size();
//		fc << fc.width(9) << colCombineArray[i].sameColumnMatrix.size() << "\t";
//		fc << fc.width(6) << origin[i] << "\t ";
//		fc << fc.width(6) << minus[i] << " \t";
//		fc << fc.width(7) << now[i] << std::endl;
//	}
//
//	std::cout << dfa << std::endl;
//	fc.close();
//	fs.close();
//	fm.close();
//}

void AnotherReplaceRowMatchValue(COLUMNCOMBINE &inColCom, std::vector<ROWTRANSFORM> &row, ushort old, ushort now)
{
	/*std::map<ulong, rowMatch>::iterator rowmatch;
	for(rowmatch = it->second.id_rowMatch.begin(); rowmatch != it->second.id_rowMatch.end(); ++it)
	{
		for(size_t j = 0; j < rowmatch->second.size(); ++j)
		{
			if(rowmatch->second[j] == old)
			{
				rowmatch->second[j] = now;
			}
		}
	}*/
	for(size_t i = 0; i < inColCom.rowTrans.size(); ++i)
	{
		for(size_t j = 0; j < inColCom.rowTrans[i].rowTransform.size(); ++j)
		{
			if(inColCom.rowTrans[i].rowTransform[j].newNum == old)
			{
				row[i].rowTransform[j].newNum = now;
			}
		}
	}
}

void RemoveTheSame(COLCOMBINEARRAY &colCombineArray)
{
	std::vector<SORTASSITANT> nVec;
	SORTASSITANT p;
	std::vector<std::vector<ushort> > diff;
	std::vector<ROWTRANSFORM> row;
	for(size_t i = 0; i < colCombineArray.size(); ++i)
	{
		nVec.clear();
		diff.clear();
		row = colCombineArray[i].rowTrans;
		for(size_t j = 0; j < colCombineArray[i].sameColumnMatrix.size(); ++j)
		{
			p.n = j;
			p.m_t = &colCombineArray[i].sameColumnMatrix;
			nVec.push_back(p);
		}
		std::sort(nVec.begin(), nVec.end(), ROWSORT());
		ushort now = 0;
		AnotherReplaceRowMatchValue(colCombineArray[i], row, nVec[0].n, now);
		diff.push_back(colCombineArray[i].sameColumnMatrix[nVec[0].n]);
		for(size_t j = 1; j < nVec.size(); )
		{
			if(colCombineArray[i].sameColumnMatrix[nVec[j - 1].n] != colCombineArray[i].sameColumnMatrix[nVec[j].n])
			{
				//now++;
				now = diff.size();
				diff.push_back(colCombineArray[i].sameColumnMatrix[nVec[j].n]);
			}
			
			AnotherReplaceRowMatchValue(colCombineArray[i], row, nVec[j].n, now);
			++j;
		}
		/*if(colCombineArray[i].sameColumnMatrix[nVec[nVec.size() - 1].n] != colCombineArray[i].sameColumnMatrix[nVec[nVec.size() - 2].n])
		{
			now = colCombineArray[i].sameColumnMatrix.size();
			
		}
		AnotherReplaceRowMatchValue(colCombineArray[i], row, nVec[nVec.size() - 1].n, now);
		diff.push_back(colCombineArray[i].sameColumnMatrix[nVec[nVec.size() - 1].n]);*/

		colCombineArray[i].rowTrans = row;
		colCombineArray[i].sameColumnMatrix = diff;
	}
}



void SameColDfaCombine(COLCOMBINEARRAY &colCombineArray)
{
	ushort column, colLocation;
	
	std::vector<int> origin, minus, now;

	CGroupRes groupRes;
	groupRes.ReadFromFile("FinalResult.cdt");
	CDfaArray &CDfaSet = groupRes.GetDfaTable();
	
	std::set<ushort> allCol;
	std::set<ushort>::iterator col_it, col;
	COLUMNCOMBINE sameColInform;
	for (size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		allCol.insert(CDfaSet[i].GetColumnNum());
		CDfaSet[i].SetId(i + 1);
	}
	
	for(col_it = allCol.begin(); col_it != allCol.end(); ++col_it)
	{
		sameColInform.column = *col_it;
		colCombineArray.push_back(sameColInform);
		origin.push_back(0);
		minus.push_back(0);
		now.push_back(0);
	}

	for(size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		column = CDfaSet[i].GetColumnNum();
		col_it = allCol.find(column);

		colLocation = 0;
		for(col = allCol.begin(); col != col_it; col++, colLocation++) ;

		///////////////////////////////////////////////////////////////////////////////
		origin[colLocation] += CDfaSet[i].Size();

		ROWTRANSFORM rowMatch;
		rowMatch.dfaId = CDfaSet[i].GetId();
		ROWNODE rownode;
		size_t originSize, k = colCombineArray[colLocation].sameColumnMatrix.size();
		for(size_t j = 0; j < CDfaSet[i].Size(); ++j)
		{
			//originSize = colCombineArray[colLocation].sameColumnMatrix.size();
			//for(k = 0; k < originSize; ++k)
			//{
			//	if(equal(CDfaSet[i][j], colCombineArray[colLocation].sameColumnMatrix[k]))
			//	{
			//		rownode.newNum = k;
			//		rowMatch.rowTransform.push_back(rownode);
			//		///////////////////////////////////////////////////////////////
			//		minus[colLocation]++;
			//			
			//		break;
			//	}
			//}
			//if(k == originSize)
			//{
			//	std::vector<ushort> v;
			//	for(size_t t = 0; t < CDfaSet[i][j].Size(); ++t)
			//	{
			//		v.push_back(CDfaSet[i][j][t]);
			//	}
			//	colCombineArray[colLocation].sameColumnMatrix.push_back(v);
			//	//SameColDfa[colLocation].PushBack(CDfaSet[i][j]);
			//	rownode.newNum = originSize;
			//	rowMatch.rowTransform.push_back(rownode);	
			//}
			std::vector<ushort> v;
			for(size_t t = 0; t < CDfaSet[i][j].Size(); ++t)
			{
				v.push_back(CDfaSet[i][j][t]);
			}
			rownode.newNum = k;
			rowMatch.rowTransform.push_back(rownode);
			k++;
			colCombineArray[colLocation].sameColumnMatrix.push_back(v);
		}
		colCombineArray[colLocation].rowTrans.push_back(rowMatch);
	}
	RemoveTheSame(colCombineArray);
	//std::fstream  fs("new SameColDfa.txt", std::ios::out);
	//std::fstream fm("new id_rowMatch.txt", std::ios::out);
	//std::fstream fc("new result.txt", std::ios::out);
	//size_t dfa = 0;
	//for(size_t i = 0; i < colCombineArray.size(); ++i)
	//{
	//	/////////////////////////////////////////////////////////////////////////////////
	//	now[i] = colCombineArray[i].sameColumnMatrix.size();

	//	fs << "NEXT " << colCombineArray[i].column << std::endl; 
	//	for(size_t j = 0; j < colCombineArray[i].sameColumnMatrix.size(); ++j)
	//	{
	//		for(size_t k = 0; k < colCombineArray[i].sameColumnMatrix[j].size(); ++k)
	//		{
	//			fs.width(7);
	//			fs << colCombineArray[i].sameColumnMatrix[j][k] << " ";
	//		}
	//		fs << std::endl;
	//	}
	//}
	//for(size_t i = 0; i < colCombineArray.size(); ++i)
	//{
	//	fm << "column" << colCombineArray[i].column << std::endl;
	//	for(size_t j = 0; j < colCombineArray[i].rowTrans.size(); ++j)/*it_rowMatch = map_it->second.id_rowMatch.begin(); it_rowMatch != map_it->second.id_rowMatch.end(); ++it_rowMatch)*/
	//	{
	//		fm << "DfaID ";
	//		fm.width(7);
	//		fm << colCombineArray[i].rowTrans[j].dfaId;
	//		for(size_t k = 0; k < colCombineArray[i].rowTrans[j].rowTransform.size(); ++k)
	//		{
	//			fm.width(7);
	//			fm << colCombineArray[i].rowTrans[j].rowTransform[k].newNum;
	//		}
	//		fm << std::endl;
	//	}

	//for(size_t i = 0; i < colCombineArray.size(); ++i)
	//{
	//	fc << "column\tDfa num\tstate num\torigin\tminus\tnow" << std::endl;
	//	fc << fc.width(6) <<colCombineArray[i].column << "\t";
	//	fc << fc.width(7) << colCombineArray[i].rowTrans.size() << "\t";
	//	dfa += colCombineArray[i].rowTrans.size();
	//	fc << fc.width(9) << colCombineArray[i].sameColumnMatrix.size() << "\t";
	//	fc << fc.width(6) << origin[i] << "\t ";
	//	fc << fc.width(6) << minus[i] << " \t";
	//	fc << fc.width(7) << now[i] << std::endl;
	//}

	//std::cout << dfa << std::endl;
	//fc.close();
	//fs.close();
	//fm.close();
}

/*
void ReplaceRowMatchValue(std::map<ushort, Attribute>::iterator it, ushort old, ushort now)
{
	std::map<ulong, rowMatch>::iterator rowmatch;
	for(rowmatch = it->second.id_rowMatch.begin(); rowmatch != it->second.id_rowMatch.end(); ++it)
	{
		for(size_t j = 0; j < rowmatch->second.size(); ++j)
		{
			if(rowmatch->second[j] == old)
			{
				rowmatch->second[j] = now;
			}
		}
	}
}

void TwoColDfaCombine(CDfa &inDfa, std::map<ushort, Attribute>::iterator it, CDfa &outDfa)
{
	size_t i, j, k;
	bool b = true;
	for(i = 0; i < inDfa.Size(); ++i)
	{
		b = true;
		for(j = 0; j < outDfa.Size(); ++j)
		{
			b = true;
			for(k = 0; k < inDfa[i].Size(); ++k)
			{
				if(inDfa[i][k] != outDfa[j][k])
				{
					b = false;
					break;
				}
			}
			if(b == true)
			{
				break;
			}
		}
		if(b == true)
		{
			ReplaceRowMatchValue(it, i, j);
		}
		else
		{
			CDfaRow row(inDfa[i]);
			if(inDfa.GetColumnNum() < outDfa.GetColumnNum())
			{
				for(k = inDfa.GetColumnNum(); k < outDfa.GetColumnNum(); ++k)
				{
					row.PushBack(outDfa[0][k]);
				}
			}
			outDfa.PushBack(row);
			ReplaceRowMatchValue(it, i, outDfa.Size() - 1);
		}
	}
}

void DiffColDfaCombine(CDfaArray &SameColDfa, const size_t l, const size_t h, std::map<ushort, Attribute>::iterator upper, CDfa &outComDfa)
{
	//size_t l = 0, h = 0;
	//std::map<ushort, Attribute>::iterator it;
	//for(it = columnNum.begin(); it != lower; ++it)
	//{
	//	++l;
	//}

	//for(it = columnNum.begin(); it != upper; ++it)
	//{
	//	++h;
	//}

	//outComDfa = SameColDfa[h];
	std::map<ushort, Attribute>::iterator it = upper;
	for(size_t i = h; i >= l; --i)
	{
		TwoColDfaCombine(SameColDfa[i], it, outComDfa);
		it--;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
*/
void ReplaceRowMatchValue(COLUMNCOMBINE &inColCom, ushort old, ushort now)
{
	/*std::map<ulong, rowMatch>::iterator rowmatch;
	for(rowmatch = it->second.id_rowMatch.begin(); rowmatch != it->second.id_rowMatch.end(); ++it)
	{
		for(size_t j = 0; j < rowmatch->second.size(); ++j)
		{
			if(rowmatch->second[j] == old)
			{
				rowmatch->second[j] = now;
			}
		}
	}*/
	for(size_t i = 0; i < inColCom.rowTrans.size(); ++i)
	{
		for(size_t j = 0; j < inColCom.rowTrans[i].rowTransform.size(); ++j)
		{
			if(inColCom.rowTrans[i].rowTransform[j].newNum == old)
			{
				inColCom.rowTrans[i].rowTransform[j].newNum = now;
			}
		}
	}
}

int TwoColDfaCombine(COLUMNCOMBINE &inColCom, COLUMNCOMBINE &outColCom) /*, CDfa &inDfa, std::map<ushort, Attribute>::iterator it, CDfa &outDfa)*/
{
	int minus = 0;
	size_t i, j, k;
	bool b = true;
	int difference = 0, diffLocation, mindiff;
	std::vector<ROWTRANSFORM> originRow;
	if(outColCom.sameColumnMatrix.size() == 0)
	{
		outColCom = inColCom;
	}
	else
	{
		originRow = inColCom.rowTrans;
		for(i = 0; i < inColCom.sameColumnMatrix.size(); ++i)
		{
			mindiff = inColCom.sameColumnMatrix[i].size();
			diffLocation = 0;
			for(j = 0; j < outColCom.sameColumnMatrix.size(); ++j)
			{
				b = true;
				difference = 0;
				for(k = 0; k < inColCom.sameColumnMatrix[i].size(); ++k)
				{
					if(inColCom.sameColumnMatrix[i][k] != outColCom.sameColumnMatrix[j][k])
					{
						difference++;
						b = false;
						//break;
					}
				}
				if(difference < mindiff)
				{
					diffLocation = j;
				}
				if(b == true)
				{
					break;
				}
			}
			if(b == true)/*(j != outColCom.sameColumnMatrix.size())*/
			{
				/*ReplaceRowMatchValue(inColCom, i, j);*/
				AnotherReplaceRowMatchValue(inColCom, originRow, i, j);
				minus++;
			}
			else
			{
				std::vector<ushort> row = inColCom.sameColumnMatrix[i];
				if(inColCom.column < outColCom.column)
				{
					for(k = inColCom.column; k < outColCom.column; ++k)
					{
						row.push_back(outColCom.sameColumnMatrix[diffLocation][k]);
					}
				}
				outColCom.sameColumnMatrix.push_back(row);
				/*ReplaceRowMatchValue(inColCom, i, outColCom.sameColumnMatrix.size() - 1);*/
				AnotherReplaceRowMatchValue(inColCom, originRow, i, outColCom.sameColumnMatrix.size() - 1);
			}
		}
		inColCom.rowTrans = originRow;
	}
	return minus;
}

void DiffColDfaCombine(COLCOMBINEARRAY &colCombineArray, ushort minCol, ushort maxCol, COLUMNCOMBINE &outCombineArray)/*, CDfaArray &SameColDfa, const size_t l, const size_t h, std::map<ushort, Attribute>::iterator upper, CDfa &outComDfa)*/
{
	std::fstream fcom("combine result.txt", std::ios::app | std::ios::out);
	int origin, now, minus;
	origin = now = minus = 0;

	size_t l = 0, h = 0;
	COLCOMBINEARRAY_ITERATOR colCombineArray_it;
	
	for(colCombineArray_it = colCombineArray.begin(); colCombineArray_it->column != maxCol; ++colCombineArray_it)
	{
		++h;
	}

	for(colCombineArray_it = colCombineArray.begin(); colCombineArray_it->column != minCol; ++colCombineArray_it)
	{
		++l;
	}

	//outComDfa = SameColDfa[h];
	/*std::map<ushort, Attribute>::iterator it = upper;*/
	for(size_t i = h; i >= l; --i)
	{
		origin += colCombineArray[i].sameColumnMatrix.size();
		minus += TwoColDfaCombine(colCombineArray[i], outCombineArray);
		//colCombineArray_it++;
	}
	now = outCombineArray.sameColumnMatrix.size();
	fcom.width(7);
	//fcom << maxCol << "  " << minCol << origin << fcom.width(7) << minus << fcom.width(7) << now << std::endl;
	fcom << maxCol << "  " << minCol << "\t" << origin << "\t" <<  minus << "\t" <<  now << std::endl;
	fcom.close();
}