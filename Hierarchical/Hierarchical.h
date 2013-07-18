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

//��ͼ
void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph);

//������ͨ��ͼ
void SearchConSubGraph(const GRAPH &graph, VECROWSET &vecRows);

//��ξ����㷨
void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows);
