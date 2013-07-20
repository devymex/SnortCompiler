#include "stdafx.h"

#include <hwprj\dfacompress.h>

#define STATEMAX 500


DFACOMPRESS void DfaCompress(CDfa &olddfa, ulong &sumBytes)
{
	ulong dfaSize = olddfa.Size();

	std::vector<CUnsignedArray> clusterVec;//表示簇，每个元素每个簇中的状态编号
	ulong stateCluster[STATEMAX];//标记每个状态属于哪个簇,假设最多500个状态
	std::fill(stateCluster, stateCluster + STATEMAX, STATEMAX);
	ulong codeMap[STATEMAX] = {0};//对olddfa的状态进行重新编号
	GetDfaCluster(olddfa, clusterVec, stateCluster, codeMap);

	OutPutCluster(olddfa, clusterVec, stateCluster, codeMap);//输出簇

	std::vector<CDfaRow> dfaMatrix, sparseMatrix, FinalMatrix;//更新后的dfa矩阵和稀疏矩阵,无效元素用STATEMAX表示
	std::vector<STATEID> base(dfaSize, 0);//里面如果有STATEID(-1),在存文件的时候会存成byte(-1),读入内存后展开成STATEID(-1)
	std::vector<STATEID> rowGroup(dfaSize, 0);
	std::vector<byte> colGroup;
	ulong colNum = 0;
	ulong rowNum = 0;

	UpdateMatrix(olddfa, clusterVec, stateCluster, codeMap, dfaMatrix, base, sparseMatrix);

	OutputMatrix(base, dfaMatrix, sparseMatrix);//输出提取base后的两个矩阵

	//对dfaMatrix进行行合并压缩
	RowMergeCompress(dfaMatrix, rowGroup, FinalMatrix);
	rowNum = FinalMatrix.size();
	OutPutClusterFinalMatrix(dfaMatrix, rowGroup, FinalMatrix);  //输出行压缩后的DFA 映射关系
	 //对dfaMatrix进行列压缩


	//对稀疏矩阵进行压缩，使用二维表存储
	//SparseMatrixCompress(sparseMatrix); 

	//计算该压缩后的DFA占用的字节数
	sumBytes += 2;//2字节表示稀疏表中有效元素个数
	for(ulong i = 0; i < sparseMatrix.size(); ++i)
	{
		for(ulong j = 0; j < sparseMatrix[i].Size(); ++j)
		{
			if(sparseMatrix[i][j] != STATEMAX)
			{
				sumBytes += 2;//每个有效元素占用2字节空间
			}
		}
	}

	//sumBytes += 256 * 2;
	//行和列的映射表,将映射最多的存为default值
	sumBytes += 2;//一个字节存行的映射个数，一个字节存default行映射值
	sumBytes += 2;//一个字节存列映射个数，一个字节存default列映射值
	std::vector<ulong> rowVec(rowNum, 0);
	for(ulong i = 0; i < rowGroup.size(); ++i)
	{
		++rowVec[rowGroup[i]];
	}

	ulong maxRowNum = 0;
	for(ulong i = 0; i < rowVec.size(); ++i)
	{
		if(maxRowNum < rowVec[i])
		{
			maxRowNum = rowVec[i];
		}
	}
	sumBytes += maxRowNum * 2;//存放其他行的映射关系

	std::vector<ulong> colVec(colNum, 0);

	for(ulong i = 0; i < colGroup.size(); ++i)
	{
		++colVec[i];
	}

	ulong maxColNum = 0;
	for(ulong i = 0; i < colNum; ++i)
	{
		if(maxColNum < colVec[i])
		{
			maxColNum = colVec[i];
		}
	}
	sumBytes += maxColNum * 2;//存放其他列的映射关系

	sumBytes += 1;//该字节表示矩阵中状态个数
	sumBytes += 1;//该字节表示列数
	sumBytes += FinalMatrix.size() * colNum;//矩阵中的元素个数，每个元素占用1字节
	sumBytes += 1;//该字节表示状态个数
	sumBytes += 1 + olddfa.GetFinalStates().CountDfaIds() * 2;//第一字节表示终态和dfaId对应关系的对数，后面的字节是具体的终态和dfaId对应关系
}

