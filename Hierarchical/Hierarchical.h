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

typedef std::vector<size_t> ROWSET;
typedef std::vector<size_t> GRAPH;
typedef std::vector<ROWSET> VECROWSET;
typedef VECROWSET::iterator NODEARRAY_ITER;
typedef unsigned char byte;
typedef unsigned int uint;

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

typedef struct
{
	ushort column;
	std::vector<std::vector<ushort> > sameColumnMatrix;
	std::vector<ROWTRANSFORM> rowTrans;
} COLUMNCOMBINE;

typedef std::vector<COLUMNCOMBINE> COLCOMBINEARRAY;
typedef std::vector<COLUMNCOMBINE>::iterator COLCOMBINEARRAY_ITERATOR;

struct BLOCK
{
	size_t weightIdx;
	std::vector<size_t> nodes;
};

extern size_t maxVal;

void UnflodDFA(CDfa &flodDfa, CDfa &unflodDfa);

void BuildGraph(const std::vector<std::vector<ushort> > &dfaMatrix, GRAPH &graph);

void SplitGraph(CDfa &oneDFA, GRAPH &graph, ROWSET &weightArg, std::vector<BLOCK> &blocks);

size_t StatisticMemory(const CDfa &oneDfa, const std::vector<BLOCK> &blocks, VECROWSET &vecCore);

void CreateNewMap(VECROWSET &allCharset, VECROWSET &newCharset, std::vector<std::map<size_t, size_t>> &mapvO2N);

void AdjustDfa(CDfaArray &DfaArr, std::vector<std::map<size_t, size_t>> &mapv);

size_t Charset(CDfa &dfa);

void ColMergeCompress(VECROWSET &vecCores, ulong colCnt, byte* colGroup, ulong &colNum, std::vector<CDfaRow> &FinalMatrix);

void StatisticVitualCore(const std::vector<std::vector<ushort> > &dfaMatrix, const ROWSET &partRow, ROWSET &corRow);

void AnotherReplaceRowMatchValue(COLUMNCOMBINE &inColCom, std::vector<ROWTRANSFORM> &row, ushort old, ushort now);

void RemoveTheSame(COLCOMBINEARRAY &colCombineArray);

bool equal(CDfaRow &row, std::vector<ushort> vec);

void SameColDfaCombine(CDfaArray &CDfaSet, COLCOMBINEARRAY &colCombineArray);

void ReplaceRowMatchValue(COLUMNCOMBINE &inColCom, ushort old, ushort now);

int TwoColDfaCombine(COLUMNCOMBINE &inColCom, COLUMNCOMBINE &outColCom);

void DiffColDfaCombine(COLCOMBINEARRAY &colCombineArray, ushort minCol, ushort maxCol, COLUMNCOMBINE &outCombineArray);

void CoreCompress(std::vector<COLUMNCOMBINE> &allData);