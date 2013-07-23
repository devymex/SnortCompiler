#include "Hierarchical.h"

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

void main(int nArgs, char **cArgs)
{
	CGroupRes groupRes;
	groupRes.ReadFromFile(cArgs[1]);
	CDfaArray &CDfaSet = groupRes.GetDfaTable();

	for (size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		ulong nExtraMem = 0;
		std::cout << i << std::endl;
		ROWSET rows;
		for (size_t j = 0; j < CDfaSet[i].Size(); ++j)
		{
			rows.push_back(j);
		}
		GRAPH graph;
		BuildGraph(CDfaSet[i], rows, graph);

		VECROWSET vecRows;
		SearchConnectSubgraph(graph, vecRows);

		VECROWSET vecVirtual;
		size_t memSize = HierarchicalCluster(CDfaSet[i], vecRows, vecVirtual);

		nExtraMem = (8 + 2 * Charset(CDfaSet[i]) + 2 * CDfaSet[i].GetFinalStates().CountDfaIds());

		std::ofstream fout("storesize.txt", std::ios::app);
		fout << i << '\t' << memSize << '\t' << nExtraMem << std::endl;
		fout.close();
		//std::ofstream fout("core.txt", std::ios::app);
		//fout << i << " :" << std::endl;
		//for (NODEARRAY_ITER j = vecVirtual.begin(); j != vecVirtual.end(); ++j)
		//{
		//	for (ROWSET::iterator k = j->begin(); k != j->end(); ++k)
		//	{
		//		fout << (size_t)*k << "\t";
		//	}
		//	fout << std::endl;
		//}
		//fout << std::endl;
		//fout.close();
	}

	//std::cout << "核矩阵和状态跳转表大小：	" << g_TotalMem << " Byte" << std::endl;
	//std::cout << "字符映射表，终态表和标记的8个字节：	" << nExtraMem << " Byte" << std::endl;
	//std::cout << "总空间占用：		" << (g_TotalMem + nExtraMem) << " Byte" << std::endl;

	system("pause");
}