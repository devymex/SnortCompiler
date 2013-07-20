#include "Hierarchical.h"
#include <map>
#include <algorithm>

void BuildGraph(const CDfa &oneDfa, const ROWSET &rows, GRAPH &graph)
{
	size_t nRow = rows.size();
	graph.resize(nRow * nRow);
	std::fill(graph.begin(), graph.end(), 0.0);

	for (size_t i = 0; i < nRow; ++i)
	{
		for (size_t j = i + 1; j < nRow; ++j)
		{
			size_t nEqualCnt = 0;
			for (size_t k = 0; k < oneDfa.GetGroupCount(); ++k)
			{
				if ((oneDfa[rows[i]][k] == oneDfa[rows[j]][k]))
				{
					++nEqualCnt;
				}
			}
			double radio = (double)nEqualCnt / (double)oneDfa.GetGroupCount();
			if (radio > 0.5)
			{
				graph[i * nRow + j] = graph[j * nRow + i] = radio;			 
			}
		}
	}
}

int DFS_Visit(const GRAPH &graph, size_t vex, ROWSET &flag, ROWSET &curRow)
{
	if (flag[vex] == 0)
	{
		flag[vex] = 1;
		curRow.push_back(vex);
	}
	else
	{
		return 0;
	}

	size_t nVexCnt = sqrt(graph.size());
	for (size_t i = 0; i < nVexCnt; ++i)
	{
		if (graph[vex * nVexCnt + i] > 0 &&
			flag[i] == 0)
		{
			DFS_Visit(graph, i, flag, curRow);
		}
	}

	return 1;
}

void SearchConnectSubgraph(const GRAPH &graph, VECROWSET &vecRows)
{
	size_t nVexCnt = sqrt(graph.size());

	ROWSET visit;
	visit.resize(nVexCnt);
	std::fill(visit.begin(), visit.end(), 0);

	for (size_t i = 0; i < nVexCnt; ++i)
	{
		if (visit[i] == 0)
		{
			vecRows.push_back(ROWSET());
			DFS_Visit(graph, i, visit, vecRows.back());
		}
	}
}
//MA����,removeSetΪ���Ƴ���״̬����matrixΪ��ȫ����oriSetΪ���������״̬��
void MinArray(ROWSET removeSet,GRAPH matrix, ROWSET &oriSet)
{
	int n = sqrt(matrix.size());												//n��ʾ�����ά��
	ROWSET oriSet, termiSet;													//oriSetΪ��̬����termiSetΪ��̬��
	for(int i = 0; i != n; ++i)													//��termiSet��ʼ������״̬��������
	{
		int sign = 0;															//signΪ���λ
		if(removeSet.size() != 0)												//�ж�removeSet�Ƿ�Ϊ��
		{
			sign = SignState(i, removeSet);										//�����ڣ����Ǹ�״̬�Ƿ��Ѿ����Ƴ�
		}
		if(sign == 0)															//���δ��removeSet�У���δ�Ƴ����������̬��
		{
			termiSet.push_back(i);
		}
	}
	byte startState = termiSet.front();											//startStateΪ��ʼ״̬
	oriSet.push_back(startState);												//����ʼ״̬����oriSet
	termiSet.erase(termiSet.begin());
	ROWSET::iterator signState;
	while (oriSet.size() == n)													//ѭ����ֱ�����еĵ㶼�����̬����
	{										
		double weight = 0;														//result��ʾ����Ȩֵ
		ROWSET::iterator i = termiSet.begin();									//signState�����������Ȩֵ��
		for (; i != termiSet.end(); ++i)										//ѭ��������̬��
		{
			double tmp = 0;														//tmpΪ��ʱ��������ʾ��ǰȨֵ
			for (ROWSET::iterator j = oriSet.begin(); j != oriSet.end(); ++j)	//ѭ��������̬��
			{
				if(matrix[(*i - 1) * n + *j] != 0)								//������Ϊ0���ұߴ��ڣ�����Ӳ�������tmp
				{
					tmp += matrix[(*i - 1)* n + *j];
				}
			}
			if(tmp > weight)													//���tmp����result�������
			{
				weight = tmp;
				signState = i;
			}
		}
		oriSet.push_back(*signState);											//����״̬�����̬���У���Ϊ���յ�����
		termiSet.erase(signState);
	}
}

