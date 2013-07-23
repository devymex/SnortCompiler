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

//������ͼ
void BuildGraph(const CDfa &oneDfa, GRAPH &graph, ROWSET &weightArg);

//������ͨ��ͼ
void SplitGraph(CDfa &oneDFA, GRAPH &graph, ROWSET &weightArg, std::vector<BLOCK> &blocks);

//ͳ��ռ�õ��ڴ�ռ估�����
size_t StatisticMemory(const CDfa &oneDfa, const std::vector<BLOCK> &blocks, VECROWSET &vecCore);


