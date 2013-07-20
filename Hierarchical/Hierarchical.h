#pragma once

#include <hwprj\groupres.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <algorithm>
#include <cmath>

typedef std::vector<BYTE> ROWSET;
typedef std::vector<double> GRAPH;
typedef std::vector<ROWSET> VECROWSET;

//建图
void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph);

//查找连通子图
void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows);

//统计虚拟核 ,计算存储空间,每次一个行集
size_t StatisticVitualCore(const CDfa &oneDfa,ROWSET &rs);

//层次聚类算法
void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows);
