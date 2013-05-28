#include "stdafx.h"
#include "grouping.h"
#include "../compilernew/compilernew.h"
#include "../mergedfanew/MergeDfanew.h"

GROUPINGSC CGROUPS::CGROUPS()
{
	m_pGroups = new std::vector<GROUP>;
}

GROUPINGSC CGROUPS::CGROUPS(const CGROUPS& other)
{
	m_pGroups = new std::vector<GROUP>;
	*this = other;
}

GROUPINGSC const CGROUPS &CGROUPS::operator=(const CGROUPS &other)
{
	*m_pGroups = *other.m_pGroups;
	return *this;
}

GROUPINGSC CGROUPS::~CGROUPS()
{
	delete m_pGroups;
}

GROUPINGSC GROUP &CGROUPS::operator[](size_t index)
{
	return (*m_pGroups)[index];
}

GROUPINGSC const GROUP& CGROUPS::operator[](size_t index) const
{
	return (*m_pGroups)[index];
}

GROUPINGSC void CGROUPS::Reserve(size_t nCount)
{
	m_pGroups->reserve(nCount);
}

GROUPINGSC void CGROUPS::Resize(size_t nSize)
{
	m_pGroups->resize(nSize);
}

GROUPINGSC const size_t CGROUPS::Size() const
{
	return m_pGroups->size();
}

GROUPINGSC void CGROUPS::PushBack(const GROUP &oneGroup)
{
	m_pGroups->push_back(oneGroup);
}

GROUPINGSC GROUP &CGROUPS::Back()
{
	return m_pGroups->back();
}

struct COMP
{
	bool operator()(const GROUP &group1, const GROUP &group2)
	{
		return group1.vecSigs.size() < group2.vecSigs.size();
	}
};

GROUPINGSC void CGROUPS::Sort()
{
	std::sort(m_pGroups->begin(), m_pGroups->end(), COMP());
}

GROUPINGSC void CGROUPS::Clear()
{
	m_pGroups->clear();
}

GROUPINGSC CDfaTblNew &CGROUPRes::GetDfaTable()
{
	return m_dfaTbl;
}

GROUPINGSC CSidDfaIdsNew &CGROUPRes::GetSidDfaIds()
{
	return m_sidDfaIds;
}

GROUPINGSC CGROUPS &CGROUPRes::GetGroups()
{
	return m_groups;
}

GROUPINGSC const CDfaTblNew &CGROUPRes::GetDfaTable() const
{
	return m_dfaTbl;
}

GROUPINGSC const CSidDfaIdsNew &CGROUPRes::GetSidDfaIds() const
{
	return m_sidDfaIds;
}

GROUPINGSC const CGROUPS &CGROUPRes::GetGroups() const
{
	return m_groups;
}

template<typename _Ty>
void WriteNum(std::ofstream &fout, _Ty _num, size_t nBytes = sizeof(_Ty))
{
	fout.write((char*)&_num, nBytes);
}

GROUPINGSC size_t CGROUPRes::WriteToFile(LPCTSTR filename)
{
	std::ofstream fout(filename, std::ios::binary);
	if (!fout)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (size_t)-1;
	}
	//记录文件尺寸填写位置
	std::streamoff fileSizePos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);
	//规则数量
	WriteNum(fout, m_sidDfaIds.Size());
	//记录规则偏移填写位置
	std::streamoff ruleOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);
	//DFAs数量
	WriteNum(fout, m_dfaTbl.Size());
	//记录DFAs偏移填写位置
	std::streamoff dfaOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);
	//group数量
	WriteNum(fout, m_groups.Size());
	//记录groups偏移填写位置
	std::streamoff groupOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);
	//填写规则偏移
	std::streamoff endPos = fout.tellp();
	fout.seekp(ruleOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);
	//定位文件到末尾
	fout.seekp(endPos, std::ios_base::beg);
	//开始写规则表
	for (size_t i = 0; i < m_sidDfaIds.Size(); ++i)
	{
		COMPILEDRULENEW &ruleResult = m_sidDfaIds[i];
		WriteNum(fout, ruleResult.m_nSid, 4);
		WriteNum(fout, ruleResult.m_nResult, 4);
		WriteNum(fout, ruleResult.m_dfaIds.Size(), 4);
		for (size_t j = 0; j < ruleResult.m_dfaIds.Size(); ++j)
		{
			WriteNum(fout, ruleResult.m_dfaIds[j], 4);
		}
	}
	//填写DFAs偏移
	endPos = fout.tellp();
	fout.seekp(dfaOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);
	//定位文件到末尾
	fout.seekp(0, std::ios_base::end);
	//开始写DFAs
	BYTE dfaDetails[100000];
	for (size_t i = 0; i < m_dfaTbl.Size(); ++i)
	{
		size_t len = m_dfaTbl[i].Save(dfaDetails);
		WriteNum(fout, len);
		fout.write((char*)dfaDetails, len * sizeof(BYTE));
	}
	//填写分组偏移
	endPos = fout.tellp();
	fout.seekp(groupOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);
	//定位文件到末尾
	fout.seekp(0, std::ios_base::end);
	//开始写groups
	for (size_t i = 0; i < m_groups.Size(); ++i)
	{
		WriteNum(fout, m_groups[i].dfaId);
		WriteNum(fout, m_groups[i].vecSigs.size());
		for (size_t j = 0; j < m_groups[i].vecSigs.size(); ++j)
		{
			WriteNum(fout, m_groups[i].vecSigs[j]);
		}
	}
	//填写文件尺寸
	endPos = fout.tellp();
	fout.seekp(fileSizePos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);
	fout.seekp(0, std::ios_base::end);
	fout.close();
	fout.clear();

	return 0;
}