//�������С���㷨
void StoreWagner(GRAPH matrix, VECROWSET &result)
{
	ROWSET arySet, removeSet, first, second;			//arySet��ʾ�ź����״̬����removeSet��ʾ�Ѿ��Ƴ���״̬��
	VECROWSET sets;									//vecSets������ʾ״̬�ľ��༯��result�����洢���ս��
	double minCutTmp, minCutResult = sqrt(matrix.size());	//minCutTmp������ʾ��ǰ����С��Ȩֵ��minCutResult��ʾ��ǰΪֹ����С�����Сֵ
	int n = sqrt(matrix.size());						//nΪ״̬����
	while(removeSet.size() != n)							//ѭ����ͼ���зָ�
	{
		MinArray(removeSet, matrix, arySet);			//MA�㷨����״̬��������
		byte t = arySet.back();							//t��ʾ���һ��״̬����Ҫɾ����״̬
		byte s = *(arySet.end()-2);						//s��ʾ�����ڶ���״̬����Ҫ�����״̬
		double minCutTmp = MergeState(s, t, removeSet, matrix, sets);			//��С���㷨�������С��Ȩֵ������ͼ���и���
		//�����������t״̬������vecSets.front()����ʾ��t״̬Ϊһ��״̬��������������Ϊ��״̬�ļ���
		if(minCutTmp < minCutResult)					//�����ǰ����С��ȨֵΪ��Сֵ
		{
			for(VECROWSET::size_type i = 0; i != sets.size(); ++i)					//ѭ������vecSets״̬��
			{
				if(t == sets[i].front())				//���t������vecSets.front()
				{
					first.assign(sets[i].begin(), sets[i].end());				//�򽫽��������result[1]�У�����һ���м�
				}
				else
				{
					first.push_back(t);				//���û�У���ֱ�ӽ�t������result[1]�У�����һ���м�
				}
			}
			for(int i = 0; i != n; ++i)					//ѭ������result[2]�����ڶ����м�
			{
				int sign = SignState(i, first);			//sign��Ǹ�״̬�Ƿ�����ڵ�һ���м���
				if(sign == 0)							//���û�г��֣��򱣴��ڵڶ��м���
				{
					second.push_back(i);
				}
			}
			result.push_back(first);
			result.push_back(second);
		}
	}
}

//���س��ִ��������±�
size_t maxn(size_t* bary,int size)
{
	size_t n_max = 0; 
	for (int i = 1; i < size; i++)
	{
		if (bary[n_max] < bary[i])
		{
			n_max = i;
		}
	}
	return n_max;

}

//ͳ�������,����洢�ռ�,ÿ��һ���м�
size_t StatisticVitualCore(const CDfa &oneDfa, ROWSET &rs)
{
	size_t n_size = rs.size();   //�м���С
	size_t n_statenum = oneDfa.Size();  //dfa״̬��
	size_t* bary = new size_t[n_statenum]; //ͳ�ƴ���
	size_t* bcountary = new size_t[n_statenum]; //�洢��ת״̬��ͬ�ĸ���
	//for (size_t bcount = 0; bcount < n_statenum; bcount++) //init
	//{
	//	bcountary[bcount] = 0;
	//}
	std::fill(bcountary, bcountary + n_statenum, 0);

	//size_t n_dfacol = oneDfa[0].Size();//colnum
	size_t n_dfacol = oneDfa.GetGroupCount();//colnum

	ROWSET visrow;
	for (size_t col = 0; col < n_dfacol; col++) //dfa��
	{
		//for (size_t ba = 0; ba < n_statenum; ba++) //init
		//{
		//	bary[ba] = 0;
		//}
		std::fill(bary, bary + n_statenum, 0);

		for (size_t i = 0; i< n_size; i++) //ͳ�Ƴ��ִ���
		{
			STATEID bt = oneDfa[(size_t)(rs[i])][col];
			bary[size_t(bt)]++;
		}
		STATEID maxindex = maxn(bary, n_statenum); //�������±�
		visrow.push_back((BYTE)(maxindex)); //���������
		for (size_t i = 0; i< n_size; i++)   //�洢��ת״̬��ͬ�ĸ���
		{
			BYTE bt = oneDfa[(size_t)(rs[i])][col];
			if (visrow[col] != bt)
			{
				bcountary[i]++;
			}
		}
	}
	//�����ڴ��С
	size_t vsmem = n_size;
	for(size_t i = 0; i < n_size; i++)
	{
		vsmem += 2*bcountary[i];
	}

	delete[] bary;
	delete[] bcountary;

	return vsmem;

}


