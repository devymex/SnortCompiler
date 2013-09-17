#include "Hierarchical.h"


//建立新的映射
void CreateNewMap(VECROWSET &allCharset, VECROWSET &newCharset, std::vector<std::map<size_t, size_t>> &mapvO2N)
{
	std::map<size_t, size_t>::iterator iter;
	for (ulong i = 0; i < allCharset.size(); ++i)
	{
		std::map<size_t, size_t> o2n;
		size_t cnt = 0;	
		size_t j = 0;
		for (size_t index = 0; index < 32; index++)
		{
			iter = o2n.find(allCharset[i][index]);
			if (iter == o2n.end())
			{
				o2n.insert(std::map<size_t, size_t>::value_type(allCharset[i][index], cnt));
				newCharset[i][index] = cnt;
				cnt++;
			}else 
			{
				newCharset[i][index] = iter->second;
			}			
		}

		for (size_t index = 128; index < 256; index++)
		{
			iter = o2n.find(allCharset[i][index]);
			if (iter == o2n.end())
			{
				o2n.insert(std::map<size_t, size_t>::value_type(allCharset[i][index], cnt));			
				newCharset[i][index] = cnt;
				cnt++;
			}else 
			{
				newCharset[i][index] = iter->second;
			}			
		}

		for (size_t index = 32; index < 128; index++)
		{
			iter = o2n.find(allCharset[i][index]);
			if (iter == o2n.end())
			{
				o2n.insert(std::map<size_t, size_t>::value_type(allCharset[i][index], cnt));		
				newCharset[i][index] = cnt;
				cnt++;
			}else 
			{
				newCharset[i][index] = iter->second;
			}			
		}
		mapvO2N.push_back(o2n);

	}
}

//根据新的映射调整DFA表的列
void AdjustDfa(CDfaArray &DfaArr, std::vector<std::map<size_t, size_t>> &mapv)
{
	CDfaArray tempArr = DfaArr;
	std::map<size_t, size_t>::iterator iter;
	for (ulong i = 0; i < DfaArr.Size(); ++i)
	{	
		std::map<size_t, size_t> m = mapv[i];
		for(ulong j = 0; j < DfaArr[i].Size(); ++j)
		{ 
			size_t pre = size_t(DfaArr[i].Char2Group(j));
			iter = m.find(pre);
			if (iter != m.end())
			{
				size_t preCol = iter->first;
				size_t sufCol = iter->second;
				// 将DfaArr[i]的第preCOl列复制至tempArr[i]的第sufCol列
				for (ulong k = 0; k < DfaArr[i].Size(); ++k)
				{
					tempArr[i][k][sufCol] = DfaArr[i][k][preCol];
				}	
			}
		}
	}
	DfaArr = tempArr;
}


//非默认字符个数
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

//核矩阵列字符集压缩
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

void Hierarchical(CDfa &dfa, ulong &memSize, VECROWSET &coreMatrix, GRAPH &graph)
{
	ROWSET rows;
	for (size_t j = 0; j < dfa.Size(); ++j)
	{
		rows.push_back(j);
	}

	//ywwGRAPH graph;
	ROWSET weightArg;
	BuildGraph(dfa, graph, weightArg);

	std::vector<BLOCK> blocks;
	blocks.push_back(BLOCK());
	blocks.back().weightIdx = 0;
	blocks.back().nodes = rows;
	SplitGraph(dfa, graph, weightArg, blocks);

	memSize = StatisticMemory(dfa, blocks, coreMatrix);
}

// 测试
size_t SortCharset(VECROWSET &allCharset, size_t threshold)
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

	return lastCharset.size();
}