GROUPINGSC size_t CGROUPRes::ReadFromFile(LPCTSTR filename)
{
	std::ifstream fin(filename, std::ios::binary);
	if (!fin)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (size_t)-1;
	}
	//读取文件尺寸
	size_t fileSize;
	fin.read((char*)&fileSize, 4);
	//读取规则数量
	size_t ruleNum;
	fin.read((char*)&ruleNum, 4);
	//跳过规则偏移
	fin.seekg(4, std::ios_base::cur);
	//DFAs数量
	size_t dfaNum;
	fin.read((char*)&dfaNum, 4);
	//跳过DFAs偏移
	fin.seekg(4, std::ios_base::cur);
	//group数量
	size_t groupNum;
	fin.read((char*)&groupNum, 4);
	//跳过分组偏移
	fin.seekg(4, std::ios_base::cur);
	//开始读规则表
	m_sidDfaIds.Resize(ruleNum);
	size_t SidDfaNum;
	for (size_t i = 0; i < ruleNum; ++i)
	{
		COMPILEDRULENEW &ruleResult = m_sidDfaIds[i];
		fin.read((char*)&ruleResult.m_nSid, 4);
		fin.read((char*)&ruleResult.m_nResult, 4);
		fin.read((char*)&SidDfaNum, 4);
		ruleResult.m_dfaIds.Resize(SidDfaNum);
		for (size_t j = 0; j < SidDfaNum; ++j)
		{
			fin.read((char*)&(ruleResult.m_dfaIds[j]), 4);
		}
	}
	//开始读DFAs
	m_dfaTbl.Resize(dfaNum);
	BYTE dfaDetails[100000];
	for (size_t i = 0; i < dfaNum; ++i)
	{
		CDfanew &dfa = m_dfaTbl[i];
		size_t len;
		fin.read((char*)&len, 4);
		fin.read((char*)dfaDetails, len * sizeof(BYTE));
		dfa.Load(dfaDetails, len);
	}
	//读分组
	m_groups.Resize(groupNum);
	size_t nSigNum;
	for (size_t i = 0; i < groupNum; ++i)
	{
		GROUP &oneGroup = m_groups[i];
		fin.read((char*)&oneGroup.dfaId, 4);
		fin.read((char*)&nSigNum, 4);
		oneGroup.vecSigs.resize(nSigNum);
		for (size_t j = 0; j < nSigNum; ++j)
		{
			SIGNATURE &oneSig = oneGroup.vecSigs[j];
			fin.read((char*)&oneSig, sizeof(SIGNATURE));
		}
	}
	fin.close();
	fin.clear();

	return 0;
}


void BuildSidToDfaIdsMap(CResNew &res, std::map<size_t, std::vector<size_t>> &sidToDfaIdsMap)
{
	for (size_t i = 0; i < res.GetSidDfaIds().Size(); ++i)
	{
		std::vector<size_t> vecDfaIds;
		for (size_t j = 0; j < res.GetSidDfaIds()[i].m_dfaIds.Size(); ++j)
		{
			vecDfaIds.push_back(res.GetSidDfaIds()[i].m_dfaIds[j]);
		}
		sidToDfaIdsMap[res.GetSidDfaIds()[i].m_nSid] = vecDfaIds;
	}
}

void BuildDfaIdToSidMap(CResNew &res, std::map<size_t, size_t> &dfaIdToSidMap)
{
	for (size_t i = 0; i < res.GetSidDfaIds().Size(); ++i)
	{
		for (size_t j = 0; j < res.GetSidDfaIds()[i].m_dfaIds.Size(); ++j)
		{
			dfaIdToSidMap[res.GetSidDfaIds()[i].m_dfaIds[j]] = res.GetSidDfaIds()[i].m_nSid;
		}
	}
}