//获得dfa的簇信息
void GetDfaCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, ulong *stateCluster, ulong *codeMap)
{
	clusterVec.reserve(STATEMAX);

	ulong text = 0;
	ulong label[STATEMAX] = {0};

	std::stack<CUnsignedArray> stateStack;
	STATEID sstate = olddfa.GetStartState();
	label[sstate] =1;
	codeMap[sstate] = 0;
	CUnsignedArray cua;
	cua.PushBack(sstate);
	clusterVec.push_back(cua);
	stateStack.push(cua);
	cua.Clear();
	stateCluster[sstate] = 0;
	bool temp = false;

	CUnsignedArray stateVec;
	while(!stateStack.empty())
	{
		stateVec=stateStack.top();   
		stateStack.pop();
		for(ulong k = 0; k < stateVec.Size(); k++)
		{
		
			for(ulong i = 0; i < olddfa[stateVec[k]].Size(); i++)
			{
				STATEID toState = olddfa[stateVec[k]][i];
				if(toState != (STATEID)-1 && label[toState] != 1)
				{
					cua.PushBack(toState);
					codeMap[toState] = ++text;
					label[toState] = 1;
					stateCluster[toState] = clusterVec.size();
					temp = true;
				}
				if((STATEID)-1 == toState)
				{
					label[STATEMAX - 1] = 1;
				}
			}
			if(temp)
			{
				stateStack.push(cua);
				clusterVec.push_back(cua);
				cua.Clear();
				temp = false;
			}
		}
	}
	if(1 == label[STATEMAX - 1])
	{
		cua.PushBack((STATEID)-1);
		clusterVec.push_back(cua);
		stateCluster[olddfa.Size()] = clusterVec.size() - 1;
	}
}


//根据簇，更新dfa矩阵，得到簇矩阵dfaMatrix，稀疏矩阵sparseMatrix以及簇矩阵的base
void UpdateMatrix(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec,  ulong *stateCluster, 
				  ulong *codeMap, std::vector<CDfaRow> &dfaMatrix, 
				  std::vector<STATEID> &base, std::vector<CDfaRow> &sparseMatrix)
{
	ulong dfaSize = olddfa.Size();
	ulong col = olddfa.GetGroupCount();
	dfaMatrix.resize(dfaSize, CDfaRow(col));
	base.resize(dfaSize);
	sparseMatrix.resize(dfaSize, CDfaRow(col));

	for(ulong i = 0; i < olddfa.Size(); ++i)
	{
		CDfaRow &row = olddfa[i];

		//找出该行中，最多的簇
		ulong cFlag[STATEMAX];
		memset(cFlag, 0, sizeof(ulong) * STATEMAX);
		for(ulong j = 0; j < row.Size(); ++j)
		{
			ulong clusterTmp = 0;
			if(STATEID(-1) == row[j])
			{
				clusterTmp = stateCluster[dfaSize];
			}
			else
			{
				clusterTmp = stateCluster[row[j]];
			}
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

		//将输入maxCluster簇的元素存入dfaMatrix矩阵，其余的元素存入sparseMatrix矩阵,重新标号后
		STATEID state = codeMap[i];
		CDfaRow &dfaMatrixRow = dfaMatrix[state];
		CDfaRow &sparseMatrixRow = sparseMatrix[state];
		 
		base[state] = 0;
		STATEID shift = 0;
		//最大簇不是STATEID(-1)对应的簇
		CUnsignedArray &tmp = clusterVec[maxCluster];
		if(tmp[0] != STATEID(-1))
		{
			CUnsignedArray maxClusterArray;
			for(ulong k = 0; k < tmp.Size(); ++k)
			{
				maxClusterArray.PushBack(codeMap[tmp[k]]);
			}
			maxClusterArray.Sort();
			STATEID oribaseid = maxClusterArray[0];
			base[state] = oribaseid;
			shift = base[state];
		}
		else
		{
			//最大簇是dfaSize，即STATEID(-1)对应的簇
			base[state] = STATEID(-1);//这里如果是STATEID(-1)，在存文件的时候会存成byte(-1)
			shift = STATEID(-1);
		}

		for(ulong j = 0; j < row.Size(); ++j)
		{
			STATEID id = row[j];
			if(STATEID(-1) == row[j])
			{
				id = dfaSize;
			}
			if(stateCluster[id] == maxCluster)
			{
				if(id == dfaSize)
				{
					dfaMatrixRow[j] = 0;
				}
				else
				{
					dfaMatrixRow[j] = codeMap[id] - shift;
				}
				sparseMatrixRow[j] = STATEMAX;//因为STATEID(-1)在sparseMatrixRow中可能是有效元素
			}
			else
			{
				if(id == dfaSize)
				{
					sparseMatrixRow[j] = row[j];
				}
				else
				{
					sparseMatrixRow[j] = codeMap[id];
				}
				dfaMatrixRow[j] = STATEMAX;
			}
		}
	}
}

//对dfaMatrix进行行合并压缩
void RowMergeCompress(std::vector<CDfaRow> &dfaMatrix, std::vector<STATEID> &rowGroup, 
					   std::vector<CDfaRow> &FinalMatrix)
{
	bool label = false;
	FinalMatrix.push_back( dfaMatrix[0]);
	rowGroup[0] = 0;
	for(ulong i = 1; i < dfaMatrix.size(); i++)
	{
		CDfaRow &dfaRow = dfaMatrix[i];
		label = false;
		for(ulong j = 0; j < FinalMatrix.size(); j++)
		{
				CDfaRow &finalRow = FinalMatrix[j];
				ulong k = 0;
				for(; k <dfaRow.Size(); k++)
				{
					if(dfaRow[k] == finalRow[k] || STATEMAX == dfaRow[k] || STATEMAX == finalRow[k])
					{
						continue;
					}
					else
					{
						break;
					}
				}
				if(dfaRow.Size() == k)
				{
					label = true;
					for(ulong g = 0; g < dfaRow.Size(); g++)
					{
						if(STATEMAX ==finalRow[g])
							finalRow[g] = dfaRow[g];
					}
					rowGroup[i] = j;
					break;
				}
		}
		if(!label)
		{
			rowGroup[i] = FinalMatrix.size();
			FinalMatrix.push_back( dfaRow);				
		}
	}	
}

void OutPutClusterFinalMatrix(std::vector<CDfaRow> &dfaMatrix, std::vector<STATEID> &rowGroup, 
					   std::vector<CDfaRow> &FinalMatrix)
{
	std::ofstream fout("F:\\finalDfa.txt");
	if(!fout)
	{
		std::cout << "Open file failure" << std::endl;
		return;
	}
	fout << "映射关系" << "\n";
	for(ulong i = 0; i < dfaMatrix.size(); i++)
	{
		fout << i << "\t" << rowGroup[i] << "\n";
	}
	fout << "行压缩前的DFA" << "\n";
	for(ulong i = 0; i < dfaMatrix.size(); ++i)
	{
		fout << i << "\t";
		for(ulong j = 0; j < dfaMatrix[i].Size(); ++j)
		{
			fout << dfaMatrix[i][j] << "\t";
		}
		fout << "\n";
	}
	fout << "行压缩后的DFA" << "\n";
	for(ulong i = 0; i < FinalMatrix.size(); ++i)
	{
		fout << i << "\t";
		for(ulong j = 0; j < FinalMatrix[i].Size(); ++j)
		{
			fout << FinalMatrix[i][j] << "\t";
		}
		fout << "\n";
	}
	fout.close();
}
void OutPutCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, ulong *stateCluster, ulong *codeMap)
{
	ulong dfaSize = olddfa.Size();
	std::ofstream fout("F:\\cluster.txt");
	if(!fout)
	{
		std::cout << "Open file failure!" << std::endl;
		return;
	}

	fout << "重新编号后的DFA" << std::endl;
	for(ulong i = 0; i < dfaSize; ++i)
	{
		STATEID j = codeMap[i];
		fout << j << "\t";
		//std::cout << j << " ";
		CDfaRow &rowTmp = olddfa[i];
		for(ulong k = 0; k < rowTmp.Size(); ++k)
		{
			STATEID tmp = rowTmp[k];
			if(STATEID(-1) == tmp)
			{
				fout << tmp << "\t";
				//std::cout << tmp << " ";
			}
			else
			{
				fout << codeMap[tmp] << "\t";
				//std::cout << codeMap[tmp] << " ";
			}
		}
		fout << std::endl;
		//std::cout << std::endl;
	}

	fout << "codeMap" << std::endl;
	fout << "原始编号\t重新编号后" << std::endl;
	for(ulong i = 0; i < olddfa.Size(); ++i)
	{
		fout << i << "\t" << codeMap[i] << std::endl;
	}
	
	fout << std::endl << "stateCluster" << std::endl;
	fout << "原始编号\t簇号" << std::endl;
	for(ulong i = 0; i <= olddfa.Size(); ++i)
	{
		fout << i << "\t" << stateCluster[i] << std::endl;
	}

	fout << std::endl << "clusterVec" << std::endl;
	fout << "簇号\t原始编号" << std::endl;
	for(ulong i = 0; i < clusterVec.size(); ++i)
	{
		fout << i << "\t";
		for(ulong j = 0; j < clusterVec[i].Size(); ++j)
		{
			fout << clusterVec[i][j] << ",";
		}
		fout << std::endl;
	}
	fout.close();
}

