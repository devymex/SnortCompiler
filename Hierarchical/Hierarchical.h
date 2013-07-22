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


//��ͼ
void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph);

//������ͨ��ͼ
void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows);

//ͳ������� ,����洢�ռ�,ÿ��һ���м�
size_t StatisticVitualCore(const CDfa &oneDfa,ROWSET &rs);

//��ξ����㷨
void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows);

//��Ǻ�����������Ǹ�״̬�Ƿ���״̬���г���
int SignState(byte state, ROWSET stateSet);

//MA����,removeSetΪ���Ƴ���״̬����matrixΪ��ȫ����oriSetΪ���������״̬��
void MinArray(ROWSET removeSet,GRAPH matrix, ROWSET &oriSet);

//״̬�ϲ���removeSet��ʾ�Ƴ���״̬������Ҫ���£�s��t״̬Ϊ�������״̬��brokenMartix��¼��Ȩ����ĸ��£�vecSets��¼״̬���ĸ���
int MergeState(byte s, byte t, ROWSET &removeSet, GRAPH &matrix, VECROWSET &sets);

//�������С���㷨
void StoreWagner(GRAPH matrix, VECROWSET &result);