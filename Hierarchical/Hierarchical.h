#pragma once

#include <hwprj\groupres.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <algorithm>

typedef std::vector<BYTE> ROWSET;
typedef std::vector<double> GRAPH;
typedef std::vector<ROWSET> VECROWSET;
typedef std::vector<BYTE> VISUALROW;  
typedef std::vector<VISUALROW> VISUALSET;

//��ͼ
void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph);

//������ͨ��ͼ
void SearchConnectSubgraph(const GRAPH &graph, ROWSET &curRow, VECROWSET &vecRows);

//ͳ������� ,����洢�ռ�,ÿ��һ���м�
size_t StatisticVitualCore(const CDfa &oneDfa,ROWSET &rs);

//��ξ����㷨
void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows);
