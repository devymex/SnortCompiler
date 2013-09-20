#pragma once

#include <hwprj\groupres.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <algorithm>
#include <cmath>
#include <map>
#include <fstream>
#include <unordered_map>

#include <set>

//#define limit 0.5

typedef std::vector<size_t> ROWSET;
//typedef std::vector<float> GRAPH;
typedef std::vector<size_t> GRAPH;
typedef std::vector<ROWSET> VECROWSET;
typedef VECROWSET::iterator NODEARRAY_ITER;

///////////////////////////////////////////////////////////////////////
//typedef std::vector<ushort> rowMatch;
//typedef struct
//{
//	bool pushed;
//	/*std::vector<ulong> dfaID;
//	CDfa rowMatch;*/
//	//std::map<ulong, CDfaRow> id_rowMatch;
//	std::map<ulong, rowMatch> id_rowMatch;
//} Attribute;

//////////////////////////////////////////////////////////////////////////
typedef struct
{
	std::vector<std::vector<ushort> > *m_t;
	int n;
} SORTASSITANT;

struct ROWSORT
{
	
	ROWSORT() { }
	bool operator()(SORTASSITANT p1, SORTASSITANT p2)
	{
		return (*p1.m_t)[p1.n] < (*p2.m_t)[p2.n];
	}
};
typedef struct
{
	char jumpCharacter;
	ushort nextNode;
} SKIPNODE;

typedef struct
{
	ushort newNum;
	std::vector<SKIPNODE> *skipNode;
}ROWNODE;

typedef struct
{
	ushort dfaId;
	std::vector<ROWNODE> rowTransform;
} ROWTRANSFORM;

//typedef struct
//{
//	ushort dfaId;
//	std::vector<ushort> rowTransform;
//	std::vector<SKIPNODE> skipNode;
//} ROWTRANSFORM;

typedef struct
{
	ushort column;
	std::vector<std::vector<ushort> > sameColumnMatrix;
	std::vector<ROWTRANSFORM> rowTrans;
} COLUMNCOMBINE;

typedef std::vector<COLUMNCOMBINE> COLCOMBINEARRAY;
typedef std::vector<COLUMNCOMBINE>::iterator COLCOMBINEARRAY_ITERATOR;

///////////////////////////////////////////////////////////////////////

struct BLOCK
{
	size_t weightIdx;
	std::vector<size_t> nodes;
};

extern size_t maxVal;

////建图
//void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph);
//
////查找连通子图
//void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows);
//
////统计虚拟核 ,计算存储空间,每次一个行集
//size_t StatisticVitualCore(const CDfa &oneDfa, ROWSET &rs, ROWSET &visrow);
//
////层次聚类算法
//size_t HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows, VECROWSET &vecVirtual);

//展开DFA
void UnflodDFA(CDfa &flodDfa, CDfa &unflodDfa);

//建无向图
void BuildGraph(const CDfa &oneDfa, GRAPH &graph, ROWSET &weightArg);

//划分连通子图
void SplitGraph(CDfa &oneDFA, GRAPH &graph, ROWSET &weightArg, std::vector<BLOCK> &blocks);

//统计占用的内存空间及虚拟核
size_t StatisticMemory(const CDfa &oneDfa, const std::vector<BLOCK> &blocks, VECROWSET &vecCore);

//建立新的映射
void CreateNewMap(VECROWSET &allCharset, VECROWSET &newCharset, std::vector<std::map<size_t, size_t>> &mapvO2N);

//根据新的映射调整DFA表的列
void AdjustDfa(CDfaArray &DfaArr, std::vector<std::map<size_t, size_t>> &mapv);

//非默认字符个数
size_t Charset(CDfa &dfa);

//核矩阵列字符集压缩
void ColMergeCompress(VECROWSET &vecCores, ulong colCnt, byte* colGroup, ulong &colNum, std::vector<CDfaRow> &FinalMatrix);

size_t SortCharset(VECROWSET &allCharset, size_t threshold);

void AnotherReplaceRowMatchValue(COLUMNCOMBINE &inColCom, std::vector<ROWTRANSFORM> &row, ushort old, ushort now);

void RemoveTheSame(COLCOMBINEARRAY &colCombineArray);

bool equal(CDfaRow &row, std::vector<ushort> vec);

void SameColDfaCombine(COLCOMBINEARRAY &colCombineArray);

//void DiffColDfaCombine(CDfaArray &SameColDfa, std::map<ushort, Attribute> &columnNum, std::map<ushort, Attribute>::iterator lower, std::map<ushort, Attribute>::iterator upper);

void ReplaceRowMatchValue(COLUMNCOMBINE &inColCom, ushort old, ushort now);

int TwoColDfaCombine(COLUMNCOMBINE &inColCom, COLUMNCOMBINE &outColCom);

void DiffColDfaCombine(COLCOMBINEARRAY &colCombineArray, ushort minCol, ushort maxCol, COLUMNCOMBINE &outCombineArray);