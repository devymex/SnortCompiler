#include "Hierarchical.h"

typedef struct
{
	ushort maxcol;
	ushort mincol;
} MINMAX;

void main(int nArgs, char **cArgs)
{
	
	CGroupRes groupRes;
	groupRes.ReadFromFile(cArgs[1]/*"FinalResult.cdt"*/);
	CDfaArray &CDfaSet = groupRes.GetDfaTable();
	std::vector<std::vector<SKIPNODE> > skipTable;
	COLCOMBINEARRAY colCombineArray;
	SameColDfaCombine(CDfaSet, colCombineArray);
	CoreCompress(colCombineArray, skipTable);
	WriteSkipTable(colCombineArray, skipTable, "skipTableResult.ppp");
	std::vector<std::vector<ST > > skipTableR;
	ReadSkipTable("skipTableResult.ppp", skipTableR);
	system("pause");
}