void BuildChainSet(CResNew &res, std::vector<RULECHAIN> &chainSet)
{
	res.GetRegexTbl();
	chainSet.reserve(10000);
	std::string strTmp;
	size_t nBeg, nEnd;
	//size_t nPos;
	for (size_t i = 0; i < res.GetRegexTbl().Size(); ++i)
	{
		chainSet.push_back(RULECHAIN());
		RULECHAIN &cr = chainSet.back();
		for (size_t j = 0; j < res.GetRegexTbl()[i].Size(); ++j)
		{
			strTmp = res.GetRegexTbl()[i][j].C_Str();
			nBeg = strTmp.find_first_of('/');
			nEnd = strTmp.find_last_of('/');
			cr.strChain += strTmp.substr(nBeg + 1, nEnd - nBeg - 1);
		}
		//while ((nPos = cr.strList.find("\\x", 0)) != std::string::npos)
		//{
		//	cr.strList.erase(nPos, 2);
		//}
		for (size_t j = 0; j < res.GetRegexTbl()[i].GetSigCnt(); ++j)
		{
			cr.sigs.push_back(res.GetRegexTbl()[i].GetSig(j));
		}
	}
	for (size_t i = 0; i < res.GetDfaTable().Size(); ++i)
	{
		chainSet[i].dfa = res.GetDfaTable()[i];
	}
}

bool ExtractTraits(const RULECHAIN &rulechain, CHAINTRAIT &trait)
{
	bool flag = false;
	size_t nLen = rulechain.strChain.length();
	if (nLen > CHAINTRAIT::TRAIT_LEN)
	{
		flag = true;
		CopyMemory(trait.szTrait, &rulechain.strChain[0], CHAINTRAIT::TRAIT_LEN);
	}
	return flag;
}

void BuildChainTraitMap(const std::vector<RULECHAIN> &chainSet, TRAITMAP &traitMap)
{
	//traitMap.rehash(CHAINTRAITHASH::HASH_SIZE);
	size_t idx = 0;
	for (std::vector<RULECHAIN>::const_iterator i = chainSet.begin(); i != chainSet.end(); ++i, ++idx)
	{
		CHAINTRAIT trait;
		if (ExtractTraits(*i, trait))
		{
			TRAITMAP::iterator irt = traitMap.find(trait);
			if (irt == traitMap.end())
			{
				std::vector<size_t> newVec;
				newVec.push_back(i - chainSet.begin());
				traitMap.insert(std::make_pair(trait, newVec));
			}
			else
			{
				irt->second.push_back(i - chainSet.begin());
			}
		}
	}
}

//计算两个字符串的最长公共前缀
std::string LongestCommonPrefix(const std::string &str1, const std::string &str2)
{
	size_t count = 0;
	size_t min = str1.length() < str2.length() ? str1.length() : str2.length();
	while (count < min && str1[count] == str2[count])
	{
		++count;
	}
	return str1.substr(0, count);
}

//初步分组
void PreGroup(const std::vector<RULECHAIN> &chainSet, const TRAITMAP &traitMap, std::vector<CHAINGROUP> &vecChainGroups, std::vector<size_t> &vecWaitForGroup)
{
	std::vector<size_t> vecDistributed;//记录分配出去的规则链
	for (TRAITMAP::const_iterator i = traitMap.begin(); i != traitMap.end(); ++i)
	{
		if (i->second.size() > 1)
		{
			vecChainGroups.push_back(CHAINGROUP());
			CHAINGROUP &oneGroup = vecChainGroups.back();
			oneGroup.chainIds = i->second;
			vecDistributed.insert(vecDistributed.begin(), i->second.begin(), i->second.end());
		}
	}

	//计算每个分组的最长公共前缀
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end(); ++i)
	{
		i->comStr = chainSet[i->chainIds[0]].strChain;
		for (std::vector<size_t>::const_iterator j = i->chainIds.begin() + 1; j != i->chainIds.end(); ++j)
		{
			i->comStr = LongestCommonPrefix(i->comStr, chainSet[*j].strChain);
		}
	}

	//记录还未分配出去的规则链
	for (size_t i = 0; i < chainSet.size(); ++i)
	{
		if (std::find(vecDistributed.begin(), vecDistributed.end(), i) == vecDistributed.end())
		{
			vecWaitForGroup.push_back(i);
		}
	}
}

