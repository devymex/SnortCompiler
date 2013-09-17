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



//#define limit 0.5

typedef std::vector<size_t> ROWSET;
//typedef std::vector<float> GRAPH;
typedef std::vector<size_t> GRAPH;
typedef std::vector<ROWSET> VECROWSET;
typedef VECROWSET::iterator NODEARRAY_ITER;

struct BLOCK
{
	size_t weightIdx;
	std::vector<size_t> nodes;
};

extern size_t maxVal;

////��ͼ
//void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph);
//
////������ͨ��ͼ
//void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows);
//
////ͳ������� ,����洢�ռ�,ÿ��һ���м�
//size_t StatisticVitualCore(const CDfa &oneDfa, ROWSET &rs, ROWSET &visrow);
//
////��ξ����㷨
//size_t HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows, VECROWSET &vecVirtual);

//չ��DFA
void UnflodDFA(CDfa &flodDfa, CDfa &unflodDfa);

//������ͼ
void BuildGraph(const CDfa &oneDfa, GRAPH &graph, ROWSET &weightArg);

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

size_t SortCharset(VECROWSET &allCharset, size_t threshold);


