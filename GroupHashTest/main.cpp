#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#include <hwprj\ctimer.h>
#include <hwprj\dfarow.h>
#include <hwprj\dfa.h>
#include <hwprj\dfaarray.h>
#include <hwprj\compres.h>
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>
#include <hwprj\buildhash.h>

void main(int nArgs, char **pArgs)
{

	//if (nArgs != 2)
	//{
	//	return;
	//}
	//
	//std::string strFile = pArgs[1];
	//strFile += "result.cdt";
	////编译结果文件

	//CCompileResults result;
	////读入编译结果文件
	//if (0 == result.ReadFromFile(strFile.c_str()))
	//{
	//	CTimer t;
	//	CGroupRes groupRes;
	//	//分组
	//	Grouping(result, groupRes);

	//	HASHRES HashResMap;

	//	//哈希
	//	HashMapping(groupRes, HashResMap);

	//	//存储最终结果的文件
	//	std::string strWriteFileName(pArgs[1]);
	//	strWriteFileName += "FinalResult.cdt";

	//	//写最终结果文件
	//	groupRes.WriteToFile(strWriteFileName.c_str());

	//	//总分组数
	//	std::cout << groupRes.GetGroups().Size() << std::endl;
	//
	//	//使用到的哈希槽数
	//	std::cout << HashResMap.size() << std::endl;

	//	std::cout << "Total time: " << t.Reset() << std::endl;
	//}

	std::string strFile = pArgs[1];
	strFile += "FinalResult.cdt";

	CGroupRes groupRes;
	groupRes.ReadFromFile(strFile.c_str());

	ulong tempMax = 0;
	for (ulong i = 0; i < groupRes.GetSidDfaIds().Size(); ++i)
	{
		if (tempMax < groupRes.GetSidDfaIds()[i].m_dfaIds.Size())
		{
			tempMax = groupRes.GetSidDfaIds()[i].m_dfaIds.Size();
		}
		if (groupRes.GetSidDfaIds()[i].m_dfaIds.Size() == 15)
		{
			std::cout << groupRes.GetSidDfaIds()[i].m_nSid << std::endl;
		}
	}
	std::cout << tempMax << std::endl;

	ulong nTotal = 0;
	ulong nMax = 0;
	for (ulong i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	{
		ulong nCount = groupRes.GetDfaTable()[i].GetGroupCount();
		byte *temp = new byte[nCount];
		std::fill(temp, temp + nCount, 0);
		for (ulong j = 0; j < SC_DFACOLCNT; ++j)
		{
			++temp[groupRes.GetDfaTable()[i].Char2Group(j)];
		}
		nMax = 0;
		for (ulong j = 0; j < nCount; ++j)
		{
			if (nMax < temp[j])
			{
				nMax = temp[j];
			}
		}
		delete[] temp;
		if (nMax < 128)
		{
			std::cout << "error" << std::endl;
			system("pause");
		}
		nTotal += 2 * (256 - nMax + 1);
	}

	std::cout << nTotal << std::endl;

	//CGroupRes groupRes;
	//groupRes.ReadFromFile("..\\FinalResult.cdt");

	//ulong nTotal = 0;
	//ulong nReduceTotal = 0;
	//for (ulong i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	//{
	//	ulong nCol = groupRes.GetDfaTable()[i].GetGroupCount();
	//	ulong *matrix = new ulong[nCol * nCol];
	//	for (ulong r = 0; r < nCol; ++r)
	//	{
	//		for (ulong c = 0; c < nCol; ++c)
	//		{
	//			if (c == r)
	//			{
	//				matrix[r * nCol + c] = 0;
	//			}
	//			else
	//			{
	//				ulong tmp = 0;
	//				for (ulong j = 0; j < groupRes.GetDfaTable()[i].Size(); ++j)
	//				{
	//					byte a = groupRes.GetDfaTable()[i][j][r];
	//					byte b = groupRes.GetDfaTable()[i][j][c];
	//					if (a == b)
	//					//if (groupRes.GetDfaTable()[i][j][r] == groupRes.GetDfaTable()[i][j][c])
	//					{
	//						++tmp;
	//					}
	//				}
	//				matrix[r * nCol + c] = tmp;
	//			}
	//		}
	//	}

	//	int max = 0;
	//	std::vector<ulong> vecCols;
	//	//ulong nComCol;
	//	for (ulong r = 0; r < nCol; ++r)
	//	{
	//		ulong sum = 0;
	//		ulong nNumOfIdx = 0;
	//		std::vector<ulong> vecColsTmp;
	//		for (ulong c = 0; c < nCol; ++c)
	//		{
	//			if (matrix[r * nCol + c] > groupRes.GetDfaTable()[i].Size() * 0.5)
	//			{
	//				++nNumOfIdx;
	//				vecColsTmp.push_back(c);
	//				sum += matrix[r * nCol + c];
	//			}
	//		}
	//		int nReduce = groupRes.GetDfaTable()[i].Size() * nNumOfIdx - ((groupRes.GetDfaTable()[i].Size() * nNumOfIdx - sum) * 2 + groupRes.GetDfaTable()[i].Size());
	//		if (max < nReduce)
	//		{
	//			//nComCol = r;
	//			vecCols = vecColsTmp;
	//			max = nReduce;
	//		}
	//	}

	//	nReduceTotal += max;

	//	delete[] matrix;

	//	std::vector<ulong> vecRemainCols;
	//	ulong idx = 0;
	//	for (ulong c = 0; c < nCol; ++c)
	//	{
	//		if (idx < vecCols.size() && c == vecCols[idx])
	//		{
	//			++idx;
	//		}
	//		else
	//		{
	//			vecRemainCols.push_back(c);
	//		}
	//	}

	//	ulong nRow = groupRes.GetDfaTable()[i].Size();
	//	matrix = new ulong[nRow * nRow];
	//	ulong nRemainCol = vecRemainCols.size();
	//	for (ulong r = 0; r < nRow; ++r)
	//	{
	//		for (ulong c = 0; c < nRow; ++c)
	//		{
	//			if (c == r)
	//			{
	//				matrix[r * nRow + c] = 0;
	//			}
	//			else
	//			{
	//				ulong tmp = 0;
	//				for (ulong j = 0; j < nRemainCol; ++j)
	//				{
	//					byte a = groupRes.GetDfaTable()[i][r][vecRemainCols[j]];
	//					byte b = groupRes.GetDfaTable()[i][c][vecRemainCols[j]];
	//					if (a == b)
	//					//if (groupRes.GetDfaTable()[i][j][r] == groupRes.GetDfaTable()[i][j][c])
	//					{
	//						++tmp;
	//					}
	//				}
	//				matrix[r * nRow + c] = tmp;
	//			}
	//		}
	//	}

	//	std::string str = "F:\\DFAS\\";
	//	std::stringstream ss;
	//	ss << i + 1;
	//	std::string fileNum;
	//	ss >> fileNum;
	//	str += fileNum;
	//	str += ".txt";

	//	std::ofstream fout(str);

	//	for (ulong j = 0; j < groupRes.GetDfaTable()[i].Size(); ++j)
	//	{
	//		for (ulong k = 0; k < vecRemainCols.size(); ++k)
	//		{
	//			fout << groupRes.GetDfaTable()[i][j][vecRemainCols[k]] << ' ';
	//		}
	//		fout << std::endl;
	//	}
	//	fout.clear();
	//	fout.close();

	//	max = 0;
	//	std::vector<ulong> vecRows;
	//	ulong nComRow;
	//	for (ulong r = 0; r < nRow; ++r)
	//	{
	//		ulong sum = 0;
	//		ulong nNumOfIdx = 0;
	//		std::vector<ulong> vecRowsTmp;
	//		for (ulong c = 0; c < nRow; ++c)
	//		{
	//			if (matrix[r * nRow + c] > nRemainCol * 0.5)
	//			{
	//				++nNumOfIdx;
	//				vecRowsTmp.push_back(c);
	//				sum += matrix[r * nRow + c];
	//			}
	//		}
	//		int nReduce = nNumOfIdx * nRemainCol - ((nNumOfIdx * nRemainCol - sum) * 2);
	//		if (max < nReduce)
	//		{
	//			nComRow = r;
	//			vecRows = vecRowsTmp;
	//			max = nReduce;
	//		}
	//	}

	//	std::vector<ulong> vecRemainRows;
	//	idx = 0;
	//	for (ulong r = 0; r < nRow; ++r)
	//	{
	//		if (idx < vecRows.size() && r == vecRows[idx])
	//		{
	//			++idx;
	//		}
	//		else
	//		{
	//			vecRemainRows.push_back(r);
	//		}
	//	}

	//	nReduceTotal += max;
	//	nTotal += groupRes.GetDfaTable()[i].Size() * groupRes.GetDfaTable()[i].GetGroupCount();

	//	delete[] matrix;
	//}

	//std::cout << nTotal << std::endl;
	//std::cout << nReduceTotal << std::endl;
	//std::cout << nReduceTotal / (double)nTotal << std::endl;

	system("pause");
}