//进一步分组
void FartherGroup(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups, std::vector<size_t> &vecWaitForGroup)
{
	size_t max;
	size_t tmp;
	size_t index;
	bool flag;
	for (std::vector<size_t>::const_iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end(); ++i)
	{
		max = 0;
		index = 0;
		flag = false;
		for (size_t j = 0; j < vecChainGroups.size(); ++j)
		{
			tmp = LongestCommonPrefix(vecChainGroups[j].comStr, chainSet[*i].strChain).length();
			if (tmp == chainSet[*i].strChain.length() && tmp == vecChainGroups[j].comStr.length())
			{
				flag = true;
				max = tmp;
				index = j;
				break;
			}
			if (max < tmp)
			{
				max = tmp;
				index = j;
			}
		}
		if (max >= THRESHOLD || flag)
		{
			vecChainGroups[index].chainIds.push_back(*i);
			vecChainGroups[index].comStr = vecChainGroups[index].comStr.substr(0, max);
		}
		else
		{
			vecChainGroups.push_back(CHAINGROUP());
			CHAINGROUP &group = vecChainGroups.back();
			group.chainIds.push_back(*i);
			group.comStr = chainSet[*i].strChain;
		}
	}

	vecWaitForGroup.clear();
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end();)
	{
		if (i->chainIds.size() == 1)
		{
			vecWaitForGroup.push_back(i->chainIds[0]);
			i = vecChainGroups.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void SortChainId(std::vector<CHAINGROUP> &vecChainGroups, const std::vector<RULECHAIN> &chainSet)
{
	struct COMP
	{
		const std::vector<RULECHAIN> *pchainSet;
		COMP(const std::vector<RULECHAIN> *p) : pchainSet(p) {}
		bool operator()(const size_t &id1, const size_t &id2)
		{
			return (*pchainSet)[id1].dfa.Size() < (*pchainSet)[id2].dfa.Size();
			//return (*pchainSet)[id1].sigs.size() > (*pchainSet)[id2].sigs.size();
		}
	};
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end(); ++i)
	{
		std::sort(i->chainIds.begin(), i->chainIds.end(), COMP(&chainSet));
	}
}

void ExtractExplosion(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups, std::vector<size_t> &vecWaitForGroup, std::vector<size_t> &vecExplosion)
{
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end();)
	{
		for (std::vector<size_t>::iterator j = i->chainIds.begin(); j != i->chainIds.end();)
		{
			if (chainSet[*j].dfa.Size() == 0)
			{
				vecExplosion.push_back(*j);
				j = i->chainIds.erase(j);
			}
			else
			{
				break;
			}
		}
		if (i->chainIds.size() == 1 || i->chainIds.size() == 0)
		{
			if (i->chainIds.size() == 1)
			{
				vecWaitForGroup.push_back(i->chainIds[0]);
			}
			i = vecChainGroups.erase(i);
		}
		else
		{
			++i;
		}
	}

	for (std::vector<size_t>::iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end();)
	{
		if (chainSet[*i].dfa.Size() == 0)
		{
			vecExplosion.push_back(*i);
			i = vecWaitForGroup.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void SplitByComSig(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups, std::vector<size_t> &vecWaitForGroup)
{
	std::vector<CHAINGROUP> vecOldGroup(vecChainGroups.begin(), vecChainGroups.end());
	vecChainGroups.clear();
	for (std::vector<CHAINGROUP>::iterator i = vecOldGroup.begin(); i != vecOldGroup.end(); ++i)
	{
		std::vector<size_t> &chainIds = i->chainIds;
		while (!chainIds.empty())
		{
			std::vector<size_t> haveComSigIds(chainIds.begin(), chainIds.end());
			size_t count = 0;
			std::vector<SIGNATURE> vecComSigs;
			while (!haveComSigIds.empty() && count < NUMOFCOMSIGS)
			{
				std::map<SIGNATURE, std::vector<size_t>> SigToIdsMap;
				for (std::vector<size_t>::iterator j = haveComSigIds.begin(); j != haveComSigIds.end(); ++j)
				{
					for (std::vector<SIGNATURE>::const_iterator k = chainSet[*j].sigs.begin(); k != chainSet[*j].sigs.end(); ++k)
					{
						SigToIdsMap[*k].push_back(*j);
					}
				}
				size_t max = 0;
				std::map<SIGNATURE, std::vector<size_t>>::iterator iter;
				for (std::map<SIGNATURE, std::vector<size_t>>::iterator j = SigToIdsMap.begin(); j != SigToIdsMap.end(); ++j)
				{
					if (max < j->second.size() && std::find(vecComSigs.begin(), vecComSigs.end(), j->first) == vecComSigs.end())
					{
						max = j->second.size();
						iter = j;
					}
				}
				if (max <= 1)
				{
					break;
				}
				vecComSigs.push_back(iter->first);
				haveComSigIds = iter->second;
				++count;
			}
			if (count >= NUMOFCOMSIGS)
			{
				vecChainGroups.push_back(CHAINGROUP());
				CHAINGROUP &oneGroup = vecChainGroups.back();
				oneGroup.chainIds.insert(oneGroup.chainIds.begin(), haveComSigIds.begin(), haveComSigIds.end());
				for (std::vector<size_t>::iterator j = chainIds.begin(); j != chainIds.end();)
				{
					if (std::find(haveComSigIds.begin(), haveComSigIds.end(), *j) != haveComSigIds.end())
					{
						j = chainIds.erase(j);
					}
					else
					{
						++j;
					}
				}
			}
			else
			{
				vecWaitForGroup.insert(vecWaitForGroup.begin(), chainIds.begin(), chainIds.end());
				chainIds.clear();
			}
		}
	}

	SortChainId(vecChainGroups, chainSet);
}

void fdisplay(CDfanew &newdfa, const char* fileName)
{
	std::ofstream fout(fileName);
	fout << (size_t)newdfa.GetStartId() << std::endl;
	for(size_t i = 0; i != newdfa.Size(); ++i)
	{
		std::map<STATEID, size_t> rowStateCnt;
		for(size_t j = 0; j != newdfa.GetGroupCount(); ++j)
		{
			rowStateCnt[newdfa[i][j]]++;
		}
		STATEID maxId = 0;
		for (std::map<STATEID, size_t>::iterator j = rowStateCnt.begin(); j != rowStateCnt.end(); ++j)
		{
			if (j->second > rowStateCnt[maxId])
			{
				maxId = j->first;
			}
		}
		fout << "s"<< i << ", maxId: " << (size_t)maxId << ", ";
		for(size_t j = 0; j != newdfa.GetGroupCount(); ++j)
		{
			if (newdfa[i][j] != maxId)
			{
				fout << "<" << j << "," <<  (size_t)newdfa[i][j] << "> " ;
			}
		}
		fout << std::endl;
	}
	fout.close();
}


void MergeMore(std::vector<CHAINGROUP> &vecChainGroups, CResNew &res, std::vector<size_t> &vecWaitForGroup)
{
	bool mergeFlag;
	std::vector<CDfanew> vecDfas(2);
	CDfanew MergeDfa;
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end();)
	{
		std::cout << "More" << std::endl;
		std::cout << "NO: " << i - vecChainGroups.begin() << std::endl;
		std::cout << "Total: " << vecChainGroups.size() << std::endl << std::endl;
		std::vector<CDfanew> vecDfas(2);
		vecDfas[0] = res.GetDfaTable()[i->chainIds[0]];
		outPutDfa(vecDfas[0], "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\dfa_7371_2.txt");
		vecDfas[1] = res.GetDfaTable()[i->chainIds[1]];
		if (!NOrMerge(vecDfas, MergeDfa))
		{
			for (size_t j = 0; j < i->chainIds.size(); ++j)
			{
				vecWaitForGroup.push_back(i->chainIds[j]);
			}
			i = vecChainGroups.erase(i);
			continue;
		}
		mergeFlag = true;
		vecDfas[0] = MergeDfa;
		for (size_t j = 2; j < i->chainIds.size(); ++j)
		{
			vecDfas[1] = res.GetDfaTable()[i->chainIds[j]];
			//outPutDfa(vecDfas[1], "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\Dfa_1.txt");//用于测试
			if (!NOrMerge(vecDfas, MergeDfa))
			{
				mergeFlag = false;
				res.GetDfaTable().PushBack(vecDfas[0]);
				if (i->chainIds.size() - j == 1)
				{
					vecWaitForGroup.push_back(i->chainIds[j]);
				}
				else
				{
					vecChainGroups.resize(vecChainGroups.size() + 1);
					CHAINGROUP &oneGroup = vecChainGroups.back();
					oneGroup.chainIds.insert(oneGroup.chainIds.begin(), i->chainIds.begin() + j, i->chainIds.end());
				}
				i->chainIds.erase(i->chainIds.begin() + j, i->chainIds.end());
				i->mergeDfaId = res.GetDfaTable().Size() - 1;
				break;
			}
			else
			{
				vecDfas[0] = MergeDfa;
				//outPutDfa(MergeDfa, "F:\\cppProject\\huawei\\PreciseMatch\\testMerg\\mergeDfa_2.txt");
			}
		}
		if (mergeFlag)
		{
			res.GetDfaTable().PushBack(vecDfas[0]);
			i->mergeDfaId = res.GetDfaTable().Size() - 1;
		}
		++i;
	}
}

void ExtractSigs(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups)
{
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end(); ++i)
	{
		std::map<SIGNATURE, size_t> SigMap;
		for (std::vector<size_t>::iterator j = i->chainIds.begin(); j != i->chainIds.end(); ++j)
		{
			for (std::vector<SIGNATURE>::const_iterator k = chainSet[*j].sigs.begin(); k != chainSet[*j].sigs.end(); ++k)
			{
				++SigMap[*k];
			};
		}

		i->comSigs.clear();
		for (std::map<SIGNATURE, size_t>::iterator j = SigMap.begin(); j != SigMap.end(); ++j)
		{
			if (j->second == i->chainIds.size())
			{
				i->comSigs.push_back(j->first);
			}
		}
	}
}

void PutInByRule(std::map<size_t, size_t> &dfaIdToSidMap, std::map<size_t, std::vector<size_t>> &sidToDfaIdsMap, std::vector<CHAINGROUP> &vecChainGroups, CResNew &res, std::vector<size_t> &vecWaitForGroup)
{
	std::map<size_t, size_t> dfaIdToGroupId;
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end(); ++i)
	{
		for (std::vector<size_t>::iterator j = i->chainIds.begin(); j != i->chainIds.end(); ++j)
		{
			dfaIdToGroupId[*j] = i - vecChainGroups.begin();
		}
	}
	std::vector<size_t> vecWaitForGroupCopy(vecWaitForGroup.begin(), vecWaitForGroup.end());
	vecWaitForGroup.clear();
	for (std::vector<size_t>::iterator i = vecWaitForGroupCopy.begin(); i != vecWaitForGroupCopy.end(); ++i)
	{
		std::cout << "PutInByRule" << std::endl;
		std::cout << "One" << std::endl;
		std::cout << "NO: " << i - vecWaitForGroupCopy.begin() << std::endl;
		std::cout << "Total: " << vecWaitForGroupCopy.size() << std::endl << std::endl;
		vecWaitForGroup.push_back(*i);
		for (std::vector<size_t>::iterator j = sidToDfaIdsMap[dfaIdToSidMap[*i]].begin(); j != sidToDfaIdsMap[dfaIdToSidMap[*i]].end(); ++j)
		{
			std::map<size_t, size_t>::iterator k = dfaIdToGroupId.find(*j);
			if (k != dfaIdToGroupId.end())
			{
				std::vector<CDfanew> vecDfas;
				vecDfas.push_back(res.GetDfaTable()[vecChainGroups[k->second].mergeDfaId]);
				vecDfas.push_back(res.GetDfaTable()[*i]);
				CDfanew MergeDfa;
				if (NOrMerge(vecDfas, MergeDfa))
				{
					vecChainGroups[k->second].chainIds.push_back(*i);
					res.GetDfaTable().PushBack(MergeDfa);
					vecChainGroups[k->second].mergeDfaId = res.GetDfaTable().Size() - 1;
					vecWaitForGroup.pop_back();
					break;
				}
			}
		}
	}
}

