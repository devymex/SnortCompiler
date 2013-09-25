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
	
	COLCOMBINEARRAY colCombineArray;
	SameColDfaCombine(CDfaSet, colCombineArray);
	CoreCompress(colCombineArray);
	system("pause");
}