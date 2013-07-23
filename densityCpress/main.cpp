#include "stdafx.h"
#include <hwprj/groupres.h>
#include <hwprj/dfaarray.h>
#include <hwprj/dfa.h>
#include "denCpressFun.h"
//#include "densityCpress.h"

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

void main()
{
	ulong sumBytes = 0;
	ulong cnt = 0;
	CGroupRes groupRes;
	groupRes.ReadFromFile("..\\..\\output\\FinalResult.cdt");
	CDfaArray &dfaary = groupRes.GetDfaTable();

	for (ulong dfanum = 0; dfanum < dfaary.Size(); ++dfanum)
	{
		std::cout << dfanum << std::endl;
		CDfa &dfa = dfaary[dfanum];
		ulong dfasize = dfa.Size();
		double *disMatrix = new double[dfasize * (dfasize - 1)/2];
		SetDistance(dfa, disMatrix);

		DenCpressDfa cpressDfa(dfasize);
	//	cpressDfa.SetStaSize(dfasize);
	//	cpressDfa.InitRowDIf();
		int cost, parCost, cost1, cost2;
		parCost = OPTICS(dfa, disMatrix, 0.5, 2, cpressDfa);

		//½øÐÐÁÐÑ¹Ëõ
		ulong colNum = 0;
		byte colGroup[256] = {0};
		ulong colCnt = dfa.GetGroupCount();
		std::vector<CDfaRow> FinalMatrix;
		ColMergeCompress(cpressDfa, colCnt, colGroup, colNum, FinalMatrix);

		CFinalStates &finalSta = dfa.GetFinalStates();
		cost1 = parCost + 8 + 2 * Charset(dfa) + dfa.GetGroupCount() * cpressDfa.Size() + 2 * finalSta.CountDfaIds();
		cost2 = parCost + 8 + 2 * Charset(dfa) + colNum * cpressDfa.Size() + 2 * finalSta.CountDfaIds() + colCnt;
		cost = cost1;
		if(cost2 < cost1)
		{
			cost = cost2;
			++cnt;
		}
		sumBytes += cost;

		//size_t c1 = parCost + dfa.GetGroupCount() * cpressDfa.Size();
		//size_t c2 = 8 + 2 * Charset(dfa) + 2 * finalSta.CountDfaIds();

		//if (dfanum == 97)
		//{
		//	std::cout << cpressDfa.Size() << std::endl;

		//	size_t dif = 0;
		//	for (size_t i = 0; i< dfa.Size(); ++i)
		//	{
		//		dif += cpressDfa.getDif(i).size();
		//	}
		//	std::cout << dif << std::endl;
		//	
		//	for (size_t i = 0; i < cpressDfa.Size(); ++i)
		//	{
		//		CDfaRow &row = cpressDfa.getCurCluster(i);
		//		for (size_t j = 0; j < row.Size(); ++j)
		//		{
		//			std::cout << row[j] << " ";
		//		}
		//		std::cout << std::endl;
		//	}
		//}

		std::ofstream ofscore;
		if (dfanum == 0)
		{
			ofscore.open("..\\..\\output\\core.txt");
		}
		else
		{
			ofscore.open("..\\..\\output\\core.txt", std::ofstream::app);
		}

		ofscore << dfanum << " : " << std::endl;
		for (size_t i = 0; i < cpressDfa.Size(); ++i)
		{
			CDfaRow &row = cpressDfa.getCurCluster(i);
			for (size_t j = 0; j < row.Size(); ++j)
			{
				ofscore << (ushort)row[j] << "\t";
			}
			ofscore << std::endl;
		}

		ofscore << std::endl << std::endl;


		std::ofstream ofs;
		if (dfanum == 0)
		{
			ofs.open("..\\..\\output\\reduction.txt");
			//ofs << dfanum << " : " << c1 << "\t" << c2 << "\t" << cost << std::endl;
			ofs << dfanum << "\t" << cost << std::endl;
		}
		else
		{
			ofs.open("..\\..\\output\\reduction.txt", std::ofstream::app);
			//ofs << dfanum << " : " << c1 << "\t" << c2 << "\t" << cost << std::endl;
			ofs << dfanum << "\t" << cost << std::endl;
		}
	}

	std::cout << sumBytes << std::endl;
	std::cout << cnt << std::endl;
	system("pause");
}