GROUPINGSC void UpdateComSigs(std::vector<SIGNATURE> &oldSigs, const std::vector<SIGNATURE> &newSigs)
{
	std::map<SIGNATURE, size_t> sigToCountMap;
	for (std::vector<SIGNATURE>::iterator i = oldSigs.begin(); i != oldSigs.end(); ++i)
	{
		++sigToCountMap[*i];
	}
	oldSigs.clear();
	for (std::vector<SIGNATURE>::const_iterator i = newSigs.begin(); i != newSigs.end(); ++i)
	{
		++sigToCountMap[*i];
	}
	for (std::map<SIGNATURE, size_t>::iterator i = sigToCountMap.begin(); i != sigToCountMap.end(); ++i)
	{
		if (i->second == 2)
		{
			oldSigs.push_back(i->first);
		}
	}
}

void PutInBySig(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups, CResNew &res, std::vector<size_t> &vecWaitForGroup)
{
	std::vector<size_t> vecWaitForGroupCopy(vecWaitForGroup.begin(), vecWaitForGroup.end());
	vecWaitForGroup.clear();
	for (std::vector<size_t>::iterator i = vecWaitForGroupCopy.begin(); i != vecWaitForGroupCopy.end(); ++i)
	{
		std::cout << "PutInBySig" << std::endl;
		std::cout << "One" << std::endl;
		std::cout << "NO: " << i - vecWaitForGroupCopy.begin() << std::endl;
		std::cout << "Total: " << vecWaitForGroupCopy.size() << std::endl << std::endl;
		vecWaitForGroup.push_back(*i);
		for (std::vector<CHAINGROUP>::iterator j = vecChainGroups.begin(); j != vecChainGroups.end(); ++j)
		{
			std::vector<SIGNATURE> vecSigs(chainSet[*i].sigs.begin(), chainSet[*i].sigs.end());
			size_t count = 0;
			while (count < NUMOFCOMSIGS)
			{
				std::vector<SIGNATURE>::iterator k = std::find_first_of(vecSigs.begin(), vecSigs.end(), j->comSigs.begin(), j->comSigs.end());
				if (k == vecSigs.end())
				{
					break;
				}
				vecSigs.erase(k);
				++count;
			}
			if (count >= NUMOFCOMSIGS)
			{
				std::vector<CDfanew> vecDfas;
				vecDfas.push_back(res.GetDfaTable()[j->mergeDfaId]);
				vecDfas.push_back(res.GetDfaTable()[*i]);
				CDfanew MergeDfa;
				if (NOrMerge(vecDfas, MergeDfa))
				{
					j->chainIds.push_back(*i);
					res.GetDfaTable().PushBack(MergeDfa);
					j->mergeDfaId = res.GetDfaTable().Size() - 1;
					vecWaitForGroup.pop_back();
					UpdateComSigs(j->comSigs, chainSet[*i].sigs);
					break;
				}
			}
		}
	}
}

