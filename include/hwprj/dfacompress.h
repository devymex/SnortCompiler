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
void GetDfaCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, ulong *stateCluster, ulong *codeMap);//���dfa�Ĵ���Ϣ
//���ݴأ�����dfa���󣬵õ��ؾ���dfaMatrix��ϡ�����sparseMatrix�Լ��ؾ����base
void UpdateMatrix(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, ulong *stateCluster,
				  ulong *codeMap, std::vector<CDfaRow> &dfaMatrix, 
				  std::vector<STATEID> &base, std::vector<CDfaRow> &sparseMatrix);

//��dfaMatrix�����кϲ�ѹ��
void RowMergeCompress(std::vector<CDfaRow> &dfaMatrix, std::vector<STATEID> &rowGroup, 
					   std::vector<CDfaRow> &MatrixAfterRowMerge);
//��MatrixAfterRowMerge������ѹ��
void ColMergeCompress(std::vector<CDfaRow> &MatrixAfterRowMerge, byte* colGroup, ulong &colNum, std::vector<CDfaRow> &FinalMatrix);
void OutPutCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, ulong *stateCluster, ulong *codeMap);//�������

void OutputMatrix(std::vector<STATEID> &base, std::vector<CDfaRow> &dfaMatrix, std::vector<CDfaRow> &sparseMatrix);//�����ȡbase��ľ����Լ�ϡ�����

void OutPutClusterFinalMatrix(std::vector<CDfaRow> &dfaMatrix, std::vector<STATEID> &rowGroup, 
					   std::vector<CDfaRow> &FinalMatrix);
bool operator == (const std::vector<STATEID> &key1, const std::vector<STATEID> &key2);
ulong CalcFNV(const std::vector<STATEID> &ary);//����unordered_map��key������Ӧ��hashֵ
struct COLUMNKEYHASH
{
	inline ulong operator ()(const std::vector<STATEID> &column)
	{
		return CalcFNV(column);
	}
};
