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
typedef unsigned char byte;
typedef unsigned int uint;

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
///////////////////////////////////////////////////////////////////////

struct BLOCK
{
	size_t weightIdx;
	std::vector<size_t> nodes;
};

extern size_t maxVal;

//չ��DFA
void UnflodDFA(CDfa &flodDfa, CDfa &unflodDfa);

//������ͼ
void BuildGraph(std::vector<std::vector<ushort> > &dfaMatrix, GRAPH &graph);

//������ͨ��ͼ
void SplitGraph(CDfa &oneDFA, GRAPH &graph, ROWSET &weightArg, std::vector<BLOCK> &blocks);

//ͳ��ռ�õ��ڴ�ռ估�����
size_t StatisticMemory(const CDfa &oneDfa, const std::vector<BLOCK> &blocks, VECROWSET &vecCore);

//�����µ�ӳ��
void CreateNewMap(VECROWSET &allCharset, VECROWSET &newCharset, std::vector<std::map<size_t, size_t>> &mapvO2N);

//�����µ�ӳ�����DFA�����
void AdjustDfa(CDfaArray &DfaArr, std::vector<std::map<size_t, size_t>> &mapv);

//��Ĭ���ַ�����
size_t Charset(CDfa &dfa);

//�˾������ַ���ѹ��
void ColMergeCompress(VECROWSET &vecCores, ulong colCnt, byte* colGroup, ulong &colNum, std::vector<CDfaRow> &FinalMatrix);

void StatisticVitualCore(const std::vector<std::vector<ushort> > &dfaMatrix, const ROWSET &partRow, ROWSET &corRow);

size_t SortCharset(VECROWSET &allCharset, size_t threshold);

bool equal(CDfaRow &row, std::vector<ushort> vec);

void SameColDfaCombine(COLCOMBINEARRAY &colCombineArray);

bool Estimate(const std::vector<std::vector<ushort> > &dfaMatrix, const ROWSET &partRow, ROWSET &corRow);

void update(const VECROWSET &partRows, const VECROWSET &corRows, COLUMNCOMBINE &dfaData, std::vector<std::vector<SKIPNODE> > skipTable);

void ReplaceRowMatchValue(COLUMNCOMBINE &inColCom, ushort old, ushort now);

void CoreCompress(std::vector<COLUMNCOMBINE> &allData);

void TwoColDfaCombine(COLUMNCOMBINE &inColCom, COLUMNCOMBINE &outColCom);

void DiffColDfaCombine(COLCOMBINEARRAY &colCombineArray, ushort minCol, ushort maxCol, COLUMNCOMBINE &outCombineArray);