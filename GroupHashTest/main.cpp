#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <sstream>
#include <tchar.h>
#include <filesystem>

#include <hwprj\ctimer.h>
#include <hwprj\unsary.h>
#include <hwprj\compiler.h>
#include <hwprj\compres.h>
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>
#include <hwprj\buildhash.h>
#include <hwprj\trace.h>

int main()
{
	// Defina a path object to express a directory
	//std::tr2::sys::path rulePath("D:\\Projects\\VS2012\\SnortCompiler\\rules\\");
	//// Construct a directory iterator for visit this path.
	//std::tr2::sys::directory_iterator iDirCur(rulePath);
	////the end iterator for this path.
	//std::tr2::sys::directory_iterator iDirEnd;

	//CCompileResults result;
	//char szExt[] = {'s', 'e', 'l', 'u', 'r'};
	//for (; iDirCur != iDirEnd; ++iDirCur)
	//{
	//	const std::tr2::sys::path &curPath = *iDirCur;
	//	if (!std::tr2::sys::is_directory(curPath))
	//	{
	//		std::string strFullName = rulePath.directory_string();
	//		strFullName.append(curPath.directory_string());
	//		if (strFullName.size() < 5)
	//		{
	//			continue;
	//		}
	//		std::string::reverse_iterator rb = strFullName.rbegin();
	//		std::string::reverse_iterator re = rb + 5;
	//		std::string::reverse_iterator ri = rb;
	//		for (; ri != re; ++ri)
	//		{
	//			if (tolower(*ri) != tolower(szExt[ri - rb]))
	//			{
	//				break;
	//			}
	//		}
	//		if (ri != re)
	//		{
	//			continue;
	//		}
	//		std::cout << strFullName << std::endl;
	//		try
	//		{
	//			CompileRuleFile(strFullName.c_str(), result);
	//		}
	//		catch (CTrace &e)
	//		{
	//			std::cout << e.File() << " - " << e.Line() << ": " << e.What() << std::endl;
	//			system("pause");
	//		}
	//	}
	//}
	//result.WriteToFile("..\\result.cdt");

	//ulong ulRuleCnt = 0;
	//for (ulong i = 0; i < result.GetSidDfaIds().Size(); ++i)
	//{
	//	if (result.GetSidDfaIds()[i].m_nResult == COMPILEDINFO::RES_SUCCESS)
	//	{
	//		++ulRuleCnt;
	//	}
	//}
	//std::cout << result.GetSidDfaIds().Size() << ": " << ulRuleCnt << std::endl;

	//CCompileResults result;
	//result.ReadFromFile("..\\result.cdt");

	//CGroupRes groupRes;
	//groupRes.ReadFromFile("..\\FinalResult.cdt");

	//std::ofstream fout("..\\Ids.txt");
	//for (ulong i = 0; i < groupRes.GetGroups().Size(); ++i)
	//{
	//	for (ulong j = 0; j < groupRes.GetGroups()[i].DfaIds.Size(); ++j)
	//	{
	//		fout << groupRes.GetGroups()[i].DfaIds[j] << " ";
	//	}
	//	fout << std::endl;
	//}
	//fout.clear();
	//fout.close();

	//std::ofstream foutMerge("..\\MergeId.txt");
	//for (ulong i = 0; i < groupRes.GetGroups().Size(); ++i)
	//{
	//	foutMerge << groupRes.GetDfaTable()[groupRes.GetGroups()[i].mergeDfaId].GetFinalStates().CountDfaIds() << std::endl;
	//}
	//foutMerge.clear();
	//foutMerge.close();

	//CTimer ctime;
	//CCompileResults result;
	//try
	//{
	//	CompileRuleFile("..\\allrules.rule", result);
	//}
	//catch (CTrace &e)
	//{
	//	std::cout << e.File() << " - " << e.Line() << ": " << e.What() << std::endl;
	//	system("pause");
	//}
	//result.WriteToFile("..\\result.cdt");

	////result.ReadFromFile("..\\result.cdt");

	//CGroupRes groupRes;
	//Grouping(result, groupRes);
	//groupRes.WriteToFile("..\\GroupResult.cdt");
	////groupRes.ReadFromFile("..\\GroupResult.cdt");
	//HASHRES HashResMap;
	//HashMapping(groupRes, HashResMap);
	//groupRes.WriteToFile("..\\FinalResult.cdt");
	////for (ulong i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	////{
	////	groupRes.GetDfaTable()[i].MergeColumn();
	////}
	//std::cout << "Total time: " << ctime.Reset() << std::endl;
	//std::cout << groupRes.GetGroups().Size() << std::endl;
	//std::cout << HashResMap.size() << std::endl;

	CGroupRes groupRes;
	groupRes.ReadFromFile("..\\FinalResult.cdt");

	//std::ofstream fout1("F:\\cppProject\\huawei\\PreciseMatch\\CompressTest\\ori_size.txt");
	//if(!fout1)
	//{
	//	std::cout << "" << std::endl;
	//	return 0;
	//}
	//ulong ori_size = 0;//原始大小
	//for(ulong i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	//{
	//	ulong dfa_size = 0;
	//	dfa_size += 256;//m_pGroup的大小
	//	ulong gCnt = groupRes.GetDfaTable()[i].GetGroupCount();
	//	ulong size = groupRes.GetDfaTable()[i].Size() * gCnt;
	//	dfa_size += size;//dfa表中元素的个数
	//	dfa_size += gCnt;

	//	fout1 << i << "\t" << dfa_size << std::endl;
	//	ori_size += dfa_size;
	//}

	//fout1 << std::endl << ori_size << std::endl;
	//fout1.close();

	std::ofstream fout2("F:\\cppProject\\huawei\\PreciseMatch\\CompressTest\\new_size_1.txt");
	if(!fout2)
	{
		std::cout << "" << std::endl;
		return 0;
	}
	ulong new_size = 0;
	for(ulong i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	{
		ulong tmpSize = 0;

		CDfa &dfa = groupRes.GetDfaTable()[i];
		ulong colCnt = dfa.GetGroupCount();
		ulong dfasize = dfa.Size();
		for(ulong j = 0; j < dfasize; ++j)
		{
			std::vector<STATEID> tmp(SC_DFACOLCNT + 1, 0);
			for(STATEID k = 0; k < colCnt; ++k)
			{
				STATEID tmpk = dfa[j][k];
				if(tmpk == (STATEID)-1)
				{
					++tmp[SC_DFACOLCNT];//-1的个数
				}
				else
				{
					++tmp[tmpk];
				}
			}

			ulong max = 0;//出现最多的列数
			STATEID maxid = 0;
			for(STATEID k = 0; k <= SC_DFACOLCNT; ++k)
			{
				if(tmp[k] > max)
				{
					max = tmp[k];//中存的是k出现的个数
					if(k == SC_DFACOLCNT)
					{
						maxid = STATEID(-1);
					}
					else
					{
						maxid = k;
					}
				}
			}

			//tmpSize += 2;//1个字节存放colCnt - max(用于写入和读出文件),1字节存放maxid(default值)
			//for(STATEID k = 0; k < colCnt; ++k)
			//{
			//	if(dfa[j][k] != maxid)
			//	{
			//		//如果这个元素不是maxid，那么求出k列对应了哪些字符集合
			//		for(STATEID m = 0; m < SC_DFACOLCNT; ++m)
			//		{
			//			if(dfa.Char2Group(m) == k)
			//			{
			//				tmpSize += 2;//表示存储一个<k, char>对
			//			}
			//		}
			//	}
			//}
			tmpSize += 2;
			for(STATEID k = 0; k < colCnt; ++k)
			{
				if(dfa[j][k] != maxid)
				{
					tmpSize += 2;
				}
			}
		}
		tmpSize += 256;
		fout2 << i << "\t" << tmpSize << std::endl;
		new_size += tmpSize;
	}

	fout2 << std::endl << new_size << std::endl;
	fout2.close();

	//std::string str = "F:\\cppProject\\huawei\\PreciseMatch\\CompressTest\\";
	//std::ofstream fout;
	//for(ulong i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	//{
	//	std::string strFile = str;
	//	std::stringstream ss;
	//	ss << i;
	//	strFile += ss.str() + ".txt";
	//	fout.open(strFile.c_str());
	//	if(!fout)
	//	{
	//		std::cout << "Open Failure!" << std::endl;
	//		return 0;
	//	}
	//	groupRes.GetDfaTable()[i].Dump(strFile.c_str());
	//	fout.close();
	//}

	//输出每个dfa的大小
	//std::ofstream fout("F:\\cppProject\\huawei\\PreciseMatch\\CompressTest\\dfasize.txt");
	//if(!fout)
	//{
	//	std::cout << "Open File Failure!" << std::endl;
	//	return 0;
	//}
	//fout << "序号\t" << "dfa大小(B)" << std::endl; 
	//for(ulong i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	//{
	//	fout << i << "\t" << groupRes.GetDfaTable()[i].CalcStoreSize() << std::endl;
	//}
	//fout.close();
	
	//std::vector<byte> v(5);

	//CCompileResults result;
	//result.ReadFromFile("..\\result.cdt");
	//std::ifstream fin("..\\Ids.txt");
	//std::ifstream finMerge("..\\MergeId.txt");
	//std::vector<size_t> mergeDfaSize;
	//size_t id;
	//while (finMerge >> id)
	//{
	//	mergeDfaSize.push_back(id);
	//}

	//size_t count = 0;
	//std::string str;
	//while (std::getline(fin, str))
	//{
	//	//std::cout << count << std::endl;
	//	std::stringstream ss(str);
	//	std::vector<size_t> vecIds;
	//	while (ss >> id)
	//	{
	//		vecIds.push_back(id);
	//	}

	//	CDfaArray vecDfas;
	//	vecDfas.Resize(2);
	//	vecDfas[0] = result.GetDfaTable()[vecIds[0]];
	//	for (ulong i = 1; i < vecIds.size(); ++i)
	//	{
	//		vecDfas[1] = result.GetDfaTable()[vecIds[i]];
	//		CDfa MergeDfa;
	//		if (!MergeMultipleDfas(vecDfas, MergeDfa))
	//		{
	//			std::cout << "error" << std::endl;
	//		}
	//		vecDfas[0] = MergeDfa;
	//	}
	//	if (vecDfas[0].GetFinalStates().CountDfaIds() != mergeDfaSize[count++])
	//	{
	//		std::cout << "error" << std::endl;
	//	}
	//}

	//std::ifstream fin("..\\allrules.rule");
	//std::vector<std::string> vecRules;
	//std::string str;
	//while (std::getline(fin, str))
	//{
	//	vecRules.push_back(str);
	//}
	//fin.clear();
	//fin.close();

	//CGroupRes groupRes;
	//groupRes.ReadFromFile("..\\FinalResult.cdt");
	//std::ofstream fout("..\\CanCompile.rule");
	//for (ulong i = 0; i < groupRes.GetSidDfaIds().Size(); ++i)
	//{
	//	if (groupRes.GetSidDfaIds()[i].m_nResult == COMPILEDINFO::RES_SUCCESS)
	//	{
	//		str = "sid:";
	//		std::string sid;
	//		std::stringstream ss;
	//		ss << groupRes.GetSidDfaIds()[i].m_nSid;
	//		ss >> sid;
	//		str += sid;
	//		str += ";";
	//		for (ulong j = 0; j < vecRules.size(); ++j)
	//		{
	//			if (vecRules[j].find(str, 0) != std::string::npos)
	//			{
	//				fout << vecRules[j] << std::endl;
	//			}
	//		}
	//	}
	//}
	//fout.clear();
	//fout.close();
	//std::cout << HashResMap.size() << std::endl;

	system("pause");
	return 0;
}