void BuildGroupBySig(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups, CResNew &res, std::vector<size_t> &vecWaitForGroup)
{
	std::vector<CHAINGROUP> newGroups;
	std::vector<size_t> visited(vecWaitForGroup.size(), 0);
	size_t idx = 0;
	for (std::vector<size_t>::iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end(); ++i, ++idx)
	{
		if (visited[idx])
		{
			continue;
		}
		visited[idx] = 1;
		newGroups.push_back(CHAINGROUP());
		CHAINGROUP &oneGroup = newGroups.back();
		oneGroup.chainIds.push_back(*i);
		oneGroup.comSigs = chainSet[*i].sigs;
		for (std::vector<size_t>::iterator j = i + 1; j != vecWaitForGroup.end(); ++j)
		{
			if (visited[j - vecWaitForGroup.begin()])
			{
				continue;
			}
			std::vector<SIGNATURE> vecSigs(oneGroup.comSigs.begin(), oneGroup.comSigs.end());
			size_t count = 0;
			while (count < NUMOFCOMSIGS)
			{
				std::vector<SIGNATURE>::iterator k = std::find_first_of(vecSigs.begin(), vecSigs.end(), chainSet[*j].sigs.begin(), chainSet[*j].sigs.end());
				if (k == vecSigs.end())
				{
					break;
				}
				vecSigs.erase(k);
				++count;
			}
			if (count >= NUMOFCOMSIGS)
			{
				visited[j - vecWaitForGroup.begin()] = 1;
				oneGroup.chainIds.push_back(*j);
				UpdateComSigs(oneGroup.comSigs, chainSet[*j].sigs);
			}
		}
	}

	vecWaitForGroup.clear();
	for (std::vector<CHAINGROUP>::iterator i = newGroups.begin(); i != newGroups.end(); )
	{
		if (i->chainIds.size() == 1)
		{
			vecWaitForGroup.push_back(i->chainIds[0]);
			i = newGroups.erase(i);
		}
		else
		{
			++i;
		}
	}

	std::map<SIGNATURE, std::vector<size_t>> SigToIdsMap;
	for (std::vector<size_t>::iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end(); )
	{
		if (chainSet[*i].sigs.size() == 1)
		{
			SigToIdsMap[chainSet[*i].sigs[0]].push_back(*i);
			vecWaitForGroup.erase(i);
		}
		else
		{
			++i;
		}
	}
	for (std::map<SIGNATURE, std::vector<size_t>>::iterator i = SigToIdsMap.begin(); i != SigToIdsMap.end(); ++i)
	{
		if (i->second.size() > 1)
		{
			newGroups.push_back(CHAINGROUP());
			CHAINGROUP &oneGroup = newGroups.back();
			oneGroup.chainIds = i->second;
			oneGroup.comSigs.push_back(i->first);
		}
		else if (i->second.size() == 1)
		{
			vecWaitForGroup.push_back(i->second[0]);
		}
		else
		{
			continue;
		}
	}

	SortChainId(newGroups, chainSet);
	MergeMore(newGroups, res, vecWaitForGroup);
	ExtractSigs(chainSet, newGroups);
	vecChainGroups.insert(vecChainGroups.end(), newGroups.begin(), newGroups.end());
	for (std::vector<size_t>::iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end(); ++i)
	{
		vecChainGroups.push_back(CHAINGROUP());
		CHAINGROUP &oneGroup = vecChainGroups.back();
		oneGroup.chainIds.push_back(*i);
		oneGroup.comSigs = chainSet[*i].sigs;
		oneGroup.mergeDfaId = *i;
	}
	vecWaitForGroup.clear();
}

