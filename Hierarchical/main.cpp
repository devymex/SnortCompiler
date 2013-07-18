#include "Hierarchical.h"

void main(int nArgs, char **cArgs)
{
	CGroupRes groupRes;
	groupRes.ReadFromFile(cArgs[1]);
	CDfaArray &CDfaSet = groupRes.GetDfaTable();

	for (size_t i = 0; i < CDfaSet.Size(); ++i)
	{
		if (i == 33)
		{
			ROWSET rows;
			std::cout << CDfaSet[i].Size();
			for (size_t j = 0; j < CDfaSet[i].Size(); ++j)
			{
				rows.push_back(j);
			}
			rows.pop_back();
			size_t n_size = StatisticVitualCore(CDfaSet[i], rows);
	/*	GRAPH graph;

			BuildGraph(CDfaSet[i], rows, graph);

			VECROWSET vecRows;

			SearchConnectSubgraph(graph, rows, vecRows);
		HierarchicalCluster(CDfaSet[i], vecRows);*/
		}
	}

	system("pause");
}