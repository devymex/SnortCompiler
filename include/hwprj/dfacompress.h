#pragma once

#include <vector>
#include <hwprj\dfa.h>
#include <hwprj\dfa.h>
#include <hwprj\unsary.h>

#ifndef DFACOMPRESS_DS
#define DFACOMPRESS __declspec(dllimport)
#else
#define DFACOMPRESS __declspec(dllexport)
#endif

DFACOMPRESS void DfaCompress(CDfa &olddfa, ulong &sumBytes);
void GetDfaCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, ulong *stateCluster, ulong *codeMap);//获得dfa的簇信息
//根据簇，更新dfa矩阵，得到簇矩阵dfaMatrix，稀疏矩阵sparseMatrix以及簇矩阵的base
void UpdateMatrix(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, ulong *stateCluster,
				  ulong *codeMap, std::vector<CDfaRow> &dfaMatrix, 
				  std::vector<STATEID> &base, std::vector<CDfaRow> &sparseMatrix);

//对dfaMatrix进行行合并压缩
void RowMergeCompress(std::vector<CDfaRow> &dfaMatrix, std::vector<STATEID> &rowGroup, 
					   std::vector<CDfaRow> &MatrixAfterRowMerge);
//对MatrixAfterRowMerge进行列压缩
void ColMergeCompress(CDfa &olddfa, byte* colGroup, ulong &colNum, std::vector<CDfaRow> &FinalMatrix);

void OutPutCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, ulong *stateCluster, ulong *codeMap);//输出测试

void OutputMatrix(std::vector<STATEID> &base, std::vector<CDfaRow> &dfaMatrix, std::vector<CDfaRow> &sparseMatrix);//输出提取base后的矩阵以及稀疏矩阵

void OutPutClusterFinalMatrix(std::vector<CDfaRow> &dfaMatrix, std::vector<STATEID> &rowGroup, 
					   std::vector<CDfaRow> &FinalMatrix);