void ClearUselessDfa(const std::vector<CHAINGROUP> &vecChainGroups, CResNew &res)
{
	std::vector<size_t> occurred(res.GetDfaTable().Size(), 0);
	for (std::vector<CHAINGROUP>::const_iterator i = vecChainGroups.begin(); i != vecChainGroups.end(); ++i)
	{
		occurred[i->mergeDfaId] = 1;
	}
	for (std::vector<size_t>::iterator i = occurred.begin(); i != occurred.end(); ++i)
	{
		if (*i == 0)
		{
			res.GetDfaTable()[i - occurred.begin()].Clear();
		}
	}
}

void BuildGroupRes(const std::vector<CHAINGROUP> &vecChainGroups, const CResNew &res, CGROUPRes &groupRes)
{
	groupRes.GetGroups().Resize(vecChainGroups.size());
	groupRes.GetDfaTable().Resize(vecChainGroups.size());
	size_t idx = 0;
	for (std::vector<CHAINGROUP>::const_iterator i = vecChainGroups.begin(); i != vecChainGroups.end(); ++i, ++idx)
	{
		groupRes.GetGroups()[idx].dfaId = idx;
		groupRes.GetGroups()[idx].vecSigs = i->comSigs;
		groupRes.GetDfaTable()[idx] = res.GetDfaTable()[i->mergeDfaId];
	}
	groupRes.GetSidDfaIds() = res.GetSidDfaIds();
}

