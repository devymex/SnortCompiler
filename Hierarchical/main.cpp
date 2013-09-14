#include "Hierarchical.h"


//�����µ�ӳ��
void CreateNewMap(VECROWSET &allCharset, VECROWSET &newCharset, std::vector<std::map<size_t, size_t>> &mapv)
{

	std::map<size_t,size_t>::iterator iter;
	for (ulong i = 0; i < allCharset.size(); ++i)
	{
		std::map<size_t, size_t> m;
		size_t cnt = 0;	
		size_t j = 0;
		for (size_t index = 0; index < 32; index++)
		{
			iter = m.find(allCharset[i][index]);
			if (iter == m.end())
			{
				m.insert(std::map<size_t, size_t>::value_type(allCharset[i][index], cnt));
				newCharset[i][index] = cnt;
				cnt++;
			}else 
			{
				newCharset[i][index] = iter->second;
			}			
		}
		for (size_t index = 128; index < 256; index++)
		{
			iter = m.find(allCharset[i][index]);
			if (iter == m.end())
			{
				m.insert(std::map<size_t, size_t>::value_type(allCharset[i][index], cnt));
				newCharset[i][index] = cnt;
				cnt++;
			}else 
			{
				newCharset[i][index] = iter->second;
			}			
		}
		for (size_t index = 32; index < 127; index++)
		{
			iter = m.find(allCharset[i][index]);
			if (iter == m.end())
			{
				m.insert(std::map<size_t, size_t>::value_type(allCharset[i][index], cnt));
				newCharset[i][index] = cnt;
				cnt++;
			}else 
			{
				newCharset[i][index] = iter->second;
			}			
		}
		/*for (ROWSET::reverse_iterator ritr = allCharset[i].rbegin(); ritr != allCharset[i].rend(); ++ritr, j--)
		{
			iter = m.find(*ritr);
			if (iter == m.end())
			{
				m.insert(std::map<size_t, size_t>::value_type(*ritr, cnt));
				newCharset[i][j] = cnt;
				cnt++;
			}else 
			{
				newCharset[i][j] = iter->second;
			}			
		}		*/	
	
		mapv.push_back(m);
		
	}
}

//�����µ�ӳ�����DFA�����
void AdjustDfa(CDfaArray &DfaArr, std::vector<std::map<size_t, size_t>> &mapv)
{
	CDfaArray tempArr = DfaArr;
	std::map<size_t,size_t>::iterator iter;
	std::map<size_t, size_t> mCnt = mapv[0];
	for (ulong i = 0; i < DfaArr.Size(); ++i)
	{		
		std::map<size_t, size_t> m = mapv[i];
		/*for (ulong j = 0; j < DfaArr[i].GetGroupCount(); ++j)
		{*/
			for (iter = m.begin(); iter != m.end(); iter++)
			{
				size_t preCol = iter->first;
				size_t sufCol = iter->second;
				// ��DfaArr[i]�ĵ�preCOl�и�����tempArr[i]�ĵ�sufCol��
				for (ulong k = 0; k < DfaArr[i].Size(); ++k)
				{
					tempArr[i][k][sufCol] = DfaArr[i][k][preCol];
				}		
			}
		//}
	}
	DfaArr = tempArr;
}


//��Ĭ���ַ�����
size_t Charset(CDfa &dfa)
{
	std::map<byte, size_t> mapCharset;
	for (byte i = 0; i < SC_DFACOLCNT - 1; ++i)
	{
		++mapCharset[dfa.Char2Group(i)];
	}

	size_t max = 0;
	for (std::map<byte, size_t>::iterator i = mapCharset.begin(); i != mapCharset.end(); ++i)
	{
		if (i->second > max)
		{
			max = i->second;
		}
	}

	return (256 - max);
}

struct COLUMNKEYHASH
{
	ulong operator()(const std::vector<STATEID> &ary)
	{
		const ulong _FNV_offset_basis = 2166136261U;
		const ulong _FNV_prime = 16777619U;

		ulong hash = _FNV_offset_basis;
		for (std::vector<STATEID>::const_iterator i = ary.cbegin(); i != ary.cend(); ++i)
		{
			hash ^= *i;
			hash *= _FNV_prime;
		}
		return hash;
	}
};

