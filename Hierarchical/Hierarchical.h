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

//建无向图
void BuildGraph(const CDfa &oneDfa, GRAPH &graph, ROWSET &weightArg);

//划分连通子图
void SplitGraph(CDfa &oneDFA, GRAPH &graph, ROWSET &weightArg, std::vector<BLOCK> &blocks);

//统计占用的内存空间及虚拟核
size_t StatisticMemory(const CDfa &oneDfa, const std::vector<BLOCK> &blocks, VECROWSET &vecCore);