void HierarchicalCluster(const CDfa &oneDfa, VECROWSET &vecRows)
{
	for (size_t i = 0; i < vecRows.size(); ++i)
	{
		size_t curRowval = StatisticVitualCore(oneDfa, vecRows[i]);

		GRAPH curGraph;
		BuildGraph(oneDfa, vecRows[i], curGraph);

		VECROWSET vecTmp;
		StoreWagner();

		size_t partRowval = StatisticVitualCore(oneDfa, vecTmp[0])
			+ StatisticVitualCore(oneDfa, vecTmp[1]);

		//�Ƚ����δ洢�ռ��С�Ƿ����
		//������٣������ָ��ķָ�ͼ
		//�������ָ�ǰ�ķָ�ͼ
		if (curRowval > partRowval)
		{
			vecRows.push_back(vecTmp[1]);
			--i;
		}
	}

}
//״̬�ϲ���removeSet��ʾ�Ƴ���״̬������Ҫ���£�s��t״̬Ϊ�������״̬��brokenMartix��¼��Ȩ����ĸ��£�vecSets��¼״̬���ĸ���
int MergeState(byte s, byte t, ROWSET &removeSet, GRAPH &matrix, VECROWSET &sets)
{
	int n = sqrt(matrix.size());						//n��ʾ״̬����
	int tCount = (int)t, sCount = (int)s;					//tCount��ʾt�����кţ�sCount��ʾs�����к�
	double minCutTmp = 0;									//minCutTmp������ʾ��ǰ����С��Ȩֵ
	//���´�Ȩ����ֻ��Ҫ����s״̬���к���
	for(int i = 0; i != n; ++i)
	{
		int sign = SignState(i, removeSet);					//sign������Ǹ�״̬�Ƿ���removeSet����
		int tNumber = (tCount - 1) * n + i;					//tNumber��ʾt��i�е�Ԫ��
		if(sign == 0 && matrix[tNumber] != 0)			//�����״̬�ڵ�ǰ״̬���У�����t��i��Ԫ�ز�Ϊ0�������s��i��Ԫ��
		{
			minCutTmp += matrix[tNumber];				//�ۼ�����С��Ȩֵ
			matrix[(sCount - 1) * n + i] += matrix[tNumber];							//����s��
			matrix[(i - 1) * n + sCount] = matrix[(sCount - 1) * n + i];					//����s��
		}
	}
	removeSet.push_back(t);									//����removeSet����t״̬��������
	//����vecSets״̬����ֻ��Ҫ����s��t״̬
	ROWSET tmpVec;											//tmpVec��ʾ��ʱ����
	if(sets.size() == 0)									//���vecSetsΪ�գ����ʼ��
	{
		tmpVec.resize(2);
		sets.push_back(tmpVec);
	}
	tmpVec.push_back(s);									//��s����tmpVec
	tmpVec.push_back(t);									//��t����tmpVec
	VECROWSET::size_type i = 0;								//iΪ������
	for(; i != sets.size(); ++i)							//����vecSets
	{
		if(s == sets[i].front())							//���s��vecSets�У���ʾs֮ǰ���������״̬
		{
			sets[i].push_back(t);							//��t����s���ڵ�������
			break;
		}
	}
	if(i == sets.size())									//���s֮ǰû�д��������״̬
	{
		sets.push_back(tmpVec);								//��s��t����vecSets
	}
	return minCutTmp;
}
//��Ǻ�����������Ǹ�״̬�Ƿ���״̬���г���
int SignState(byte state, ROWSET stateSet)
{
	int sign = 0;											//sign������Ǹ�״̬�Ƿ���״̬����
	for(ROWSET::size_type i= 0; i != stateSet.size(); ++i)					//ѭ������״̬��
	{
		if(state == stateSet[i])							//����õ���״̬����
		{
			sign = 1;										//���λ��Ϊ1
		}
	}
	return sign;											//���ر�ǣ�1������֣�0����δ����
}