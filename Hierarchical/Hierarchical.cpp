#include "Hierarchical.h"

size_t maxVal = 0;
ushort threshold = 2;

void BuildGraph(const std::vector<std::vector<ushort> > &dfaMatrix, GRAPH &graph)
{
	int row = dfaMatrix.size();
	int col = dfaMatrix[0].size();
	const int weight = col - threshold;

	graph.resize(row * row);
	std::fill(graph.begin(), graph.end(), 0);

	for (int i = 0; i < row; ++i)
	{
		for (int j = i + 1; j < row; ++j)
		{
			int nEqualCnt = 0;
			for (int k = 0; k < col; ++k)
			{
				if ((dfaMatrix[i][k] == dfaMatrix[j][k]))
				{
					++nEqualCnt;
				}
			}
			if(nEqualCnt >= weight)
				graph[i * row + j] = graph[j * row + i] = nEqualCnt;
			else
				graph[i * row + j] = graph[j * row + i] = 0;
		}
	}
}

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

void SearchConnectSubgraph(const GRAPH &graph, ROWSET &nodes, size_t limit, VECROWSET &partRows)
{
	size_t nVexCnt = sqrt(graph.size());
	if (nVexCnt * nVexCnt != graph.size())
	{
		throw 0;
	}

	ROWSET visit;
	visit.resize(nVexCnt);
	std::fill(visit.begin(), visit.end(), 0);

	for (size_t i = 0; i < nodes.size(); ++i)
	{
		if (visit[nodes[i]] == 0)
		{
			partRows.push_back(ROWSET());
			DFS_Visit(graph, nodes[i], limit, nodes, visit, partRows.back());
		}
	}
}

void StatisticVitualCore(const std::vector<std::vector<ushort> > &dfaMatrix, const ROWSET &partRow, ROWSET &corRow)
{
	int row = partRow.size();
	int col = dfaMatrix[0].size();

	for (int i = 0; i < col; ++i)
	{
		std::map<ushort, int> bary;

		for (int j = 0; j< row; ++j)
		{
			++bary[dfaMatrix[partRow[j]][i]];
		}

		ushort maxindex = bary.begin()->first;
		int max = bary.begin()->second;
		for (std::map<ushort, int>::iterator i = bary.begin(); i != bary.end(); ++i)
		{
			if (i->second > max)
			{
				max = i->second;
				maxindex = i->first;
			}
		}
		corRow.push_back(maxindex);
	}
}

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

