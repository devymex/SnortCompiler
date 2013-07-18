#pragma once

#include <hwprj\groupres.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <algorithm>

typedef std::vector<BYTE> ROWSET;
typedef std::vector<double> GRAPH;
typedef std::vector<ROWSET> VECROWSET;

//��ͼ
void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph);

//������ͨ��ͼ
void SearchConnectSubgraph(const GRAPH &graph, ROWSET &curRow, VECROWSET &vecRows);

//��ξ����㷨
void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows);