bool operator == (const std::vector<STATEID> &key1, const std::vector<STATEID> &key2)
{
	if(key1.size() != key2.size())
		return false;
	for (int i = 0; i < key1.size(); i++)
	{
		if(key1[i] != key2[i])
		{
			return false;
		}
	}
	return true;
}

//�˾������ַ���ѹ��
void ColMergeCompress(VECROWSET &vecCores, ulong colCnt, byte* colGroup, ulong &colNum, std::vector<CDfaRow> &FinalMatrix)
{
	typedef std::unordered_map<std::vector<STATEID>, STATEID, COLUMNKEYHASH> STATESETHASH;
	STATESETHASH ssh;

	ulong row = vecCores.size();
	ulong col = colCnt;
	std::vector<STATEID> key;
	STATESETHASH::iterator p;
	for(ulong j = 0; j < col; ++j)
	{
		for(ulong i = 0; i < row; ++i)
		{
			key.push_back(vecCores[i][j]);
		}
		p = ssh.find(key);
		if(p == ssh.end())
		{
			ulong keyID = ssh.size();
			ssh[key] = keyID;
			colGroup[j] = keyID;	
			++colNum;
		}
		else
		{
			colGroup[j] = p->second;
		}
		key.clear();
	}
	CDfaRow CDrow(ssh.size());
	for(ulong i = 0; i < row; i++)
	{
		FinalMatrix.push_back(CDrow);
	}
	for(STATESETHASH::iterator q = ssh.begin(); q != ssh.end(); ++q)
	{
		for(ulong i = 0; i < row; ++i)
		{
			FinalMatrix[i][q->second] = q->first[i];
		}
	}
}

void Hierarchical(CDfa &dfa, ulong &memSize, VECROWSET &coreMatrix)
{
	ROWSET rows;
	for (size_t j = 0; j <dfa.Size(); ++j)
	{
		rows.push_back(j);
	}

	GRAPH graph;
	ROWSET weightArg;
	BuildGraph(dfa, graph, weightArg);

	std::vector<BLOCK> blocks;
	blocks.push_back(BLOCK());
	blocks.back().weightIdx = 0;
	blocks.back().nodes = rows;
	SplitGraph(dfa, graph, weightArg, blocks);

	memSize = StatisticMemory(dfa, blocks, coreMatrix);
}

void SortCharset(VECROWSET &allCharset, size_t threshold)
{
	VECROWSET lastCharset;
	for (NODEARRAY_ITER i = allCharset.begin(); i != allCharset.end(); ++i)
	{
		lastCharset.push_back(ROWSET());
		ROWSET& b = lastCharset.back();
		//lastCharset.back().assign( i->begin(), i->begin() + threshold);
		b.insert(b.begin(), i->begin(), i->begin() + threshold);
		b.insert(b.end(), i->begin() + 128, i->end());
	}

	std::sort(lastCharset.begin(), lastCharset.end());
	lastCharset.erase(std::unique(lastCharset.begin(), lastCharset.end()), lastCharset.end());

	std::cout << lastCharset.size() << std::endl;
}

