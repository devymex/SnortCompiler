#include "Hierarchical.h"

size_t threshold = 2;

void BuildGraph(MATRIX &dfaMatrix, std::vector<ushort> &graph)
{
	const size_t row = dfaMatrix.size();
	const size_t col = dfaMatrix[0].size();
	const ushort weight = static_cast<ushort>(col - threshold);

	graph.resize(row * row);
	std::fill(graph.begin(), graph.end(), 0);

	for (size_t i = 0; i < row; ++i)
	{
		for (size_t j = i + 1; j < row; ++j)
		{
			ushort nEqualCnt = 0;
			for (size_t k = 0; k < col; ++k)
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

int DFS_Visit(const std::vector<ushort> &graph, size_t vex, ushort weight, ROWSET &curRow, CORESPOND &visit,  size_t &rowCount, size_t &colCount)
{
	const size_t row = static_cast<size_t>(sqrt(graph.size()));
	if (visit[vex].rowNum == UINT_MAX && visit[vex].colNum == UINT_MAX)
	{
		visit[vex].rowNum = rowCount;
		visit[vex].colNum = colCount;
		++colCount;
		curRow.push_back(vex);
	}
	else
	{
		return 0;
	}
	for (size_t i = 0; i != row; ++i)
	{
		if (graph[vex * row + i] >= weight && visit[i].rowNum == UINT_MAX && visit[i].colNum == UINT_MAX)
		{
			DFS_Visit(graph, i, weight, curRow, visit, rowCount, colCount);
		}
	}
	return 1;
}

void SearchConnectSubgraph(const std::vector<ushort> &graph, ushort weight, VECROWSET &partRows, CORESPOND &visit)
{
	const size_t row = static_cast<size_t>(sqrt(graph.size()));
	visit.resize(row);
	STATENODE temp;
	temp.rowNum = UINT_MAX;
	temp.colNum = UINT_MAX;
	std::fill(visit.begin(), visit.end(), temp);

	size_t rowCount = static_cast<size_t>(-1);
	for (size_t i = 0; i < row; ++i)
	{
		if (visit[i].rowNum == UINT_MAX && visit[i].colNum == UINT_MAX)
		{
			++rowCount;
			size_t colCount = 0;
			partRows.push_back(ROWSET());
			DFS_Visit(graph, i, weight, partRows.back(), visit, rowCount, colCount);
		}
	}
}

void StatisticVitualCore(const MATRIX &dfaMatrix, const ROWSET &partRow, ROW &corRow)
{
	const size_t row = partRow.size();
	const size_t col = dfaMatrix[0].size();

	for (size_t i = 0; i < col; ++i)
	{
		std::map<ushort, int> bary;

		for (size_t j = 0; j< row; ++j)
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

void SameColDfaCombine(CDfaArray &CDfaSet, COLCOMBINEARRAY &colCombineArray)
{
	ushort column, colLocation;

	std::vector<int> origin, minus, now;

	std::set<ushort> allCol;
	std::set<ushort>::iterator col_it, col;
	COLUMNCOMBINE sameColInform;

	for (size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		allCol.insert(CDfaSet[i].GetGroupCount());
		CDfaSet[i].SetId(i);
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
		for(col = allCol.begin(); col != col_it; ++col, ++colLocation) ;

		origin[colLocation] += CDfaSet[i].Size();

		ROWTRANSFORM rowMatch;
		rowMatch.dfaId = CDfaSet[i].GetId();
		rowMatch.skipTableNum = 0;
		size_t originSize, k = colCombineArray[colLocation].sameColumnMatrix.size();
		for(size_t j = 0; j < CDfaSet[i].Size(); ++j)
		{
			std::vector<ushort> v;
			for(size_t t = 0; t < CDfaSet[i][j].Size(); ++t)
			{
				v.push_back(CDfaSet[i][j][t]);
			}
			rowMatch.rowTransform.push_back(k);
			++k;
			colCombineArray[colLocation].sameColumnMatrix.push_back(v);
		}
		colCombineArray[colLocation].rowTrans.push_back(rowMatch);
	}
}

bool Estimate(const MATRIX &dfaMatrix, const ROWSET &partRow, ROW &corRow)
{
	const size_t row = partRow.size();
	const size_t col = dfaMatrix[0].size();
	for(size_t i = 0; i != row; ++i)
	{	
		size_t count = 0;
		for(size_t j = 0; j != col; ++j)
		{
			if(dfaMatrix[partRow[i]][j] != corRow[j])
				++count;
		}
		if(count > threshold)
			return false;
	}
	return true;
}

size_t RandNum(const ROWSET &temp, size_t col)
{
	size_t num;
	while(true)
	{
		num = rand() % col;
		std::vector<size_t>::const_iterator i = temp.begin();
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

void Update(const VECROWSET &partRows, const MATRIX &corMatrix, COLUMNCOMBINE &dfaData, std::vector<std::vector<SKIPNODE> > &skipTable, CORESPOND &visit)
{
	const std::vector<std::vector<ushort> > &dfaMatrix = dfaData.sameColumnMatrix; 
	std::vector<ROWTRANSFORM> &dfaRows = dfaData.rowTrans;
	const int row = dfaMatrix.size();
	const int col = dfaData.column;
	int beg = skipTable.size();
	for(size_t i = 0; i != dfaRows.size(); ++i)
	{
		dfaRows[i].skipTableNum = skipTable.size();
		for(size_t j = 0; j != dfaRows[i].rowTransform.size(); ++j)
		{
			std::vector<SKIPNODE> skipRow;
			skipTable.push_back(skipRow);
		}
	}

	for(std::vector<ROWTRANSFORM>::iterator k = dfaRows.begin(); k != dfaRows.end(); ++k)
	{
		for(ROW::iterator l = k->rowTransform.begin(); l != k->rowTransform.end(); ++l)
		{
			size_t r = visit[*l].rowNum, c = visit[*l].colNum;
			std::vector<SKIPNODE> &skipRow = skipTable[beg + partRows[r][c]];
			ROWSET temp;
			for(size_t m = 0; m != col; ++m)
			{
				if(dfaMatrix[partRows[r][c]][m] != corMatrix[r][m])
				{
					SKIPNODE skip;
					skip.jumpCharacter = static_cast<char>(m);
					skip.nextNode = dfaMatrix[partRows[r][c]][m];
					skipRow.push_back(skip);
					temp.push_back(m);
				}
			}
			size_t p = threshold - temp.size();
			for(size_t m = 0; m != p; ++m)
			{
				size_t ranNum = RandNum(temp, col);
				SKIPNODE skip;
				skip.jumpCharacter = static_cast<char>(ranNum);
				skip.nextNode = dfaMatrix[partRows[r][c]][ranNum];
				skipRow.push_back(skip);
				temp.push_back(ranNum);
			}
			*l = r;
		}
	}
}

void Exception(const MATRIX &dfaMatrix, VECROWSET &partRows, MATRIX &corMatrix, CORESPOND &visit, VECROWSET::iterator &iter, size_t &rowNumber)
{
	const size_t col = dfaMatrix[0].size();
	const size_t n = iter->size();
	ROWSET temp;
	temp.assign(iter->begin(), iter->end());
	for(ROWSET::reverse_iterator i = temp.rbegin(); i != temp.rend(); ++i)
	{
		ROWSET member;
		member.push_back(*i);
		iter = partRows.insert(iter, member);
	}
	size_t number = 0;
	for(ROWSET::iterator i = temp.begin(); i != temp.end(); ++i)
	{
		ROW corRow;
		for(size_t j = 0; j != col; ++j)
		{
			corRow.push_back(dfaMatrix[*i][j]);
		}
		corMatrix.push_back(corRow);
		visit[temp[number]].rowNum = rowNumber + number;
		visit[temp[number]].colNum = 0;
		++number;
	}
	rowNumber += number;
	iter = partRows.erase(iter + n);
	for(VECROWSET::iterator i = iter; i != partRows.end(); ++i)
	{
		for(ROWSET::iterator j = i->begin(); j != i->end(); ++j)
		{
			visit[*j].rowNum += number - 1;
		}
	}
}

void CoreCompress(std::vector<COLUMNCOMBINE> &allData, std::vector<std::vector<SKIPNODE> > &skipTable)
{
	for(std::vector<COLUMNCOMBINE>::iterator i = allData.begin(); i != allData.end(); ++i)
	{
		std::vector<std::vector<ushort> > &dfaMatrix = i->sameColumnMatrix;
		const ushort col = i->column;
		const ushort weight = col - threshold;
		std::vector<ushort> graph;
		BuildGraph(dfaMatrix, graph);
		ROWSET nodes;
		VECROWSET partRows;
		CORESPOND visit;
		SearchConnectSubgraph(graph, weight, partRows, visit);
		MATRIX corMatrix;
		VECROWSET::iterator j = partRows.begin();
		size_t rowNumber = 0;
		while(j != partRows.end())
		{
			ROW corRow;
			StatisticVitualCore(dfaMatrix, *j, corRow);
			if(Estimate(dfaMatrix, *j, corRow))
			{
				corMatrix.push_back(corRow);
				++j;
				++rowNumber;
			}
			else
			{
				Exception(dfaMatrix, partRows, corMatrix, visit, j, rowNumber);
			}
		}
		Update(partRows, corMatrix, *i, skipTable, visit);
	}
}

void WriteSkipTable(std::vector<COLUMNCOMBINE> &allData,const std::vector<std::vector<SKIPNODE> > &skipTable, const char *str)
{
	std::fstream fileout(str, std::ios::out | std::ios::binary);
	ushort number = 0;
	for(std::vector<COLUMNCOMBINE>::iterator i = allData.begin(); i != allData.end(); ++i)
	{
		number += i->rowTrans.size();
	}
	fileout.write(reinterpret_cast<char *>(&number), sizeof(ushort));
	for(std::vector<COLUMNCOMBINE>::iterator j = allData.begin(); j != allData.end(); ++j)
	{
		for(std::vector<ROWTRANSFORM>::iterator k = j->rowTrans.begin(); k != j->rowTrans.end(); ++k)
		{
			fileout.write(reinterpret_cast<char *>(&(k->dfaId)), sizeof(ushort));
			ushort dfaSize = k->rowTransform.size();
			fileout.write(reinterpret_cast<char *>(&dfaSize), sizeof(ushort));
			for(ulong m = k->skipTableNum; m != k->skipTableNum + dfaSize; ++m)
			{
				for(ushort l = 0; l != threshold; ++l)
				{
					char temp = skipTable[m][l].jumpCharacter;
					fileout.write(reinterpret_cast<char *>(&temp), sizeof(char));
				}
			}
		}
	}	
}

void ReadSkipTable(const char *str, std::vector<std::vector<std::vector<char> > > &skipTable)
{
	std::fstream filein(str, std::ios::in | std::ios::binary);
	ushort number;
	filein.read(reinterpret_cast<char *>(&number), sizeof(ushort));
	for(ushort i = 0; i != number; ++i)
	{
		skipTable.push_back(std::vector<std::vector<char> >());
	}
	for(ushort i = 0; i != number; ++i)
	{
		ushort dfaId, stateNum;
		filein.read(reinterpret_cast<char *>(&dfaId), sizeof(ushort));
		filein.read(reinterpret_cast<char *>(&stateNum), sizeof(ushort));
		for(ushort j = 0; j != stateNum; ++j)
		{
			std::vector<char> skipNode;
			for(ushort k = 0; k != threshold; ++k)
			{
				char temp;
				filein.read(reinterpret_cast<char *>(&temp), sizeof(char));
				skipNode.push_back(temp);
			}
			skipTable[dfaId].push_back(skipNode);
		}
	}
}