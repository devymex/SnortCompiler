#include "Hierarchical.h"



void main(int nArgs, char **cArgs)
{
	CDfaArray SameColDfa;
	std::map<ushort, Attribute> columnNum;
	SameColDfaCombine(SameColDfa, columnNum);
	system("pause");
}