GROUPINGSC void grouping(CResNew &res, CGROUPRes &groupRes)
{
	//Load all rules chains from file
	CTimer t1, tAll;

	//std::cout << "Loading results from file..." << std::endl;
	//CResNew res;
	//res.ReadFromFile(filename);
	//std::cout << "Completed in " << t1.Reset() << " Sec. chains: " << res.GetRegexTbl().Size() << std::endl << std::endl;

	std::cout << "Build sidToDfaIdsMap from result..." << std::endl;
	std::map<size_t, std::vector<size_t>> sidToDfaIdsMap;
	BuildSidToDfaIdsMap(res, sidToDfaIdsMap);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	std::cout << "Build dfaIdToSidMap from result..." << std::endl;
	std::map<size_t, size_t> dfaIdToSidMap;
	BuildDfaIdToSidMap(res, dfaIdToSidMap);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	std::cout << "Build chainSet from result..." << std::endl;
	std::vector<RULECHAIN> chainSet;
	BuildChainSet(res, chainSet);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	// Build chain trait map
	std::cout << "Building a trait map for a trait to a vector of chain index numbers..." << std::endl;
	TRAITMAP traitMap;
	BuildChainTraitMap(chainSet, traitMap);
	std::cout << "Completed in " << t1.Reset() << " Sec. Traits: " << traitMap.size() << std::endl << std::endl;

	//Start preliminary grouping...
	std::cout << "Start preliminary grouping..." << std::endl;
	std::vector<CHAINGROUP> vecChainGroups;
	std::vector<size_t> vecWaitForGroup;
	PreGroup(chainSet, traitMap, vecChainGroups, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Start farther grouping...
	std::cout << "Start farther grouping..." << std::endl;
	FartherGroup(chainSet, vecChainGroups, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Sort chainid in each group by dfa size...
	std::cout << "Sort chainid in each group by dfa size..." << std::endl;
	SortChainId(vecChainGroups, chainSet);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Extract explosion chains...
	std::cout << "Extract explosion chains..." << std::endl;
	std::vector<size_t> vecExplosion;
	ExtractExplosion(chainSet, vecChainGroups, vecWaitForGroup, vecExplosion);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Split by common Sig...
	std::cout << "Split by common Sig..." << std::endl;
	SplitByComSig(chainSet, vecChainGroups, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Merge dfa in a group...
	std::cout << "Merge dfa in a group..." << std::endl;
	MergeMore(vecChainGroups, res, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Extract common Signatures in each group...
	std::cout << "Extract common Signatures in each group..." << std::endl;
	ExtractSigs(chainSet, vecChainGroups);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Put chain in group which belong to a same rule...
	std::cout << "Put chain in group which belong to a same rule..." << std::endl;
	PutInByRule(dfaIdToSidMap, sidToDfaIdsMap, vecChainGroups, res, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Put chain in group which have the same signature...
	std::cout << "Put chain in group which have the same signature..." << std::endl;
	PutInBySig(chainSet, vecChainGroups, res, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//New group which have the same signature...
	std::cout << "New group which have the same signature..." << std::endl;
	BuildGroupBySig(chainSet, vecChainGroups, res, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Clear useless dfa...
	std::cout << "Clear useless dfa..." << std::endl;
	ClearUselessDfa(vecChainGroups, res);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Build group result...
	std::cout << "Build group result..." << std::endl;
	BuildGroupRes(vecChainGroups, res, groupRes);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	std::cout << vecChainGroups.size() << std::endl;

	std::cout << "Total time: " << tAll.Reset() << " Sec." << std::endl;
}