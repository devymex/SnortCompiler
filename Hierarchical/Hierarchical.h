#pragma once

#include <hwprj\groupres.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <algorithm>
#include <cmath>
#include <map>
#include <fstream>

#define limit 0.7

typedef std::vector<size_t> ROWSET;
typedef std::vector<float> GRAPH;
typedef std::vector<ROWSET> VECROWSET;
typedef VECROWSET::iterator NODEARRAY_ITER;

//extern ulong g_TotalMem;

//建图
void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph);

//查找连通子图
void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows);

//统计虚拟核 ,计算存储空间,每次一个行集
size_t StatisticVitualCore(const CDfa &oneDfa, ROWSET &rs, ROWSET &visrow);

//层次聚类算法
size_t HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows, VECROWSET &vecVirtual);

////标记函数，用来标记该状态是否在状态集中出现
//int SignState(byte state, ROWSET stateSet);
//
////MA排序,removeSet为已移除的状态集，matrix为带全矩阵，oriSet为输出的排序状态集
//void MinArray(ROWSET removeSet,GRAPH matrix, ROWSET &oriSet);
//
////状态合并，removeSet表示移除的状态集，需要更新，s和t状态为最后两个状态，brokenMartix记录带权矩阵的更新，vecSets记录状态集的更新
//int MergeState(byte s, byte t, ROWSET &removeSet, GRAPH &matrix, VECROWSET &sets);
//
////最大流最小割算法
//void StoreWagner(GRAPH matrix, VECROWSET &result);
