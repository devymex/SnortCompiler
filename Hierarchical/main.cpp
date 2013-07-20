#include "Hierarchical.h"


void main(int nArgs, char **cArgs)
{
	CGroupRes groupRes;
	groupRes.ReadFromFile(cArgs[1]);
	CDfaArray &CDfaSet = groupRes.GetDfaTable();

	for (size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		if (i == 2)
		{
			ROWSET rows;
			for (size_t j = 0; j < CDfaSet[i].Size(); ++j)
			{
				rows.push_back(j);
			}
			GRAPH graph;
			BuildGraph(CDfaSet[i], rows, graph);

			VECROWSET vecRows;
			SearchConnectSubgraph(graph, vecRows);

			HierarchicalCluster(CDfaSet[i], vecRows);
		}
	}

	std::cout << g_TotalMem << std::endl;

	system("pause");
}