#include <iostream>

#include <hwprj\ctimer.h>
#include <hwprj\compres.h>
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>
#include <hwprj\buildhash.h>
#include <fstream>
#include <algorithm>

void StatisticErrorRules(CCompileResults &result)
{
	std::vector<int> vecCnt;
	vecCnt.resize(8);
	std::fill(vecCnt.begin(), vecCnt.end(), 0);

	CSidDfaIds sidDfaIds = result.GetSidDfaIds();
	ulong sidSize = sidDfaIds.Size();
	for (size_t i = 0; i < sidSize; ++i)
	{
		if (sidDfaIds[i].m_nResult & COMPILEDINFO::RES_EMPTY)
		{
			++vecCnt[0];
		}
		if (sidDfaIds[i].m_nResult & COMPILEDINFO::RES_EXCEEDLIMIT)
		{
			++vecCnt[1];
		}
		if (sidDfaIds[i].m_nResult & COMPILEDINFO::RES_HASBYTE)
		{
			++vecCnt[2];
		}
		if (sidDfaIds[i].m_nResult & COMPILEDINFO::RES_HASNOSIG)
		{
			++vecCnt[3];
		}
		if (sidDfaIds[i].m_nResult & COMPILEDINFO::RES_HASNOT)
		{
			++vecCnt[4];
		}
		if (sidDfaIds[i].m_nResult & COMPILEDINFO::RES_OPTIONERROR)
		{
			++vecCnt[5];
		}
		if (sidDfaIds[i].m_nResult & COMPILEDINFO::RES_PCREERROR)
		{
			++vecCnt[6];
		}
		if (!(sidDfaIds[i].m_nResult | COMPILEDINFO::RES_SUCCESS))
		{
			++vecCnt[7];
		}
	}

	//统计结果输出文件
	std::ofstream fos("errorRules.txt");
	if (fos.is_open())
	{
		fos << "规则总数： " << sidSize << std::endl;
		int errorRulesCnt = sidDfaIds.Size() - vecCnt[7];
		fos << "不能处理的规则数： " << errorRulesCnt 
			<< "; 占规则百分比： " << ((double)errorRulesCnt / (double)sidSize) << std::endl;
		fos << "规则内容为空的规则数： " 
			<< vecCnt[0] << "; 占规则百分比： " << ((double)vecCnt[0] / (double)sidSize) << std::endl;
		fos << "超出状态限制的规则数： " 
			<< vecCnt[1] << "; 占规则百分比： " << ((double)vecCnt[1] / (double)sidSize) << std::endl;
		fos << "包含byte_test和byte_jump的规则数： " 
			<< vecCnt[2] << "; 占规则百分比： " << ((double)vecCnt[2] / (double)sidSize) << std::endl;
		fos << "没有特征字符串的规则数： " 
			<< vecCnt[3] << "; 占规则百分比： " << ((double)vecCnt[3] / (double)sidSize) << std::endl;
		fos << "包含'!'（逻辑非）标识的规则数： " 
			<< vecCnt[4] << "; 占规则百分比： " << ((double)vecCnt[4] / (double)sidSize) << std::endl;
		fos << "snort规则选项错误的规则数： " 
			<< vecCnt[5] << "; 占规则百分比： " << ((double)vecCnt[5] / (double)sidSize) << std::endl;
		fos << "pcre选项错误的规则数： " 
			<< vecCnt[6] << "; 占规则百分比： " << ((double)vecCnt[6] / (double)sidSize) << std::endl;
	}
	fos.close();
}

void StatisticChain(CCompileResults &result)
{
	CDfaArray dfaTblSet = result.GetDfaTable();
	CRegRule dfaSigSet = result.GetRegexTbl();
	ulong stateSum = 0, edgeSum = 0;
	

	for (size_t i = 0; i < dfaTblSet.Size(); ++i)
	{
		ulong TblSize = dfaTblSet[i].Size();
		stateSum += TblSize;
		for (size_t j = 0; j < dfaTblSet[i].Size(); ++j)
		{
			for (size_t k = 0; k < dfaTblSet[i].GetGroupCount(); ++k)
			{
				if (dfaTblSet[i][j][k] != (STATEID)(-1))
				{
					++edgeSum;
				}
			}
		}
	}

	std::vector<ulong> sigSet;
	for (size_t i = 0; i < dfaSigSet.Size(); ++i)
	{
		CUnsignedArray oneDfasig = dfaSigSet[i].GetSigs();
		for (size_t j = 0; j < oneDfasig.Size(); ++j)
		{
			sigSet.push_back(oneDfasig[j]);
		}
	}
	std::sort(sigSet.begin(), sigSet.end());
	sigSet.erase(std::unique(sigSet.begin(), sigSet.end()), sigSet.end());

	std::ofstream fout("signature.txt");
	for (std::vector<ulong>::iterator i = sigSet.begin(); i != sigSet.end(); ++i)
	{
		fout << *i << std::endl;
	}
	fout.close();
	////统计结果输出文件
	//std::ofstream fos("chain.txt");
	//if (fos.is_open())
	//{
	//	fos << "所有的链数： " << dfaTblSet.Size() << std::endl;
	//	fos << "DFA状态总数： " << stateSum << std::endl;
	//	fos << "DFA迁移边总数： " << edgeSum << std::endl;
	//	fos << "所有规则Signature总数： " << sigSet.size() << std::endl;
	//}
	//fos.close();
}