void OutputMatrix(std::vector<STATEID> &base, std::vector<CDfaRow> &dfaMatrix, std::vector<CDfaRow> &sparseMatrix)
{
	std::ofstream fout1("F:\\matrix.txt");
	if(!fout1)
	{
		std::cout << "Open file failure!" << std::endl;
		return;
	}
	fout1 << "base, 重新编号后" << std::endl;
	fout1 << "重新编号后\t偏移量" << std::endl;
	for(ulong i = 0; i < base.size(); ++i)
	{
		fout1 << i << "\t" << base[i] << std::endl;
	}
	fout1 << "dfaMatrix, 重新编号后" << std::endl;
	for(ulong i = 0; i < dfaMatrix.size(); ++i)
	{
		fout1 << i << "\t";
		for(ulong j = 0; j < dfaMatrix[i].Size(); ++j)
		{
			fout1 << dfaMatrix[i][j] << "\t";
		}
		fout1 << std::endl;
	}

	fout1 << std::endl << "sparseMatrix，重新编号后" << std::endl;
	for(ulong i = 0; i < sparseMatrix.size(); ++i)
	{
		fout1 << i << "\t";
		for(ulong j = 0; j < sparseMatrix[i].Size(); ++j)
		{
			fout1 << sparseMatrix[i][j] << "\t";
		}
		fout1 << std::endl;
	}
	fout1.close();
}