void AnotherReplaceRowMatchValue(COLUMNCOMBINE &inColCom, std::vector<ROWTRANSFORM> &row, ushort old, ushort now)
{
	for(size_t i = 0; i < inColCom.rowTrans.size(); ++i)
	{
		ROWTRANSFORM &cur =  inColCom.rowTrans[i];
		size_t temp = cur.rowTransform.size();
		ROWNODE *p = cur.rowTransform.data();
		for(size_t j = 0; j < temp; ++j)
		{
			if(p[j].newNum == old)
			{
				p[j].newNum = now;
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
		colCombineArray[i].rowTrans = row;
		colCombineArray[i].sameColumnMatrix = diff;
	}
}



void SameColDfaCombine(CDfaArray &CDfaSet, COLCOMBINEARRAY &colCombineArray)
{
	ushort column, colLocation;

	std::vector<int> origin, minus, now;

	//CGroupRes groupRes;
	//groupRes.ReadFromFile(/*"FinalResult.cdt"*/);
	//CDfaArray &CDfaSet = groupRes.GetDfaTable();

	std::set<ushort> allCol;
	std::set<ushort>::iterator col_it, col;
	COLUMNCOMBINE sameColInform;
	for (size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		allCol.insert(CDfaSet[i].GetGroupCount());
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
		column = CDfaSet[i].GetGroupCount();
		col_it = allCol.find(column);

		colLocation = 0;
		for(col = allCol.begin(); col != col_it; col++, colLocation++) ;

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
			rownode.skipNode = NULL;
			rowMatch.rowTransform.push_back(rownode);
			k++;
			colCombineArray[colLocation].sameColumnMatrix.push_back(v);
		}
		colCombineArray[colLocation].rowTrans.push_back(rowMatch);
	}
	size_t ts = 0, cs = 0;
	for(std::vector<COLUMNCOMBINE>::iterator i = colCombineArray.begin(); i != colCombineArray.end(); ++i)
	{
		ts += i->sameColumnMatrix.size();
		cs += ts * i->column;
	}
	std::cout << "total states : " << ts << std::endl;
	std::cout << "before compress : " << static_cast<double>(cs) * 2.0 / pow(1024.0, 2) << "MB" << std::endl;
	RemoveTheSame(colCombineArray);
}

void ReplaceRowMatchValue(COLUMNCOMBINE &inColCom, ushort old, ushort now)
{
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
			if(b == true)
			{
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

	for(size_t i = h; i >= l; --i)
	{
		origin += colCombineArray[i].sameColumnMatrix.size();
		minus += TwoColDfaCombine(colCombineArray[i], outCombineArray);
	}
	now = outCombineArray.sameColumnMatrix.size();
	fcom.width(7);
	fcom << maxCol << "  " << minCol << "\t" << origin << "\t" <<  minus << "\t" <<  now << std::endl;
	fcom.close();
}

bool Estimate(const std::vector<std::vector<ushort> > &dfaMatrix, const ROWSET &partRow, ROWSET &corRow)
{
	const int row = partRow.size();
	const int col = dfaMatrix[0].size();
	for(int i = 0; i != row; ++i)
	{	
		int count = 0;
		for(int j = 0; j != col; ++j)
		{
			if(dfaMatrix[partRow[i]][j] != corRow[j])
				++count;
		}
		if(count > threshold)
			return false;
	}
	return true;
}

int RandNum(const std::vector<int> &temp, int col)
{
	int num;
	while(true)
	{
		num = rand() % col;
		std::vector<int>::const_iterator i = temp.begin();
		for(; i != temp.end(); ++i)
		{
			if(num == *i)
				break;
		}
		if(i == temp.end())
		{
			return num;
		}
	}
}

void update(const VECROWSET &partRows, const VECROWSET &corRows, COLUMNCOMBINE &dfaData, std::vector<std::vector<SKIPNODE> > &skipTable)
{
	const std::vector<std::vector<ushort> > &dfaMatrix = dfaData.sameColumnMatrix; 
	std::vector<ROWTRANSFORM> &dfaRows = dfaData.rowTrans;
	const int row = dfaMatrix.size();
	const int col = dfaData.column;

	for(std::vector<ROWTRANSFORM>::iterator k = dfaRows.begin(); k != dfaRows.end(); ++k)
	{
		for(std::vector<ROWNODE>::iterator l = k->rowTransform.begin(); l != k->rowTransform.end(); ++l)
		{
			std::vector<SKIPNODE> skipRow;
			for(size_t i = 0; i != partRows.size(); ++i)
			{
				if(corRows[i] != ROWSET() && partRows[i].size() != 1)
				{
					for(size_t j = 0; j != partRows[i].size(); ++j)
					{
						if(partRows[i][j] == static_cast<size_t>(l->newNum))
						{
							std::vector<int> temp;
							for(size_t m = 0; m != col; ++m)
							{
								if(dfaMatrix[partRows[i][j]][m] != corRows[i][m])
								{
									SKIPNODE skip;
									skip.jumpCharacter = static_cast<char>(m);
									skip.nextNode = dfaMatrix[partRows[i][j]][m];
									skipRow.push_back(skip);
									temp.push_back(m);
								}
							}
							for(int m = 0; m != threshold - temp.size(); ++m)
							{
								int ranNum = RandNum(temp, col);
								SKIPNODE skip;
								skip.jumpCharacter = static_cast<char>(ranNum);
								skip.nextNode = dfaMatrix[partRows[i][j]][ranNum];
								skipRow.push_back(skip);
							}
							skipTable.push_back(skipRow);
							l->skipNode = &skipTable.back();
							l->newNum = i;
							break;
						}
					}
					break;
				}
			}
		}
	}
}

void fileout(const VECROWSET &partRows, const VECROWSET &corRows, COLUMNCOMBINE &dfaData, std::vector<std::vector<SKIPNODE> > &skipTable, size_t &mergeSR, size_t &unmergeSR, size_t &corMR)
{
	std::fstream fout("final.txt", std::ios::app | std::ios::out);
	size_t stateNum = dfaData.sameColumnMatrix.size();
	fout <<"col : " << dfaData.column << std::endl;
	fout <<"corMatrix : " << corRows.size() << std::endl;
	corMR += corRows.size() * dfaData.column;
	size_t merge = 0, unmerge = 0;
	for(size_t i = 0; i != corRows.size(); ++i)
	{
		if(corRows[i] == ROWSET())
		{
			unmerge += partRows[i].size();
			unmergeSR += partRows[i].size();
		}
		else
		{
			merge += partRows[i].size();
			mergeSR += partRows[i].size();
		}
	}
	double compressRate = static_cast<double>(merge) / static_cast<double>(stateNum) * 100.0;
	fout << "sum of state : " << stateNum << std::endl;
	fout << "merge state : " << merge << std::endl;
	fout << "unmerge state : " << unmerge << std::endl;
	fout << "compress rate : " << compressRate << '%' << std::endl;
}

void skipout(std::vector<std::vector<SKIPNODE> > &skipTable)
{
	std::fstream fout("skipTable.txt", std::ios::out | std::ios::binary);
	fout << "skipTable : " << std::endl;
	for(std::vector<std::vector<SKIPNODE> >::iterator i = skipTable.begin(); i != skipTable.end(); ++i)
	{
		for(std::vector<SKIPNODE>::iterator j = i->begin(); j != i->end(); ++j)
		{
			fout << j->jumpCharacter << " " << j->nextNode << "\t";
		}
		fout << std::endl;
	}
}

//void CoreCompress(std::vector<COLUMNCOMBINE> &allData)
//{
//	std::vector<std::vector<SKIPNODE> > skipTable;
//	size_t st = 0, cr = 0;
//	int count = 0;
//	for(std::vector<COLUMNCOMBINE>::iterator i = allData.begin(); i != allData.end(); ++i)
//	{
//		std::vector<std::vector<ushort> > &dfaMatrix = i->sameColumnMatrix;
//		const ushort col = i->column;
//		const ushort weight = col - threshold;
//		GRAPH graph;
//		BuildGraph(dfaMatrix, graph);
//		ROWSET nodes;
//		for(int j = 0; j != dfaMatrix.size(); ++j)
//			nodes.push_back(j);
//		VECROWSET partRows;
//		SearchConnectSubgraph(graph, nodes, weight, partRows);
//		VECROWSET corMatrix;
//		bool sign = true;
//		for(VECROWSET::iterator j = partRows.begin(); j != partRows.end(); ++j)
//		{
//			ROWSET corRow;
//			StatisticVitualCore(dfaMatrix, *j, corRow);
//			bool mem = Estimate(dfaMatrix, *j, corRow);
//			if(mem == true)
//				corMatrix.push_back(corRow);
//			else
//			{
//
//			}
//		}
//		update(partRows, corMatrix, *i, skipTable);
//		st += i->sameColumnMatrix.size();
//		cr += corMatrix.size() * i->column;
//	}
//	skipout(skipTable);
//	std::cout << "Different column numbers : " << allData.size() << std::endl;
//	std::cout << "all kerinel matrix : " << static_cast<double>(cr) * 2.0 / pow(1024.0, 2) << "MB" << std::endl;
//	std::cout << "all transition table : " << static_cast<double>(st) * 6.0 / pow(1024.0, 2) << "MB" << std::endl;
//	std::cout << "!!!" << count << std::endl;
//}

void PartitionSameColumn(COLUMNCOMBINE &dfas, std::vector<std::vector<SKIPNODE> > &skipTable)
{
	std::vector<std::vector<size_t> > part;
	part.push_back(std::vector<size_t>());
	for(size_t i = 0; i != dfas.sameColumnMatrix.size(); ++i)
	{
		part.back().push_back(i);
	}
	std::vector<std::vector<ushort> > &dfaMatrix = dfas.sameColumnMatrix;
	const ushort col = dfas.column;
	GRAPH graph;
	BuildGraph(dfaMatrix, graph);
	VECROWSET corMatrix;
	for(size_t i = 0; i != part.size();)
	{
		std::vector<size_t> &nodes = part[i];
		VECROWSET partRows;
		for(int weight = col - threshold; weight != col; ++weight)
		{
			partRows.clear();
			corMatrix.clear();
			SearchConnectSubgraph(graph, nodes, weight, partRows);
			if(partRows.size() == 1)
			{
				ROWSET corRow;
				StatisticVitualCore(dfaMatrix, partRows[0], corRow);
				if(Estimate(dfaMatrix, partRows[0], corRow))
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		for(VECROWSET::iterator j = partRows.begin(); j != partRows.end(); ++j)
		{
			ROWSET corRow;
			StatisticVitualCore(dfaMatrix, *j, corRow);
			bool mem = Estimate(dfaMatrix, *j, corRow);
			if(mem == true)
			{
				corMatrix.insert(corMatrix.begin() + i, corRow);
				part.insert(part.begin() + i, *j);
				++i;
			}
			else
			{
				part.push_back(*j);
			}
		}
		part.erase(part.begin() + i);
	}
	update(part, corMatrix, dfas, skipTable);
}

void PartitionGraph(std::vector<COLUMNCOMBINE> &allData, std::vector<std::vector<SKIPNODE> > &skipTable)
{
	for(std::vector<COLUMNCOMBINE>::iterator i = allData.begin(); i != allData.end(); ++i)
	{
		PartitionSameColumn(*i, skipTable);
	}
}