void main(int nArgs, char **cArgs)
{
	CGroupRes groupRes;
	groupRes.ReadFromFile(cArgs[1]);
	CDfaArray &CDfaSet = groupRes.GetDfaTable();

	//�˾�����ӳ�����ת����˾���Ĵ洢�ռ�֮��
	ulong sumBytes = 0;
	//�ܹ�������ѹ��������
	ulong cnt = 0;
	//�ַ���ӳ����С
	ulong charBytes = 0;
	//��ת���С
	ulong skiptblBytes = 0;
	//���ľ����С
	ulong coreBytes = 0;
	//��̬���ϴ�С
	ulong finalBytes = 0;


	VECROWSET allCharset;
	for (size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		//ulong nExtraMem = 0;
		std::cout << i << std::endl;

		allCharset.push_back(ROWSET());
		for (size_t j = 0; j < SC_DFACOLCNT; ++j)
		{
			allCharset.back().push_back(size_t(CDfaSet[i].Char2Group(j)));
		}

		////չ��DFAΪ256��
		//CDfa unflodDfa;
		//UnflodDFA(CDfaSet[i],unflodDfa);

		//ulong memSize;
		//VECROWSET coreMatrix;
		////��ξ��෽��������һ��DFA������ѹ������ת��ͺ˾���Ĵ洢�ռ��С�Լ��˾�������
		//Hierarchical(CDfaSet[i], memSize, coreMatrix);


		//VECROWSET vecRows;
		//SearchConnectSubgraph(graph, vecRows);
		//VECROWSET vecVirtual;
		//size_t memSize = HierarchicalCluster(CDfaSet[i], vecRows, vecVirtual);

		//skiptblBytes += (memSize - CDfaSet[i].GetGroupCount() * coreMatrix.size());
		//coreBytes += CDfaSet[i].GetGroupCount() * coreMatrix.size();
		//finalBytes += 2 * CDfaSet[i].GetFinalStates().CountDfaIds();
		//charBytes += 2 * Charset(CDfaSet[i]);

		//ulong colNum = 0;
		//byte colGroup[256] = {0};
		//ulong colCnt = unflodDfa.GetGroupCount();
		//std::vector<CDfaRow> FinalMatrix;
		////�˾�����ѹ��
		//ColMergeCompress(coreMatrix, colCnt, colGroup, colNum, FinalMatrix);

		////�˾�����ѹ����洢�Ŀռ��С
		//size_t cost = memSize;
		//size_t cost2 = memSize - unflodDfa.GetGroupCount() * coreMatrix.size() + colNum * coreMatrix.size();
		//if (cost > cost2)
		//{
		//	cost = cost2;
		//	++cnt;
		//}
		//sumBytes += cost;

		////����Ĵ洢�ռ��С������8���ֽڹ̶������Ϣ���ַ�����С����������ӳ���С������̬����С
		//nExtraMem = 8 + 2 * Charset(CDfaSet[i]) + 2 * CDfaSet[i].GetFinalStates().CountDfaIds();

		////����洢�ռ������Ϣ
		//std::ofstream fout("storesize.txt", std::ios::app);
		//fout << i << '\t' << memSize << std::endl;
		//fout.close();

		////����˾��������Ϣ
		//std::ofstream ofile("core.txt", std::ios::app);
		//ofile << i << " :" << std::endl;
		//for (NODEARRAY_ITER j = vecCores.begin(); j != vecCores.end(); ++j)
		//{
		//	for (ROWSET::iterator k = j->begin(); k != j->end(); ++k)
		//	{
		//		ofile << *k << "\t";
		//	}
		//	ofile << std::endl;
		//}
		//ofile << std::endl;
		//ofile.close();
	}

	std::ofstream fout1("op1.txt");
	std::ofstream fout2("op2.txt");
	
	VECROWSET newCharset = allCharset;
	std::vector<std::map<size_t, size_t>> mapv;
	fout1 << "ԭ�ַ�ӳ���:" << std::endl; 
	for (ulong i = 0; i < 1; i++)
	{
		for (ulong j = 0; j < allCharset[i].size(); j++)
		{
			fout1 << allCharset[i][j] << "\t";
		}
		fout1 << std::endl;
	}

	fout1 << "ԭ����DFA:" << std::endl;
	for (ulong i = 0; i < 1; ++i)
	{
		for(ulong j = 0; j < CDfaSet[i].Size(); ++j)
		{
			for(ulong k = 0; k < CDfaSet[i][j].Size(); ++k)
				fout1 << CDfaSet[i][j][k] << "\t" ;
			fout1 << std::endl;
		}

	}

	CreateNewMap(allCharset, newCharset, mapv);	
	SortCharset(newCharset, 32);
	AdjustDfa(CDfaSet, mapv);

	fout2 << "���ַ�ӳ���:" << std::endl;
	for (ulong i = 0; i < 1; i++)
	{
		for (ulong j = 0; j < newCharset[i].size(); j++)
		{
			fout2 << newCharset[i][j] << "\t" ;
		}
		fout2 << std::endl;
	}
	fout2 << "DFA���޸�֮��" << std::endl;
	for (ulong i = 0; i < 1; ++i)
	{
		for(ulong j = 0; j < CDfaSet[i].Size(); ++j)
		{
			for(ulong k = 0; k < CDfaSet[i][j].Size(); ++k)
				fout2 << CDfaSet[i][j][k] << "\t";
			fout2 << std::endl;
		}

	}
	//std::cout << sumBytes << std::endl;
	//std::cout << cnt << std::endl;
	//std::cout << charBytes << std::endl;
	//std::cout << skiptblBytes << std::endl;
	//std::cout << coreBytes << std::endl;
	//std::cout << finalBytes << std::endl;
	//std::cout << maxVal << std::endl;
	system("pause");
}