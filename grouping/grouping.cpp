#include "stdafx.h"
#include "grouping.h"
#include "../compilernew/compilernew.h"
#include "../mergedfanew/MergeDfanew.h"

GROUPINGSC CSIGNATURES::CSIGNATURES()
{
	m_pSigs = new std::vector<SIGNATURE>;
}
GROUPINGSC CSIGNATURES::CSIGNATURES(const CSIGNATURES& other)
{
	m_pSigs = new std::vector<SIGNATURE>;
	*this = other;
}
GROUPINGSC const CSIGNATURES &CSIGNATURES::operator=(const CSIGNATURES &other)
{
	*m_pSigs = *other.m_pSigs;
	return *this;
}

GROUPINGSC CSIGNATURES::~CSIGNATURES()
{
	delete m_pSigs;
}

GROUPINGSC const size_t CSIGNATURES::Size() const
{
	return m_pSigs->size();
}

GROUPINGSC void CSIGNATURES::Resize(size_t nSize)
{
	m_pSigs->resize(nSize);
}

GROUPINGSC void CSIGNATURES::PushBack(SIGNATURE Sig)
{
	m_pSigs->push_back(Sig);
}

GROUPINGSC SIGNATURE &CSIGNATURES::operator[](size_t nIdx)
{
	return (*m_pSigs)[nIdx];
}

GROUPINGSC const SIGNATURE &CSIGNATURES::operator[](size_t nIdx) const
{
	return (*m_pSigs)[nIdx];
}

GROUPINGSC void CSIGNATURES::Clear()
{
	m_pSigs->clear();
}

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

GROUPINGSC const size_t CGROUPS::Size() const
{
	return m_pGroups->size();
}

GROUPINGSC void CGROUPS::Resize(size_t nSize)
{
	m_pGroups->resize(nSize);
}

GROUPINGSC GROUP &CGROUPS::operator[](size_t nIdx)
{
	return (*m_pGroups)[nIdx];
}

GROUPINGSC const GROUP &CGROUPS::operator[](size_t nIdx) const
{
	return (*m_pGroups)[nIdx];
}

GROUPINGSC void CGROUPS::PushBack(GROUP oneGroup)
{
	m_pGroups->push_back(oneGroup);
}

GROUPINGSC GROUP& CGROUPS::Back()
{
	return m_pGroups->back();
}

GROUPINGSC void CGROUPS::Clear()
{
	m_pGroups->clear();
}

GROUPINGSC void CGROUPS::Erase(size_t nIdx)
{
	m_pGroups->erase(m_pGroups->begin() + nIdx);
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
		WriteNum(fout, m_groups[i].DfaIds.Size());
		for (size_t j = 0; j < m_groups[i].DfaIds.Size(); ++j)
		{
			WriteNum(fout, m_groups[i].DfaIds[j]);
		}
		WriteNum(fout, m_groups[i].ComSigs.Size());
		for (size_t j = 0; j < m_groups[i].ComSigs.Size(); ++j)
		{
			WriteNum(fout, m_groups[i].ComSigs[j]);
		}
		WriteNum(fout, m_groups[i].mergeDfaId);
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
		CDfaNew &dfa = m_dfaTbl[i];
		size_t len;
		fin.read((char*)&len, 4);
		fin.read((char*)dfaDetails, len * sizeof(BYTE));
		dfa.Load(dfaDetails, len);
	}
	//读分组
	m_groups.Resize(groupNum);
	size_t nDfaIdNum;
	size_t nSigNum;
	for (size_t i = 0; i < groupNum; ++i)
	{
		GROUP &oneGroup = m_groups[i];
		fin.read((char*)&nDfaIdNum, 4);
		oneGroup.DfaIds.Resize(nDfaIdNum);
		for (size_t j = 0; j < nDfaIdNum; ++j)
		{
			size_t &nDfaId = oneGroup.DfaIds[j];
			fin.read((char*)&nDfaId, 4);
		}
		fin.read((char*)&nSigNum, 4);
		oneGroup.ComSigs.Resize(nSigNum);
		for (size_t j = 0; j < nSigNum; ++j)
		{
			SIGNATURE &oneSig = oneGroup.ComSigs[j];
			fin.read((char*)&oneSig, sizeof(SIGNATURE));
		}
		fin.read((char*)&oneGroup.mergeDfaId, 4);
	}
	fin.close();
	fin.clear();

	return 0;
}

