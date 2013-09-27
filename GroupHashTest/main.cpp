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

	//ͳ�ƽ������ļ�
	std::ofstream fos("errorRules.txt");
	if (fos.is_open())
	{
		fos << "���������� " << sidSize << std::endl;
		int errorRulesCnt = sidDfaIds.Size() - vecCnt[7];
		fos << "���ܴ���Ĺ������� " << errorRulesCnt 
			<< "; ռ����ٷֱȣ� " << ((double)errorRulesCnt / (double)sidSize) << std::endl;
		fos << "��������Ϊ�յĹ������� " 
			<< vecCnt[0] << "; ռ����ٷֱȣ� " << ((double)vecCnt[0] / (double)sidSize) << std::endl;
		fos << "����״̬���ƵĹ������� " 
			<< vecCnt[1] << "; ռ����ٷֱȣ� " << ((double)vecCnt[1] / (double)sidSize) << std::endl;
		fos << "����byte_test��byte_jump�Ĺ������� " 
			<< vecCnt[2] << "; ռ����ٷֱȣ� " << ((double)vecCnt[2] / (double)sidSize) << std::endl;
		fos << "û�������ַ����Ĺ������� " 
			<< vecCnt[3] << "; ռ����ٷֱȣ� " << ((double)vecCnt[3] / (double)sidSize) << std::endl;
		fos << "����'!'���߼��ǣ���ʶ�Ĺ������� " 
			<< vecCnt[4] << "; ռ����ٷֱȣ� " << ((double)vecCnt[4] / (double)sidSize) << std::endl;
		fos << "snort����ѡ�����Ĺ������� " 
			<< vecCnt[5] << "; ռ����ٷֱȣ� " << ((double)vecCnt[5] / (double)sidSize) << std::endl;
		fos << "pcreѡ�����Ĺ������� " 
			<< vecCnt[6] << "; ռ����ٷֱȣ� " << ((double)vecCnt[6] / (double)sidSize) << std::endl;
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
	////ͳ�ƽ������ļ�
	//std::ofstream fos("chain.txt");
	//if (fos.is_open())
	//{
	//	fos << "���е������� " << dfaTblSet.Size() << std::endl;
	//	fos << "DFA״̬������ " << stateSum << std::endl;
	//	fos << "DFAǨ�Ʊ������� " << edgeSum << std::endl;
	//	fos << "���й���Signature������ " << sigSet.size() << std::endl;
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
	//std::cout << "������ֻ����һ�����ĸ����� " << oneChain << std::endl;
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
	////�������ļ�

	//CCompileResults result;
	////����������ļ�
	//if (0 == result.ReadFromFile(strFile.c_str()))
	//{
	//	//ͳ�ƴ������
	//	//StatisticErrorRules(result);

	//	//ͳ���ܱ������
	//	StatisticChain(result);

	//	//ͳ��signature���ַ��ķֲ�
	//	//StatisticSigchar(result);

	//	CTimer t;
	//	CGroupRes groupRes;
	//	//����
	//	Grouping(result, groupRes);
	//	//std::cout << "���������ʱ�� " << t.Reset() << std::endl;

	//	HASHRES HashResMap;
	//	//��ϣ
	//	HashMapping(groupRes, HashResMap);
	//	//std::cout << "��ϣ������ʱ�� " << t.Reset() << std::endl;

	//	//ͳ��ֻ����һ�����ķ������
	//	StatisticOneChain(groupRes);

	//	//�洢���ս�����ļ�
	//	std::string strWriteFileName(pArgs[1]);
	//	std::string::iterator iPos = std::find(strWriteFileName.begin(), strWriteFileName.end(), '\\');
	//	strWriteFileName.erase(iPos + 1, strWriteFileName.end());
	//	strWriteFileName += "FinalResult.cdt";

	//	//д���ս���ļ�
	//	groupRes.WriteToFile(strWriteFileName.c_str());

	//	//�ܷ�����
	//	std::cout << groupRes.GetGroups().Size() << std::endl;
	//
	//	//ʹ�õ��Ĺ�ϣ����
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