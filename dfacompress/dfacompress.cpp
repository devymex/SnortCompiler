#include "stdafx.h"

#include <hwprj\dfacompress.h>

#define STATEMAX 500


DFACOMPRESS void DfaCompress(CDfa &olddfa, ulong &sumBytes)
{
	ulong dfaSize = olddfa.Size();

	std::vector<CUnsignedArray> clusterVec;//��ʾ�أ�ÿ��Ԫ��ÿ�����е�״̬���
	ulong stateCluster[STATEMAX];//���ÿ��״̬�����ĸ���,�������500��״̬
	std::fill(stateCluster, stateCluster + STATEMAX, STATEMAX);
	ulong codeMap[STATEMAX] = {0};//��olddfa��״̬�������±��
	GetDfaCluster(olddfa, clusterVec, stateCluster, codeMap);

	OutPutCluster(olddfa, clusterVec, stateCluster, codeMap);//�����

	std::vector<CDfaRow> dfaMatrix, sparseMatrix, FinalMatrix;//���º��dfa�����ϡ�����,��ЧԪ����STATEMAX��ʾ
	std::vector<STATEID> base(dfaSize, 0);//���������STATEID(-1),�ڴ��ļ���ʱ�����byte(-1),�����ڴ��չ����STATEID(-1)
	std::vector<STATEID> rowGroup(dfaSize, 0);
	std::vector<byte> colGroup;
	ulong colNum = 0;
	ulong rowNum = 0;

	UpdateMatrix(olddfa, clusterVec, stateCluster, codeMap, dfaMatrix, base, sparseMatrix);

	OutputMatrix(base, dfaMatrix, sparseMatrix);//�����ȡbase�����������

	//��dfaMatrix�����кϲ�ѹ��
	RowMergeCompress(dfaMatrix, rowGroup, FinalMatrix);
	rowNum = FinalMatrix.size();
	OutPutClusterFinalMatrix(dfaMatrix, rowGroup, FinalMatrix);  //�����ѹ�����DFA ӳ���ϵ
	 //��dfaMatrix������ѹ��


	//��ϡ��������ѹ����ʹ�ö�ά��洢
	//SparseMatrixCompress(sparseMatrix); 

	//�����ѹ�����DFAռ�õ��ֽ���
	sumBytes += 2;//2�ֽڱ�ʾϡ�������ЧԪ�ظ���
	for(ulong i = 0; i < sparseMatrix.size(); ++i)
	{
		for(ulong j = 0; j < sparseMatrix[i].Size(); ++j)
		{
			if(sparseMatrix[i][j] != STATEMAX)
			{
				sumBytes += 2;//ÿ����ЧԪ��ռ��2�ֽڿռ�
			}
		}
	}

	//sumBytes += 256 * 2;
	//�к��е�ӳ���,��ӳ�����Ĵ�Ϊdefaultֵ
	sumBytes += 2;//һ���ֽڴ��е�ӳ�������һ���ֽڴ�default��ӳ��ֵ
	sumBytes += 2;//һ���ֽڴ���ӳ�������һ���ֽڴ�default��ӳ��ֵ
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
	sumBytes += maxRowNum * 2;//��������е�ӳ���ϵ

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
	sumBytes += maxColNum * 2;//��������е�ӳ���ϵ

	sumBytes += 1;//���ֽڱ�ʾ������״̬����
	sumBytes += 1;//���ֽڱ�ʾ����
	sumBytes += FinalMatrix.size() * colNum;//�����е�Ԫ�ظ�����ÿ��Ԫ��ռ��1�ֽ�
	sumBytes += 1;//���ֽڱ�ʾ״̬����
	sumBytes += 1 + olddfa.GetFinalStates().CountDfaIds() * 2;//��һ�ֽڱ�ʾ��̬��dfaId��Ӧ��ϵ�Ķ�����������ֽ��Ǿ������̬��dfaId��Ӧ��ϵ
}

//���dfa�Ĵ���Ϣ
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


//���ݴأ�����dfa���󣬵õ��ؾ���dfaMatrix��ϡ�����sparseMatrix�Լ��ؾ����base
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

		//�ҳ������У����Ĵ�
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

		//������maxCluster�ص�Ԫ�ش���dfaMatrix���������Ԫ�ش���sparseMatrix����,���±�ź�
		STATEID state = codeMap[i];
		CDfaRow &dfaMatrixRow = dfaMatrix[state];
		CDfaRow &sparseMatrixRow = sparseMatrix[state];
		 
		base[state] = 0;
		STATEID shift = 0;
		//���ز���STATEID(-1)��Ӧ�Ĵ�
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
			//������dfaSize����STATEID(-1)��Ӧ�Ĵ�
			base[state] = STATEID(-1);//���������STATEID(-1)���ڴ��ļ���ʱ�����byte(-1)
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
				sparseMatrixRow[j] = STATEMAX;//��ΪSTATEID(-1)��sparseMatrixRow�п�������ЧԪ��
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

//��dfaMatrix�����кϲ�ѹ��
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
	fout << "ӳ���ϵ" << "\n";
	for(ulong i = 0; i < dfaMatrix.size(); i++)
	{
		fout << i << "\t" << rowGroup[i] << "\n";
	}
	fout << "��ѹ��ǰ��DFA" << "\n";
	for(ulong i = 0; i < dfaMatrix.size(); ++i)
	{
		fout << i << "\t";
		for(ulong j = 0; j < dfaMatrix[i].Size(); ++j)
		{
			fout << dfaMatrix[i][j] << "\t";
		}
		fout << "\n";
	}
	fout << "��ѹ�����DFA" << "\n";
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

	fout << "���±�ź��DFA" << std::endl;
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
	fout << "ԭʼ���\t���±�ź�" << std::endl;
	for(ulong i = 0; i < olddfa.Size(); ++i)
	{
		fout << i << "\t" << codeMap[i] << std::endl;
	}
	
	fout << std::endl << "stateCluster" << std::endl;
	fout << "ԭʼ���\t�غ�" << std::endl;
	for(ulong i = 0; i <= olddfa.Size(); ++i)
	{
		fout << i << "\t" << stateCluster[i] << std::endl;
	}

	fout << std::endl << "clusterVec" << std::endl;
	fout << "�غ�\tԭʼ���" << std::endl;
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
	fout1 << "base, ���±�ź�" << std::endl;
	fout1 << "���±�ź�\tƫ����" << std::endl;
	for(ulong i = 0; i < base.size(); ++i)
	{
		fout1 << i << "\t" << base[i] << std::endl;
	}
	fout1 << "dfaMatrix, ���±�ź�" << std::endl;
	for(ulong i = 0; i < dfaMatrix.size(); ++i)
	{
		fout1 << i << "\t";
		for(ulong j = 0; j < dfaMatrix[i].Size(); ++j)
		{
			fout1 << dfaMatrix[i][j] << "\t";
		}
		fout1 << std::endl;
	}

	fout1 << std::endl << "sparseMatrix�����±�ź�" << std::endl;
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