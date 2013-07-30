#include "Hierarchical.h"

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

void main(int nArgs, char **cArgs)
{
	CGroupRes groupRes;
	groupRes.ReadFromFile(cArgs[1]);
	CDfaArray &CDfaSet = groupRes.GetDfaTable();

	//核矩阵列映射后，跳转表与核矩阵的存储空间之和
	ulong sumBytes = 0;
	//能够进行列压缩的组数
	ulong cnt = 0;
	//字符集映射表大小
	ulong charBytes = 0;
	//跳转表大小
	ulong skiptblBytes = 0;
	//核心矩阵大小
	ulong coreBytes = 0;
	//终态集合大小
	ulong finalBytes = 0;

	for (size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		ulong nExtraMem = 0;
		std::cout << i << std::endl;

		////展开DFA为256列
		//CDfa unflodDfa;
		//UnflodDFA(CDfaSet[i],unflodDfa);

		ulong memSize;
		VECROWSET coreMatrix;
		//层次聚类方法，输入一个DFA，给出压缩后跳转表和核矩阵的存储空间大小以及核矩阵内容
		Hierarchical(CDfaSet[i], memSize, coreMatrix);

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
		////核矩阵列压缩
		//ColMergeCompress(coreMatrix, colCnt, colGroup, colNum, FinalMatrix);

		////核矩阵列压缩后存储的空间大小
		//size_t cost = memSize;
		//size_t cost2 = memSize - unflodDfa.GetGroupCount() * coreMatrix.size() + colNum * coreMatrix.size();
		//if (cost > cost2)
		//{
		//	cost = cost2;
		//	++cnt;
		//}
		//sumBytes += cost;

		////额外的存储空间大小，包括8个字节固定相关信息，字符集大小（不包括列映射大小），终态集大小
		//nExtraMem = 8 + 2 * Charset(CDfaSet[i]) + 2 * CDfaSet[i].GetFinalStates().CountDfaIds();

		////输出存储空间相关信息
		//std::ofstream fout("storesize.txt", std::ios::app);
		//fout << i << '\t' << memSize << std::endl;
		//fout.close();

		////输出核矩阵相关信息
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

	//std::cout << sumBytes << std::endl;
	//std::cout << cnt << std::endl;
	//std::cout << charBytes << std::endl;
	//std::cout << skiptblBytes << std::endl;
	//std::cout << coreBytes << std::endl;
	//std::cout << finalBytes << std::endl;
	//std::cout << maxVal << std::endl;

	system("pause");
}