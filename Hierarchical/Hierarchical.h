#pragma once

#include <hwprj\groupres.h>
#include <iostream>
#include <vector>
#include <Windows.h>

typedef std::vector<BYTE> ROWSET;
typedef std::vector<double> GRAPH;
typedef std::vector<ROWSET> VECROWSET;
typedef std::vector<BYTE> VISUALROW;  
typedef std::vector<VISUALROW> VISUALSET;

//建图
void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph);

//查找连通子图
void SearchConSubGraph(const GRAPH &graph, VECROWSET &vecRows);

//层次聚类算法
void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows);