void ExtractDfaInfo(CResNew &res, std::vector<DFAINFO> &vecDfaInfo, std::vector<size_t> &vecWaitForGroup, std::vector<size_t> &vecExplosion)
{
	std::size_t nSize = res.GetRegexTbl().Size();
	vecDfaInfo.resize(nSize);
	for (size_t i = 0; i < nSize; ++i)
	{
		if (res.GetDfaTable()[i].Size() == 0)
		{
			vecExplosion.push_back(i);
		}
		else
		{
			vecWaitForGroup.push_back(i);
		}
		for (size_t j = 0; j < res.GetRegexTbl()[i].GetSigCnt(); ++j)
		{
			vecDfaInfo[i].Sigs.push_back(res.GetRegexTbl()[i].GetSig(j));
		}
	}
}

void GroupOnlyOneSig(std::vector<DFAINFO> &vecDfaInfo, std::vector<size_t> &vecWaitForGroup, CGROUPS &groups)
{
	std::map<SIGNATURE, CVectorNumber> sigToIdsMap;
	for (std::vector<size_t>::iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end();)
	{
		if (vecDfaInfo[*i].Sigs.size() == 1)
		{
			sigToIdsMap[vecDfaInfo[*i].Sigs[0]].PushBack(*i);
			i = vecWaitForGroup.erase(i);
		}
		else
		{
			++i;
		}
	}

	groups.Resize(sigToIdsMap.size());
	std::size_t idx = 0;
	for (std::map<SIGNATURE, CVectorNumber>::iterator i = sigToIdsMap.begin(); i != sigToIdsMap.end(); ++i, ++idx)
	{
		groups[idx].ComSigs.PushBack(i->first);
		groups[idx].DfaIds = i->second;
		groups[idx].mergeDfaId = -1;
	}
}

void Merge(CResNew &res, CGROUPS &groups)
{
	for (size_t i = 0; i < groups.Size(); ++i)
	{
		std::cout << "Merge " << std::endl;
		std::cout << "NO: " << i << std::endl;
		std::cout << "Total: " << groups.Size() << std::endl << std::endl;
		groups[i].mergeDfaId = groups[i].DfaIds[0];
		if (groups[i].DfaIds.Size() == 1)
		{
			continue;
		}
		std::vector<CDfaNew> vecDfas(2);
		CDfaNew MergeDfa;
		MergeDfa.SetId(res.GetDfaTable().Size());
		vecDfas[0] = res.GetDfaTable()[groups[i].DfaIds[0]];
		bool mergeFlag = true;
		for (size_t j = 1; j < groups[i].DfaIds.Size(); ++j)
		{
			vecDfas[1] = res.GetDfaTable()[groups[i].DfaIds[j]];
			//for (size_t k = 0; k <= j; ++k)
			//{
			//	std::cout << groups[i].DfaIds[k] << " ";
			//}
			//std::cout << std::endl;
			//outPutDfa(vecDfas[1], "..//..//output//dfa1.txt");
			//outPutDfa(vecDfas[0], "..//..//output//dfa2.txt");
			if (!NOrMerge(vecDfas, MergeDfa))
			{
				mergeFlag = false;
				if (j != 1)
				{
					res.GetDfaTable().PushBack(vecDfas[0]);
					groups[i].mergeDfaId = res.GetDfaTable().Size() - 1;
				}
				groups.PushBack(GROUP());
				for (size_t k = j; k < groups[i].DfaIds.Size(); ++k)
				{
					groups.Back().DfaIds.PushBack(groups[i].DfaIds[k]);
				}
				groups.Back().mergeDfaId = -1;
				groups.Back().ComSigs = groups[i].ComSigs;
				CVectorNumber tmp(groups[i].DfaIds);
				groups[i].DfaIds.Clear();
				for (size_t k = 0; k < j; ++k)
				{
					groups[i].DfaIds.PushBack(tmp[k]);
				}
				break;
			}
			else
			{
				vecDfas[0] = MergeDfa;
			}
		}
		if (mergeFlag)
		{
			res.GetDfaTable().PushBack(MergeDfa);
			groups[i].mergeDfaId = res.GetDfaTable().Size() - 1;
		}
	}
}