void StatisticOneChain(CGroupRes &groupRes)
{
	CGroups groups = groupRes.GetGroups();
	int oneChain = 0;
	std::vector<ulong> signature;
	for (size_t i = 0; i < groups.Size(); ++i)
	{
		//if (groups[i].DfaIds.Size() == 1)
		//{
		//	++oneChain;
		//}
		signature.push_back(groups[i].currSig);
	}
	std::sort(signature.begin(), signature.end());
	signature.erase(std::unique(signature.begin(), signature.end()), signature.end());

	std::ofstream fout("groupSignature.txt");
	for (std::vector<ulong>::iterator i = signature.begin(); i != signature.end(); ++i)
	{
		fout << *i << std::endl;
	}
	fout.close();
	//std::cout << "分组中只包含一条链的个数： " << oneChain << std::endl;
}

void StatisticSigchar(CCompileResults &result)
{
	CRegRule dfaSigSet = result.GetRegexTbl();
	std::vector<ulong> sigSet;
	for (size_t i = 0; i < dfaSigSet.Size(); ++i)
	{
		CUnsignedArray oneDfasig = dfaSigSet[i].GetSigs();
		for (size_t j = 0; j < oneDfasig.Size(); ++j)
		{
			sigSet.push_back(oneDfasig[j]);
		}
	}
	std::sort(sigSet.begin(), sigSet.end());
	sigSet.erase(std::unique(sigSet.begin(), sigSet.end()), sigSet.end());

	std::vector<std::vector<int>> dvecChar;
	dvecChar.resize(4);
	for (size_t i = 0; i < 4; ++i)
	{
		dvecChar[i].resize(256);
		std::fill(dvecChar[i].begin(), dvecChar[i].end(), 0);
	}

	std::map<unsigned short, int> mapShort;

	for (std::vector<ulong>::iterator i = sigSet.begin(); i != sigSet.end(); ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			std::vector<int> &curPos = dvecChar[j];
			unsigned char sig1byte = *(((unsigned char*)&(*i)) + j);
			++curPos[sig1byte];
		}

		unsigned short sig2byte = *(unsigned short*)((unsigned char*)&(*i));
		++mapShort[sig2byte];
	}

	//int sum = 0, flag = 0;
	//std::vector<std::pair<int, int>> tmp;
	//for (std::map<unsigned short, int>::iterator i = mapShort.begin(); i != mapShort.end(); ++i)
	//{
	//	if (i->first >= flag && i->first < (512 + flag))
	//	{
	//		sum += i->second;
	//	}
	//	else
	//	{
	//		tmp.push_back(std::pair<int, int>());
	//		tmp.back().first = flag;
	//		tmp.back().second = sum;
	//		flag += 512;
	//		sum = 0;
	//	}
	//}

	//std::ofstream fos("slip512.txt");
	//for (std::vector<std::pair<int, int>>::iterator i = tmp.begin(); i != tmp.end(); ++i)
	//{
	//	fos << i->first << "\t" << i->second << std::endl;
	//}

	std::ofstream fos("sig1byte.txt");
	for (size_t j = 0; j < 256; ++j)
	{
		fos << j << "\t";
		for (size_t i = 0; i < 4; ++i)
		{
			fos << dvecChar[i][j] << "\t";
		}
		fos << std::endl;
	}
	fos.close();

	std::ofstream fout("sig2byte.txt");
	for (std::map<unsigned short, int>::iterator i = mapShort.begin(); i != mapShort.end(); ++i)
	{
		fout << i->first << "\t" << i->second << std::endl;
	}
	fout.close();
}

void main(int nArgs, char **pArgs)
{

	//if (nArgs != 2)
	//{
	//	return;
	//}
	//
	//std::string strFile = pArgs[1];
	////编译结果文件

	//CCompileResults result;
	////读入编译结果文件
	//if (0 == result.ReadFromFile(strFile.c_str()))
	//{
	//	//统计错误规则
	//	//StatisticErrorRules(result);

	//	//统计能编译的链
	//	StatisticChain(result);

	//	//统计signature中字符的分布
	//	//StatisticSigchar(result);

	//	CTimer t;
	//	CGroupRes groupRes;
	//	//分组
	//	Grouping(result, groupRes);
	//	//std::cout << "分组过程用时： " << t.Reset() << std::endl;

	//	HASHRES HashResMap;
	//	//哈希
	//	HashMapping(groupRes, HashResMap);
	//	//std::cout << "哈希过程用时： " << t.Reset() << std::endl;

	//	//统计只包含一条链的分组个数
	//	StatisticOneChain(groupRes);

	//	//存储最终结果的文件
	//	std::string strWriteFileName(pArgs[1]);
	//	std::string::iterator iPos = std::find(strWriteFileName.begin(), strWriteFileName.end(), '\\');
	//	strWriteFileName.erase(iPos + 1, strWriteFileName.end());
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

	std::string strWriteFileName(pArgs[1]);
	std::string::iterator iPos = std::find(strWriteFileName.begin(), strWriteFileName.end(), '\\');
	strWriteFileName.erase(iPos + 1, strWriteFileName.end());
	strWriteFileName += "FinalResult.cdt";

	CGroupRes groupRes;
	groupRes.ReadFromFile(strWriteFileName.c_str());
	hash.nBucketCnt = groupRes.GetBucketCnt();
	hash.nSigCnt = 2528;
	HASHRES HashResMap;
	for (ulong i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		HASHNODE node;
		node.m_sig = groupRes.GetGroups()[i].currSig;
		node.m_nDfaId = groupRes.GetGroups()[i].mergeDfaId;
		HashResMap[hash.BlockHash(node.m_sig, hash.nSigCnt)].push_back(node);
	}

	system("pause");
}