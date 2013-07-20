#pragma once

#include <hwprj\groupres.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <algorithm>
#include <cmath>

#define limit 0.5

typedef std::vector<BYTE> ROWSET;
typedef std::vector<float> GRAPH;
typedef std::vector<ROWSET> VECROWSET;
typedef VECROWSET::iterator NODEARRAY_ITER;

extern ulong g_TotalMem;

//��ͼ
void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph);

//������ͨ��ͼ
void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows);

//ͳ������� ,����洢�ռ�,ÿ��һ���м�
size_t StatisticVitualCore(const CDfa &oneDfa,ROWSET &rs);

//��ξ����㷨
void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows);