void PutInBySig(CResNew &res, CGROUPS &groups, std::vector<size_t> &vecWaitForGroup, std::vector<DFAINFO> &vecDfaInfo)
{
	std::map<SIGNATURE, std::vector<size_t>> sigToGroupsMap;
	size_t idx = 0;
	for (size_t i = 0; i < groups.Size(); ++i, ++idx)
	{
		sigToGroupsMap[groups[i].ComSigs[0]].push_back(idx);
	}

	for (std::vector<size_t>::iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end();)
	{
		std::cout << "PutInBySig " << std::endl;
		std::cout << "NO: " << i - vecWaitForGroup.begin() << std::endl;
		std::cout << "Total: " << vecWaitForGroup.size() << std::endl << std::endl;
		std::vector<CDfaNew> vecDfas(2);
		vecDfas[0] = res.GetDfaTable()[*i];
		bool flag = false;
		for (size_t j = 0; j < vecDfaInfo[*i].Sigs.size(); ++j)
		{
			if (sigToGroupsMap[vecDfaInfo[*i].Sigs[j]].size() != 0)
			{
				for (size_t k = 0; k < sigToGroupsMap[vecDfaInfo[*i].Sigs[j]].size(); ++k)
				{
					CDfaNew MergeDfa;
					MergeDfa.SetId(res.GetDfaTable().Size());
					vecDfas[1] = res.GetDfaTable()[groups[sigToGroupsMap[vecDfaInfo[*i].Sigs[j]][k]].mergeDfaId];
					//std::cout << *i << std::endl;
					//for (size_t ii = 0; ii < groups[sigToGroupsMap[vecDfaInfo[*i].Sigs[j]][k]].DfaIds.Size(); ++ii)
					//{
					//	std::cout << groups[sigToGroupsMap[vecDfaInfo[*i].Sigs[j]][k]].DfaIds[ii] << " ";
					//}
					//std::cout << std::endl;
					//outPutDfa(vecDfas[1], "..//..//output//dfa1.txt");
					//outPutDfa(vecDfas[0], "..//..//output//dfa2.txt");
					if (NOrMerge(vecDfas, MergeDfa))
					{
						flag = true;
						groups[sigToGroupsMap[vecDfaInfo[*i].Sigs[j]][k]].DfaIds.PushBack(*i);
						res.GetDfaTable().PushBack(MergeDfa);
						groups[sigToGroupsMap[vecDfaInfo[*i].Sigs[j]][k]].mergeDfaId = res.GetDfaTable().Size() - 1;
						break;
					}
				}
				if (flag)
				{
					break;
				}
			}
		}
		if (flag)
		{
			i = vecWaitForGroup.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void BuildGroupBySig(CGROUPS &newGroups, std::vector<size_t> &vecWaitForGroup, std::vector<DFAINFO> &vecDfaInfo)
{
	std::map<std::vector<SIGNATURE>, CVectorNumber> sigsToIdsMap;
	for (size_t i = 0; i < vecWaitForGroup.size(); ++i)
	{
		sigsToIdsMap[vecDfaInfo[vecWaitForGroup[i]].Sigs].PushBack(vecWaitForGroup[i]);
	}
	vecWaitForGroup.clear();

	newGroups.Resize(sigsToIdsMap.size());
	size_t idx = 0;
	for (std::map<std::vector<SIGNATURE>, CVectorNumber>::iterator i = sigsToIdsMap.begin(); i != sigsToIdsMap.end(); ++i, ++idx)
	{
		for (size_t j = 0; j < i->first.size(); ++j)
		{
			newGroups[idx].ComSigs.PushBack(i->first[j]);
		}
		newGroups[idx].DfaIds = i->second;
		newGroups[idx].mergeDfaId = -1;
	}
}

void ExtractUsedSigs(const CGROUPS &groups, std::vector<size_t> vecExplosion, std::vector<DFAINFO> vecDfaInfo, std::vector<SIGNATURE> &vecUsed)
{
	for (size_t i = 0; i < groups.Size(); ++i)
	{
		vecUsed.push_back(groups[i].ComSigs[0]);
	}
	//for (size_t i = 0; i < vecExplosion.size(); ++i)
	//{
	//	if (vecDfaInfo[vecExplosion[i]].Sigs.size() == 1)
	//	{
	//		vecUsed.push_back(vecDfaInfo[vecExplosion[i]].Sigs[0]);
	//	}
	//}
	std::sort(vecUsed.begin(), vecUsed.end());
	vecUsed.erase(std::unique(vecUsed.begin(), vecUsed.end()), vecUsed.end());
}

void ExtractComSigs(const GROUP &g1, const GROUP &g2, std::vector<SIGNATURE> &vecUsed, std::vector<SIGNATURE> &vecComSigs)
{
	std::map<SIGNATURE, size_t> sigToNumMap;
	for (size_t i = 0; i < g1.ComSigs.Size(); ++i)
	{
		++sigToNumMap[g1.ComSigs[i]];
	}
	for (size_t i = 0; i < g2.ComSigs.Size(); ++i)
	{
		++sigToNumMap[g2.ComSigs[i]];
	}

	for (std::map<SIGNATURE, size_t>::iterator i = sigToNumMap.begin(); i != sigToNumMap.end(); ++i)
	{
		if (i->second == 2 && std::find(vecUsed.begin(), vecUsed.end(), i->first) == vecUsed.end())
		{
			vecComSigs.push_back(i->first);
		}
	}
}

size_t AvailableNum(std::map<std::vector<SIGNATURE>, size_t> &SigsToNumMap, const std::vector<SIGNATURE> &vecComSigs)
{
	if (vecComSigs.size() <= 1)
	{
		return 0;
	}
	else
	{
		return vecComSigs.size() - 2;
	}

	//size_t count = vecComSigs.size();
	//for (std::vector<SIGNATURE>::const_iterator i = vecComSigs.begin(); i != vecComSigs.end(); ++i)
	//{
	//	std::vector<SIGNATURE> vecTwoSigs(2);
	//	std::vector<SIGNATURE> vecThreeSigs(3);
	//	vecTwoSigs[0] = *i;
	//	vecThreeSigs[0] = *i;
	//	for (std::vector<SIGNATURE>::const_iterator j = i + 1; j != vecComSigs.end(); ++j)
	//	{
	//		vecTwoSigs[1] = *j;
	//		vecThreeSigs[1] = *j;
	//		if (count <= SigsToNumMap[vecTwoSigs])
	//		{
	//			return 0;
	//		}
	//		count -= SigsToNumMap[vecTwoSigs];
	//		for (std::vector<SIGNATURE>::const_iterator k = j + 1; k != vecComSigs.end(); ++k)
	//		{
	//			vecThreeSigs[2] = *k;
	//			if (count <= SigsToNumMap[vecThreeSigs])
	//			{
	//				return 0;
	//			}
	//			count -= SigsToNumMap[vecThreeSigs];
	//		}
	//	}
	//}
	//if (count < 2)
	//{
	//	return 0;
	//}
	//return count - 2;
}

void MergeGroup(CResNew &res, std::vector<SIGNATURE> &vecUsed, CGROUPS &newGroups)
{
	std::map<std::vector<SIGNATURE>, size_t> SigsToNumMap;
	for (size_t i = 0; i < newGroups.Size(); ++i)
	{
		std::vector<SIGNATURE> vecSigs;
		for (size_t j = 0; j < newGroups[i].ComSigs.Size(); ++j)
		{
			if (std::find(vecUsed.begin(), vecUsed.end(), newGroups[i].ComSigs[j]) == vecUsed.end())
			{
				vecSigs.push_back(newGroups[i].ComSigs[j]);
			}
		}
		if (vecSigs.size() == 1)
		{
			vecUsed.push_back(vecSigs[0]);
		}
		else
		{
			++SigsToNumMap[vecSigs];
		}
	}
	for (size_t i = 0; i < newGroups.Size(); ++i)
	{
		std::cout << "MergeGroup " << std::endl;
		std::cout << "NO: " << i << std::endl;
		std::cout << "Total: " << newGroups.Size() << std::endl << std::endl;
		std::vector<SIGNATURE> vecSigs;
		for (size_t j = 0; j < newGroups[i].ComSigs.Size(); ++j)
		{
			if (std::find(vecUsed.begin(), vecUsed.end(), newGroups[i].ComSigs[j]) == vecUsed.end())
			{
				vecSigs.push_back(newGroups[i].ComSigs[j]);
			}
		}
		//++SigsToNumMap[vecSigs];
		std::vector<CDfaNew> vecDfas(2);
		vecDfas[0] = res.GetDfaTable()[newGroups[i].mergeDfaId];
		for (size_t j = i + 1; j < newGroups.Size(); )
		{
			std::vector<SIGNATURE> vecComSigs;
			ExtractComSigs(newGroups[i], newGroups[j], vecUsed, vecComSigs);
			if (SigsToNumMap[vecComSigs] >= AvailableNum(SigsToNumMap, vecComSigs))
			{
				++j;
				continue;
			}
			//if (!vecComSigs.empty())
			if (vecComSigs.size() > 1)
			{
				vecDfas[1] = res.GetDfaTable()[newGroups[j].mergeDfaId];
				CDfaNew MergeDfa;
				MergeDfa.SetId(res.GetDfaTable().Size());
				if (NOrMerge(vecDfas, MergeDfa))
				{
					if (SigsToNumMap[vecSigs] > 0)
					{
						if (SigsToNumMap[vecSigs] == AvailableNum(SigsToNumMap, vecSigs))
						{
							for (size_t k = 0; k < vecSigs.size(); ++k)
							{
								std::vector<SIGNATURE>::iterator iter = std::find(vecUsed.begin(), vecUsed.end(), vecSigs[k]);
								if (iter != vecUsed.end())
								{
									vecUsed.erase(iter);
								}
							}
						}
						--SigsToNumMap[vecSigs];
					}
					++SigsToNumMap[vecComSigs];
					if (SigsToNumMap[vecComSigs] == AvailableNum(SigsToNumMap, vecComSigs))
					{
						vecUsed.insert(vecUsed.end(), vecComSigs.begin(), vecComSigs.end());
						std::sort(vecUsed.begin(), vecUsed.end());
						vecUsed.erase(std::unique(vecUsed.begin(), vecUsed.end()), vecUsed.end());
					}
					for (size_t k = 0; k < newGroups[j].DfaIds.Size(); ++k)
					{
						newGroups[i].DfaIds.PushBack(newGroups[j].DfaIds[k]);
					}
					newGroups[i].ComSigs.Clear();
					for (size_t k = 0; k < vecComSigs.size(); ++k)
					{
						newGroups[i].ComSigs.PushBack(vecComSigs[k]);
					}
					vecSigs = vecComSigs;
					res.GetDfaTable().PushBack(MergeDfa);
					newGroups[i].mergeDfaId = res.GetDfaTable().Size() - 1;
					newGroups.Erase(j);
					vecDfas[0] = MergeDfa;
				}
				else
				{
					++j;
				}
			}
			else
			{
				++j;
			}
		}
	}
}

void AddNewGroups(CGROUPS &newGroups, std::vector<size_t> &vecExplosion, std::vector<DFAINFO> &vecDfaInfo, CGROUPS &groups)
{
	for (size_t i = 0; i < newGroups.Size(); ++i)
	{
		groups.PushBack(newGroups[i]);
	}
	newGroups.Clear();
	//size_t nSize = groups.Size();
	//size_t idx = 0;
	//groups.Resize(nSize + vecExplosion.size());
	//for (std::vector<size_t>::iterator i = vecExplosion.begin(); i != vecExplosion.end(); ++i, ++idx)
	//{
	//	groups[nSize + idx].DfaIds.PushBack(*i);
	//	for (size_t j = 0; j < vecDfaInfo[*i].Sigs.size(); ++j)
	//	{
	//		groups[nSize + idx].ComSigs.PushBack(vecDfaInfo[*i].Sigs[j]);
	//	}
	//	groups[nSize + idx].mergeDfaId = *i;
	//}
}

void ClearUpRes(CResNew &res, CGROUPS &groups, CGROUPRes &groupRes)
{
	groupRes.GetSidDfaIds() = res.GetSidDfaIds();
	std::vector<size_t> occurred(res.GetDfaTable().Size(), 0);
	for (size_t i = 0; i < groups.Size(); ++i)
	{
		occurred[groups[i].mergeDfaId] = 1;
	}
	std::map<size_t, size_t> oldToNewMap;
	size_t count = 0;
	size_t idx = 0;
	for (std::vector<size_t>::iterator i = occurred.begin(); i != occurred.end(); ++i, ++idx)
	{
		if (*i == 1)
		{
			groupRes.GetDfaTable().PushBack(res.GetDfaTable()[idx]);
			oldToNewMap[idx] = count;
			++count;
		}
		else
		{
			res.GetDfaTable()[idx].Clear();
		}
	}
	groupRes.GetGroups().Resize(groups.Size());
	for (size_t i = 0; i < groups.Size(); ++i)
	{
		groupRes.GetGroups()[i].DfaIds = groups[i].DfaIds;
		groupRes.GetGroups()[i].ComSigs = groups[i].ComSigs;
		groupRes.GetGroups()[i].mergeDfaId = oldToNewMap[groups[i].mergeDfaId];
	}

	std::vector<SIGNATURE> vecSpecialSigs;
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		if (groupRes.GetGroups()[i].ComSigs.Size() == 1)
		{
			vecSpecialSigs.push_back(groupRes.GetGroups()[i].ComSigs[0]);
		}
	}
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		if (groupRes.GetGroups()[i].ComSigs.Size() >= 2)
		{
			std::vector<SIGNATURE> vecSigs;
			for (size_t j = 0; j < groupRes.GetGroups()[i].ComSigs.Size(); ++j)
			{
				vecSigs.push_back(groupRes.GetGroups()[i].ComSigs[j]);
			}
			for (std::vector<SIGNATURE>::iterator j = vecSigs.begin(); j != vecSigs.end();)
			{
				if (std::find(vecSpecialSigs.begin(), vecSpecialSigs.end(), *j) != vecSpecialSigs.end())
				{
					j = vecSigs.erase(j);
				}
				else
				{
					++j;
				}
			}
			groupRes.GetGroups()[i].ComSigs.Clear();
			for (std::vector<SIGNATURE>::iterator j = vecSigs.begin(); j != vecSigs.end(); ++j)
			{
				groupRes.GetGroups()[i].ComSigs.PushBack(*j);
			}
		}
	}
}

GROUPINGSC void grouping(CResNew &res, CGROUPRes &groupRes)
{
	CTimer t1, tAll;

	std::cout << "Extract Dfa's information..." << std::endl;
	std::vector<DFAINFO> vecDfaInfo;
	std::vector<size_t> vecWaitForGroup;
	std::vector<size_t> vecExplosion;
	ExtractDfaInfo(res, vecDfaInfo, vecWaitForGroup, vecExplosion);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	std::cout << "Group dfa who has only one sig..." << std::endl;
	CGROUPS groups;
	GroupOnlyOneSig(vecDfaInfo, vecWaitForGroup, groups);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Merge dfa with only one signature...
	std::cout << "Merge dfa with only one signature..." << std::endl;
	Merge(res, groups);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Put dfa in group which have the same signature...
	std::cout << "Put dfa in group which have the same signature..." << std::endl;
	PutInBySig(res, groups, vecWaitForGroup, vecDfaInfo);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Build group which has the same signature...
	std::cout << "Build group which has the same signature..." << std::endl;
	CGROUPS newGroups;
	BuildGroupBySig(newGroups, vecWaitForGroup, vecDfaInfo);
	Merge(res, newGroups);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Extract the already used signatures...
	std::cout << "Extract the already used signatures..." << std::endl;
	std::vector<SIGNATURE> vecUsed;
	ExtractUsedSigs(groups, vecExplosion, vecDfaInfo, vecUsed);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Merge group which have the same signature...
	std::cout << "Merge group which have the same signature..." << std::endl;
	MergeGroup(res, vecUsed, newGroups);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;
	
	//Add new groups...
	std::cout << "Add new groups..." << std::endl;
	AddNewGroups(newGroups, vecExplosion, vecDfaInfo, groups);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Clear up the result...
	std::cout << "Clear up the result..." << std::endl;
	ClearUpRes(res, groups, groupRes);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	std::cout << groupRes.GetGroups().Size() << std::endl;

	std::cout << "Total time: " << tAll.Reset() << " Sec." << std::endl;
}