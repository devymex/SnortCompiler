#include "Hierarchical.h"


typedef struct
{
	ushort maxcol;
	ushort mincol;
} MINMAX;


void main(int nArgs, char **cArgs)
{
	/*CDfaArray SameColDfa;
	std::map<ushort, Attribute> columnNum;
	SameColDfaCombine(SameColDfa, columnNum);*/
	MINMAX minmax[] ={
		64, 51,
		50, 41,
		40, 32,
		31, 25,
		24, 19,
		18, 14,
		14, 11,
		10, 8,
		7, 5,
		4, 2,
	};

	int size = sizeof(minmax) / sizeof(minmax[0]);
	COLCOMBINEARRAY colCombineArray, outComArray;

	COLUMNCOMBINE temp;
	SameColDfaCombine(colCombineArray);

	for(int i = 0; i < size; ++i)
	{
		outComArray.push_back(temp);
		DiffColDfaCombine(colCombineArray, minmax[i].mincol, minmax[i].maxcol, outComArray[i]);
	}
	


	system("pause");
}