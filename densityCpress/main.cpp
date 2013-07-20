#include "stdafx.h"
#include <hwprj/groupres.h>
#include <hwprj/dfaarray.h>
#include "denCpressFun.h"
//#include "densityCpress.h"

void main()
{
	CGroupRes groupRes;
	groupRes.ReadFromFile("..\\..\\output\\FinalResult.cdt");
	CDfaArray &dfaary = groupRes.GetDfaTable();

	for (ulong dfanum = 0; dfanum < dfaary.Size(); ++dfanum)
	{
		CDfa &dfa = dfaary[dfanum];
		ulong dfasize = dfa.Size();
		double *disMatrix = new double[dfasize * (dfasize - 1)/2];
		SetDistance(dfa, disMatrix);

		DenCpressDfa cpressDfa(dfasize);
	//	cpressDfa.SetStaSize(dfasize);
	//	cpressDfa.InitRowDIf();

		OPTICS(dfa, disMatrix, 0.5, 2, cpressDfa);
	}
}