void main(int nArgs, char **cArgs)
{
	//核矩阵列映射后，跳转表与核矩阵的存储空间之和
	ulong sumBytes = 0;
	ulong orisumBytes = 0;
	//能够进行列压缩的组数
	ulong cnt = 0;
	ulong oricnt = 0;
	//字符集映射表大小
	ulong charBytes = 0;
	ulong oricharBytes = 0;
	//跳转表大小
	ulong skiptblBytes = 0;
	ulong oriskiptblBytes = 0;
	//核心矩阵大小
	ulong coreBytes = 0;
	ulong oricoreBytes = 0;
	//终态集合大小
	ulong finalBytes = 0;
	ulong orifinalBytes = 0;

	// DFA读入
	CGroupRes groupRes;
	groupRes.ReadFromFile(cArgs[1]);
	CDfaArray &CDfaSet = groupRes.GetDfaTable();


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
	}
	VECROWSET newCharset = allCharset;
	std::vector<std::map<size_t, size_t>> mapvO2N;

	// 对未调整序列的DFA进行测试
	for (size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		
		CDfa dfa = CDfaSet[i];
		ulong memSize;
		VECROWSET coreMatrix;
		GRAPH graph;
		Hierarchical(dfa, memSize, coreMatrix, graph);			
		oriskiptblBytes += (memSize - dfa.GetGroupCount() * coreMatrix.size());
		oricoreBytes += dfa.GetGroupCount() * coreMatrix.size();
		orifinalBytes += 2 *dfa.GetFinalStates().CountDfaIds();
		//charBytes += 2 * Charset(CDfaSet[i]);

		ulong colNum = 0;
		byte colGroup[256] = {0};
		ulong colCnt = dfa.GetGroupCount();
		std::vector<CDfaRow> FinalMatrix;
		//核矩阵列压缩
		ColMergeCompress(coreMatrix, colCnt, colGroup, colNum, FinalMatrix);

		//核矩阵列压缩后存储的空间大小
		size_t cost = memSize;
		size_t cost2 = memSize - dfa.GetGroupCount() * coreMatrix.size() + colNum * coreMatrix.size();
		if (cost > cost2)
		{
			cost = cost2;
			++oricnt;
		}
		orisumBytes += cost;
	}


	// 调整字符表的序列，相应的调整DFA的序列
	CreateNewMap(allCharset, newCharset, mapvO2N);	
	size_t comCharNum = SortCharset(newCharset, 32);
	size_t comMem = comCharNum * 160;
	size_t priMem = 1588 * 96;
	charBytes = comMem + priMem;
	oricharBytes = 1588 * 256;
	AdjustDfa(CDfaSet, mapvO2N);

	
	// 对调整序列后的DFA进行测试
	for (size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		

		CDfa dfa = CDfaSet[i];
		ulong memSize;
		VECROWSET coreMatrix;
		GRAPH graph1, graph2;
		Hierarchical(dfa, memSize, coreMatrix, graph1);	
		
		skiptblBytes += (memSize - dfa.GetGroupCount() * coreMatrix.size());
		coreBytes += dfa.GetGroupCount() * coreMatrix.size();
		finalBytes += 2 *dfa.GetFinalStates().CountDfaIds();
		//charBytes += 2 * Charset(CDfaSet[i]);

		ulong colNum = 0;
		byte colGroup[256] = {0};
		ulong colCnt = dfa.GetGroupCount();
		std::vector<CDfaRow> FinalMatrix;
		//核矩阵列压缩
		ColMergeCompress(coreMatrix, colCnt, colGroup, colNum, FinalMatrix);


		//核矩阵列压缩后存储的空间大小
		size_t cost = memSize;
		size_t cost2 = memSize - dfa.GetGroupCount() * coreMatrix.size() + colNum * coreMatrix.size();
		if (cost > cost2)
		{
			cost = cost2;
			++cnt;
		}
		sumBytes += cost;
	}

	

	std::cout << "The storage space of various data structure" << std::endl;

	std::cout << "Original skip table: " << oriskiptblBytes << std::endl;
	std::cout << "Original core matrix: " << orisumBytes - oriskiptblBytes << std::endl;
	std::cout << "Originalskip table and original core matrix: " << orisumBytes << std::endl;		
	std::cout << "Original map table for the character set: " << oricharBytes << std::endl;
	std::cout << "Original Final set: " << orifinalBytes << std::endl;	
	std::cout << "The original number of the group that can be column compressed: " << oricnt << std::endl;
	std::cout << std::endl;
	std::cout << "Skip table: " << skiptblBytes << std::endl;
	std::cout << "Core matrix: " << sumBytes - skiptblBytes << std::endl;
	std::cout << "Skip table and core Matrix: " << sumBytes << std::endl;
	std::cout << "Map table for the character set: " << charBytes << std::endl;
	std::cout << "Final set: " << finalBytes << std::endl;		
	std::cout << "The number of the group that can be column compressed: " << cnt << std::endl;

	
	system("pause");
}