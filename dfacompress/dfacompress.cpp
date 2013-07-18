#include "stdafx.h"

#include <hwprj\dfacompress.h>

#define STATEMAX 500

DFACOMPRESS void DfaCompress(CDfa &olddfa, ulong &sumBytes)
{
	ulong dfaSize = olddfa.Size();

	std::vector<CUnsignedArray> clusterVec;//表示簇，每个元素每个簇中的状态编号
	ulong stateCluster[STATEMAX];//标记每个状态属于哪个簇,假设最多500个状态
	std::fill(stateCluster, stateCluster + STATEMAX, STATEMAX);

	GetDfaCluster(olddfa, clusterVec, stateCluster);

	std::vector<CDfaRow> dfaMatrix, sparseMatrix, FinalMatrix;//更新后的dfa矩阵和稀疏矩阵
	std::vector<STATEID> base(dfaSize, 0);
	std::vector<STATEID> rowGroup(dfaSize, 0);
	std::vector<byte> colGroup;
	ulong colNum;

	UpdateMatrix(olddfa, clusterVec, stateCluster, dfaMatrix, base, sparseMatrix);

	//对dfaMatrix进行行合并压缩
	RowMergeCompress(dfaMatrix, FinalMatrix, rowGroup, colGroup, colNum);

	//对稀疏矩阵进行压缩，使用二维表存储
	//SparseMatrixCompress(sparseMatrix); 

	//计算该压缩后的DFA占用的字节数
	sumBytes += 2;//2字节表示稀疏表中有效元素个数
	for(int i = 0; i < sparseMatrix.size(); ++i)
	{
		for(int j = 0; j < sparseMatrix[i].Size(); ++j)
		{
			if(sparseMatrix[i][j] != STATEMAX)
			{
				sumBytes += 2;//每个有效元素占用2字节空间
			}
		}
	}

	sumBytes += 256 * 2;//行和列的映射表
	sumBytes += 1;//该字节表示矩阵中状态个数
	sumBytes += 1;//该字节表示列数
	sumBytes += FinalMatrix.size() * colNum;//矩阵中的元素个数，每个元素占用1字节
	sumBytes += 1;//该字节表示startId
	sumBytes += 1 + olddfa.GetFinalStates().CountDfaIds() * 2;//第一字节表示终态和dfaId对应关系的对数，后面的字节是具体的终态和dfaId对应关系
}

//获得dfa的簇信息
void GetDfaCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, ulong *stateCluster)
{
}

//根据簇，更新dfa矩阵，得到簇矩阵dfaMatrix，稀疏矩阵sparseMatrix以及簇矩阵的base
void UpdateMatrix(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec,
				  ulong *stateCluster, std::vector<CDfaRow> &dfaMatrix, 
				  std::vector<STATEID> &base, std::vector<CDfaRow> &sparseMatrix)
{
	ulong dfaSize = olddfa.Size();
	ulong col = olddfa.GetGroupCount();

	for(ulong i = 0; i < olddfa.Size(); ++i)
	{
		CDfaRow &row = olddfa[i];

		//找出该行中，最多的簇
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

		//将输入maxCluster簇的元素存入dfaMatrix矩阵，其余的元素存入sparseMatrix矩阵
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
				sparseMatrixRow[j] = STATEMAX;//因为STATEID(-1)在sparseMatrixRow中可能是有效元素
			}
			else
			{
				sparseMatrixRow[j] = row[j];
				dfaMatrixRow[j] = STATEMAX;
			}
		}
	}
}

//对dfaMatrix进行行合并压缩
void RowMergeCompress(std::vector<CDfaRow> &dfaMatrix, std::vector<CDfaRow> &FinalMatrix,
					  std::vector<STATEID> &rowGroup, std::vector<byte> &colGroup, ulong &colNum)
{

}
