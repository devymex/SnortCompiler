#include "Hierarchical.h"

void main(int nArgs, char **cArgs)
{	
	CGroupRes groupRes;
	groupRes.ReadFromFile(cArgs[1]);
	CDfaArray &CDfaSet = groupRes.GetDfaTable();
	std::vector<std::vector<SKIPNODE> > skipTable;
	COLCOMBINEARRAY colCombineArray;
	SameColDfaCombine(CDfaSet, colCombineArray);
	CoreCompress(colCombineArray, skipTable);
	WriteSkipTable(colCombineArray, skipTable, cArgs[2]);
	std::vector<std::vector<std::vector<char> > > skipTableR;
	ReadSkipTable(cArgs[2], skipTableR);
	system("pause");
}