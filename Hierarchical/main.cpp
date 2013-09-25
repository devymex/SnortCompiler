#include "Hierarchical.h"

typedef struct
{
	ushort maxcol;
	ushort mincol;
} MINMAX;

void main(int nArgs, char **cArgs)
{
	COLCOMBINEARRAY colCombineArray;
	SameColDfaCombine(colCombineArray);
	CoreCompress(colCombineArray);
	system("pause");
}