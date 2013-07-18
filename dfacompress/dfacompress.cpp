#include "stdafx.h"

#include <hwprj\dfacompress.h>

#define STATEMAX 500

DFACOMPRESS void DfaCompress(CDfa &olddfa, ulong &sumBytes)
{
	ulong dfaSize = olddfa.Size();

	std::vector<CUnsignedArray> clusterVec;//��ʾ�أ�ÿ��Ԫ��ÿ�����е�״̬���
	ulong stateCluster[STATEMAX];//���ÿ��״̬�����ĸ���,�������500��״̬
	std::fill(stateCluster, stateCluster + STATEMAX, STATEMAX);

	GetDfaCluster(olddfa, clusterVec, stateCluster);

	std::vector<CDfaRow> dfaMatrix, sparseMatrix, FinalMatrix;//���º��dfa�����ϡ�����
	std::vector<STATEID> base(dfaSize, 0);
	std::vector<STATEID> rowGroup(dfaSize, 0);
	std::vector<byte> colGroup;
	ulong colNum;

	UpdateMatrix(olddfa, clusterVec, stateCluster, dfaMatrix, base, sparseMatrix);

	//��dfaMatrix�����кϲ�ѹ��
	RowMergeCompress(dfaMatrix, FinalMatrix, rowGroup, colGroup, colNum);

	//��ϡ��������ѹ����ʹ�ö�ά��洢
	//SparseMatrixCompress(sparseMatrix); 

	//�����ѹ�����DFAռ�õ��ֽ���
	sumBytes += 2;//2�ֽڱ�ʾϡ�������ЧԪ�ظ���
	for(int i = 0; i < sparseMatrix.size(); ++i)
	{
		for(int j = 0; j < sparseMatrix[i].Size(); ++j)
		{
			if(sparseMatrix[i][j] != STATEMAX)
			{
				sumBytes += 2;//ÿ����ЧԪ��ռ��2�ֽڿռ�
			}
		}
	}

	sumBytes += 256 * 2;//�к��е�ӳ���
	sumBytes += 1;//���ֽڱ�ʾ������״̬����
	sumBytes += 1;//���ֽڱ�ʾ����
	sumBytes += FinalMatrix.size() * colNum;//�����е�Ԫ�ظ�����ÿ��Ԫ��ռ��1�ֽ�
	sumBytes += 1;//���ֽڱ�ʾstartId
	sumBytes += 1 + olddfa.GetFinalStates().CountDfaIds() * 2;//��һ�ֽڱ�ʾ��̬��dfaId��Ӧ��ϵ�Ķ�����������ֽ��Ǿ������̬��dfaId��Ӧ��ϵ
}

//���dfa�Ĵ���Ϣ
void GetDfaCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, ulong *stateCluster)
{
}

//���ݴأ�����dfa���󣬵õ��ؾ���dfaMatrix��ϡ�����sparseMatrix�Լ��ؾ����base
void UpdateMatrix(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec,
				  ulong *stateCluster, std::vector<CDfaRow> &dfaMatrix, 
				  std::vector<STATEID> &base, std::vector<CDfaRow> &sparseMatrix)
{
	ulong dfaSize = olddfa.Size();
	ulong col = olddfa.GetGroupCount();

	for(ulong i = 0; i < olddfa.Size(); ++i)
	{
		CDfaRow &row = olddfa[i];

		//�ҳ������У����Ĵ�
		ulong cFlag[STATEMAX];
		memset(cFlag, 0, sizeof(ulong) * STATEMAX);
		for(ulong j = 0; j < row.Size(); ++j)
		{
			ulong clusterTmp = stateCluster[row[j]];
			++cFlag[clusterTmp];
		}

		ulong max = 0;
		ulong maxCluster = 0;
		for(ulong j = 0; j < STATEMAX; ++j)
		{
			if(max < cFlag[j])
			{
				max = cFlag[j];
				maxCluster = j;
			}
		}

		//������maxCluster�ص�Ԫ�ش���dfaMatrix���������Ԫ�ش���sparseMatrix����
		dfaMatrix.push_back(CDfaRow(col));
		CDfaRow &dfaMatrixRow = dfaMatrix.back();
		sparseMatrix.push_back(CDfaRow(col));
		CDfaRow &sparseMatrixRow = sparseMatrix.back();

		clusterVec[maxCluster].Sort();
		base.push_back(clusterVec[maxCluster][0]);

		for(ulong j = 0; j < row.Size(); ++j)
		{
			if(stateCluster[row[j]] == maxCluster)
			{
				dfaMatrixRow[j] = row[j] - base.back();
				sparseMatrixRow[j] = STATEMAX;//��ΪSTATEID(-1)��sparseMatrixRow�п�������ЧԪ��
			}
			else
			{
				sparseMatrixRow[j] = row[j];
				dfaMatrixRow[j] = STATEMAX;
			}
		}
	}
}

//��dfaMatrix�����кϲ�ѹ��
void RowMergeCompress(std::vector<CDfaRow> &dfaMatrix, std::vector<CDfaRow> &FinalMatrix,
					  std::vector<STATEID> &rowGroup, std::vector<byte> &colGroup, ulong &colNum)
{

}
