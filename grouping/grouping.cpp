/**
**  @file        grouping.cpp
**
**  @author      Lab 435, Xidian University
**
**  @brief       Support functions for grouping dfa
**
**  This implements grouping dfa algorithm, the algorithm has four steps
**  in general. At first, it groups dfa which has only one signature. Then, 
**  push the remain dfa into the group. Thirdly, it groups dfa with the same 
**  signatures. Finally, it merges two groups.
**
*/

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
	m_pGroups = new std::vector<ONEGROUP>;
}

GROUPINGSC CGROUPS::CGROUPS(const CGROUPS& other)
{
	m_pGroups = new std::vector<ONEGROUP>;
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

GROUPINGSC ONEGROUP &CGROUPS::operator[](size_t nIdx)
{
	return (*m_pGroups)[nIdx];
}

GROUPINGSC const ONEGROUP &CGROUPS::operator[](size_t nIdx) const
{
	return (*m_pGroups)[nIdx];
}

GROUPINGSC void CGROUPS::PushBack(ONEGROUP oneGroup)
{
	m_pGroups->push_back(oneGroup);
}

GROUPINGSC ONEGROUP& CGROUPS::Back()
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

/* Write the relationship between sid and dfa ids, dfa table and result of grouping
to file

Arguments:
  filename    path of the file waiting for written

Returns:      0 success
              -1 error occurred
*/

GROUPINGSC size_t CGROUPRes::WriteToFile(LPCTSTR filename)
{
	std::ofstream fout(filename, std::ios::binary);
	if (!fout)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (size_t)-1;
	}

	//mark the position used for file size
	std::streamoff fileSizePos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);

	//write the number of rules
	WriteNum(fout, m_sidDfaIds.Size());

	//mark the position used for the offset of rule
	std::streamoff ruleOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);

	//write the number of dfas
	WriteNum(fout, m_dfaTbl.Size());

	//mark the position used for the offset of dfa
	std::streamoff dfaOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);

	//write the number of groups
	WriteNum(fout, m_groups.Size());

	//mark the position used for the offset of group
	std::streamoff groupOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);

	//write the offset of rule
	std::streamoff endPos = fout.tellp();
	fout.seekp(ruleOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);

	//location to the end of the file
	fout.seekp(endPos, std::ios_base::beg);

	//start to write the relationship between sid and dfa id
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

	//write the offset of dfa
	endPos = fout.tellp();
	fout.seekp(dfaOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);

	//location to the end of the file
	fout.seekp(0, std::ios_base::end);

	//start to write dfas
	BYTE dfaDetails[100000];
	for (size_t i = 0; i < m_dfaTbl.Size(); ++i)
	{
		size_t len = m_dfaTbl[i].Save(dfaDetails);
		WriteNum(fout, len);
		fout.write((char*)dfaDetails, len * sizeof(BYTE));
	}

	//write the offset of group
	endPos = fout.tellp();
	fout.seekp(groupOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);

	//location to the end of the file
	fout.seekp(0, std::ios_base::end);

	//start to write groups
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
		WriteNum(fout, m_groups[i].currSig);
		WriteNum(fout, m_groups[i].mergeDfaId);
	}

	//write the file size
	endPos = fout.tellp();
	fout.seekp(fileSizePos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);
	fout.seekp(0, std::ios_base::end);
	fout.close();
	fout.clear();

	return 0;
}

/* Read the relationship between sid and dfa ids, dfa table and result of grouping
from file

Arguments:
  filename    path of the file to read from

Returns:      0 success
              -1 error occurred
*/

GROUPINGSC size_t CGROUPRes::ReadFromFile(LPCTSTR filename)
{
	std::ifstream fin(filename, std::ios::binary);
	if (!fin)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (size_t)-1;
	}

	//read the file size
	size_t fileSize;
	fin.read((char*)&fileSize, 4);

	//read the number of rules
	size_t ruleNum;
	fin.read((char*)&ruleNum, 4);

	//skip the offset of rule
	fin.seekg(4, std::ios_base::cur);

	//read the number of dfas
	size_t dfaNum;
	fin.read((char*)&dfaNum, 4);

	//skip the offset of dfa
	fin.seekg(4, std::ios_base::cur);

	//read the number of groups
	size_t groupNum;
	fin.read((char*)&groupNum, 4);

	//skip the offset of group
	fin.seekg(4, std::ios_base::cur);

	//start to read the relationship between sid and dfa ids
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

	//start to read dfas
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

	//start to read groups
	m_groups.Resize(groupNum);
	size_t nDfaIdNum;
	size_t nSigNum;
	for (size_t i = 0; i < groupNum; ++i)
	{
		ONEGROUP &oneGroup = m_groups[i];
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
		fin.read((char*)&oneGroup.currSig, sizeof(SIGNATURE));
		fin.read((char*)&oneGroup.mergeDfaId, 4);
	}
	fin.close();
	fin.clear();

	return 0;
}

/* Extract signatures from res to vecDfaInfo and add all index to vecWaitForGroup

Arguments:
  res               the compile result
  vecDfaInfo        signatures correspond to each dfa
  vecWaitForGroup   the index of dfa waiting for grouping

Returns:            nothing

*/

void ExtractDfaInfo(const CResNew &res, std::vector<DFAINFO> &vecDfaInfo, std::vector<size_t> &vecWaitForGroup)
{
	std::size_t nSize = res.GetRegexTbl().Size();
	vecDfaInfo.resize(nSize);
	for (size_t i = 0; i < nSize; ++i)
	{
		vecWaitForGroup.push_back(i);
		for (size_t j = 0; j < res.GetRegexTbl()[i].GetSigCnt(); ++j)
		{
			vecDfaInfo[i].Sigs.push_back(res.GetRegexTbl()[i].GetSig(j));
		}
	}
}

/* group dfa which has only one signature by its only signature

Arguments:
  vecDfaInfo        signatures correspond to each dfa
  vecWaitForGroup   the index of dfa waiting for grouping
  groups            the group result

Returns:            nothing

*/

void GroupOnlyOneSig(const std::vector<DFAINFO> &vecDfaInfo, std::vector<size_t> &vecWaitForGroup, CGROUPS &groups)
{
	std::map<SIGNATURE, CVectorNumber> sigToIdsMap;
	for (std::vector<size_t>::iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end();)
	{
		if (vecDfaInfo[*i].Sigs.size() == 1)
		{
			sigToIdsMap[vecDfaInfo[*i].Sigs[0]].PushBack(*i);

			//update the index of dfa waiting for grouping
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

/* try to merge dfa in one group, merge if success, otherwise seperate the group into two

Arguments:
  res               the compile result, the merged dfa will be pushed into the res's dfa table
  groups            the group result

Returns:            nothing

*/

void Merge(CResNew &res, CGROUPS &groups)
{
	for (size_t i = 0; i < groups.Size(); ++i)
	{
		std::cout << "Merge " << std::endl;
		std::cout << "NO: " << i << std::endl;
		std::cout << "Total: " << groups.Size() << std::endl << std::endl;
		groups[i].mergeDfaId = groups[i].DfaIds[0];

		//not need merge for group with only one dfa
		if (groups[i].DfaIds.Size() == 1)
		{
			continue;
		}
		std::vector<CDfaNew> vecDfas(2);
		CDfaNew MergeDfa;
		MergeDfa.SetId(res.GetDfaTable().Size());
		vecDfas[0] = res.GetDfaTable()[groups[i].DfaIds[0]];
		
		//flag if all dfas in the group can merge together
		bool mergeFlag = true;
		for (size_t j = 1; j < groups[i].DfaIds.Size(); ++j)
		{
			vecDfas[1] = res.GetDfaTable()[groups[i].DfaIds[j]];
			if (!MergeMultipleDfas(vecDfas, MergeDfa))
			{
				mergeFlag = false;

				//if first merge is success, it indicates that a new merged dfa need push into res's dfa table
				if (j != 1)
				{
					res.GetDfaTable().PushBack(vecDfas[0]);
					groups[i].mergeDfaId = res.GetDfaTable().Size() - 1;
				}
				groups.PushBack(ONEGROUP());
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

		//all dfas in the group can merge together
		if (mergeFlag)
		{
			res.GetDfaTable().PushBack(MergeDfa);
			groups[i].mergeDfaId = res.GetDfaTable().Size() - 1;
		}
	}
}

/* put dfa waiting for grouping into a group, if the dfa has the group's signature and 
can merge with the group's dfa merged before

Arguments:
  vecDfaInfo        signatures correspond to each dfa
  res               the compile result, the merged dfa will be pushed into the res's dfa table
  groups            the group result
  vecWaitForGroup   the index of dfa waiting for grouping

Returns:            nothing

*/

void PutInBySig(const std::vector<DFAINFO> &vecDfaInfo, CResNew &res, CGROUPS &groups, std::vector<size_t> &vecWaitForGroup)
{
	std::map<SIGNATURE, std::vector<size_t>> sigToGroupsMap;
	size_t idx = 0;
	for (size_t i = 0; i < groups.Size(); ++i, ++idx)
	{
		sigToGroupsMap[groups[i].ComSigs[0]].push_back(idx);
	}

	idx = 0;
	for (std::map<SIGNATURE, std::vector<size_t>>::iterator i = sigToGroupsMap.begin(); i != sigToGroupsMap.end(); ++i, ++idx)
	{
		std::cout << "PutInBySig " << std::endl;
		std::cout << "NO: " << idx << std::endl;
		std::cout << "Total: " << sigToGroupsMap.size() << std::endl << std::endl;
		for (std::vector<size_t>::iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			std::vector<CDfaNew> vecDfas(2);
			vecDfas[0] = res.GetDfaTable()[groups[*j].mergeDfaId];
			for (std::vector<size_t>::iterator k = vecWaitForGroup.begin(); k != vecWaitForGroup.end(); )
			{

				//the dfa doesn't have the group's signature
				if (std::find(vecDfaInfo[*k].Sigs.begin(), vecDfaInfo[*k].Sigs.end(), i->first) == vecDfaInfo[*k].Sigs.end())
				{
					++k;
					continue;
				}
				vecDfas[1] = res.GetDfaTable()[*k];
				CDfaNew MergeDfa;
				if (MergeMultipleDfas(vecDfas, MergeDfa))
				{
					res.GetDfaTable().PushBack(MergeDfa);
					groups[*j].DfaIds.PushBack(*k);
					groups[*j].mergeDfaId = res.GetDfaTable().Size() - 1;
					vecDfas[0] = MergeDfa;

					//update the index of dfa waiting for grouping
					k = vecWaitForGroup.erase(k);
				}
				else
				{
					++k;
				}
			}
		}
	}
}

/* group dfa which has the same signatures by its signatures

Arguments:
  vecDfaInfo        signatures correspond to each dfa
  newGroups         the new group result
  vecWaitForGroup   the index of dfa waiting for grouping

Returns:            nothing

*/

void BuildGroupBySig(const std::vector<DFAINFO> &vecDfaInfo, CGROUPS &newGroups, std::vector<size_t> &vecWaitForGroup)
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

/* extract signatures from group with only one signature

Arguments:
  groups            the group result, each group has only one signature
  vecUsed           the signatures which are used by group with only one signature

Returns:            nothing

*/

void ExtractUsedSigs(const CGROUPS &groups, std::vector<SIGNATURE> &vecUsed)
{
	for (size_t i = 0; i < groups.Size(); ++i)
	{
		vecUsed.push_back(groups[i].ComSigs[0]);
	}
	std::sort(vecUsed.begin(), vecUsed.end());
	vecUsed.erase(std::unique(vecUsed.begin(), vecUsed.end()), vecUsed.end());
}

/* extract common signatures from two groups

Arguments:
  g1                the first group
  g2                the second group
  vecUsed           the signatures which are used by group with only one signature
  vecComSigs        common signatures of the two groups

Returns:            nothing

*/

void ExtractComSigs(const ONEGROUP &g1, const ONEGROUP &g2, const std::vector<SIGNATURE> &vecUsed, std::vector<SIGNATURE> &vecComSigs)
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

		//the two group both have the signature and the signature is not contained in vecUsed
		if (i->second == 2 && std::find(vecUsed.begin(), vecUsed.end(), i->first) == vecUsed.end())
		{
			vecComSigs.push_back(i->first);
		}
	}
}

/* the used upper limit of the signatures

Arguments:
  vecSigs           signatures

Returns:            the used upper limit of the signatures

*/

size_t AvailableNum(const std::vector<SIGNATURE> &vecSigs)
{
	if (vecSigs.size() <= 1)
	{
		return 0;
	}
	else
	{
		return vecSigs.size() - 2;
	}
}

/* try to merge two groups, merge if the two group have common signatures , the used number 
of the common signatures isn't exceed its upper limit and the two groups merged dfa can merge

Arguments:
  res               the compile result, the merged dfa will be pushed into the res's dfa table
  vecUsed           the signatures which can not be used
  newGroups         the new group result

Returns:            nothing

*/

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
		while (true)
		{
			size_t idx = 0;
#undef min
			int nMax = std::numeric_limits<int>::min();
			for (size_t j = i + 1; j < newGroups.Size(); ++j)
			{
				std::vector<SIGNATURE> vecComSigs;
				ExtractComSigs(newGroups[i], newGroups[j], vecUsed, vecComSigs);
				if (SigsToNumMap[vecComSigs] >= AvailableNum(vecComSigs))
				{
					continue;
				}
				if (vecComSigs.size() > 1)
				{
					vecDfas[1] = res.GetDfaTable()[newGroups[j].mergeDfaId];
					CDfaNew MergeDfa;
					if (MergeMultipleDfas(vecDfas, MergeDfa))
					{

						//select the fittest group to merge
						int nReduceSize = vecDfas[0].Size() + vecDfas[1].Size() - MergeDfa.Size();
						if (nMax < nReduceSize)
						{
							nMax = nReduceSize;
							idx = j;
						}
					}
				}
			}

			//no group can merge with the current group
			if (idx == 0)
			{
				break;
			}
			else
			{
				std::vector<SIGNATURE> vecComSigs;
				ExtractComSigs(newGroups[i], newGroups[idx], vecUsed, vecComSigs);
				vecDfas[1] = res.GetDfaTable()[newGroups[idx].mergeDfaId];
				CDfaNew MergeDfa;
				if (MergeMultipleDfas(vecDfas, MergeDfa))
				{
					if (SigsToNumMap[vecSigs] > 0)
					{
						if (SigsToNumMap[vecSigs] == AvailableNum(vecSigs))
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
					if (SigsToNumMap[vecComSigs] == AvailableNum(vecComSigs))
					{
						vecUsed.insert(vecUsed.end(), vecComSigs.begin(), vecComSigs.end());
						std::sort(vecUsed.begin(), vecUsed.end());
						vecUsed.erase(std::unique(vecUsed.begin(), vecUsed.end()), vecUsed.end());
					}
					for (size_t k = 0; k < newGroups[idx].DfaIds.Size(); ++k)
					{
						newGroups[i].DfaIds.PushBack(newGroups[idx].DfaIds[k]);
					}
					newGroups[i].ComSigs.Clear();
					for (size_t k = 0; k < vecComSigs.size(); ++k)
					{
						newGroups[i].ComSigs.PushBack(vecComSigs[k]);
					}
					vecSigs = vecComSigs;
					res.GetDfaTable().PushBack(MergeDfa);
					newGroups[i].mergeDfaId = res.GetDfaTable().Size() - 1;
					newGroups.Erase(idx);
					vecDfas[0] = MergeDfa;
				}
				else
				{
					std::cout << "ERROR" << std::endl;
					system("pause");
				}
			}
		}
	}
}

/* add new group result to group result

Arguments:
  groups            the group result
  newGroups         the new group result

Returns:            nothing

*/

void AddNewGroups(CGROUPS &newGroups, CGROUPS &groups)
{
	for (size_t i = 0; i < newGroups.Size(); ++i)
	{
		groups.PushBack(newGroups[i]);
	}
	newGroups.Clear();
}

/* clear up the group result

Arguments:
  res               the compile result and its dfa table contains the merged dfas
  groups            the group result
  groupRes          the relationship between sid and dfa ids, dfa table and result of grouping

Returns:            nothing

*/

void ClearUpRes(CResNew &res, const CGROUPS &groups, CGROUPRes &groupRes)
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
			if (!vecSigs.empty())
			{
				groupRes.GetGroups()[i].ComSigs.Clear();
				for (std::vector<SIGNATURE>::iterator j = vecSigs.begin(); j != vecSigs.end(); ++j)
				{
					groupRes.GetGroups()[i].ComSigs.PushBack(*j);
				}
			}
		}
	}
}

void outPutGroups(CGROUPRes &groupRes, const char* fileName)
{
	std::ofstream fout(fileName);
	if(!fout)
	{
		std::cerr << "open file failure!" << std::endl;
		return;
	}
	fout << "groupNumber\t" << "MergeId\t" << "origDfaId" << std::endl; 
	for(size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		fout << i << "\t";
		fout << groupRes.GetGroups()[i].mergeDfaId << "\t";
		for(size_t j = 0; j < groupRes.GetGroups()[i].DfaIds.Size(); ++j)
		{
			fout << groupRes.GetGroups()[i].DfaIds[j] << std::endl << "\t" << "\t";
		}
		fout << std::endl;
	}
	fout.close();
}

void outPutResult(CGROUPRes &groupRes, const char* fileName)
{
	std::ofstream fout(fileName);
	if(!fout)
	{
		std::cerr << "open file failure!" << std::endl;
		return;
	}
	fout << "MergeId\t" << "ColumCnt\t" << "Size\t" << "TermSetSize" << std::endl;
	for(size_t i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	{
		fout << groupRes.GetDfaTable()[i].GetId() << "\t";
		fout << groupRes.GetDfaTable()[i].GetGroupCount() << "\t";
		fout << groupRes.GetDfaTable()[i].Size() << "\t";
		fout << groupRes.GetDfaTable()[i].GetTermCnt() << std::endl;
	}
	fout.close();
}

void outPutTermSet(CGROUPRes &groupRes, const char* fileName)
{
	std::ofstream fout(fileName);
	if(!fout)
	{
		std::cerr << "open file failure!" << std::endl;
		return;
	}
	fout << "MergeId\t" << "dfaSta\t" << "origdfaId" << std::endl;
	for(size_t i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	{
		fout << groupRes.GetDfaTable()[i].GetId() << "\t";
		for(size_t j = 0; j < groupRes.GetDfaTable()[i].GetTermCnt(); ++j)
		{
			fout << groupRes.GetDfaTable()[i].GetTerm(j).dfaSta << "\t";
			fout << groupRes.GetDfaTable()[i].GetTerm(j).dfaId << std::endl << "\t";
		}
		fout << std::endl;
	}
	fout.close();
}

/* the grouping algorithm

Arguments:
  res               the compile result
  groupRes          the relationship between sid and dfa ids, dfa table and result of grouping

Returns:            nothing

*/

GROUPINGSC void grouping(CResNew &res, CGROUPRes &groupRes)
{
	CTimer t1, tAll;

	std::cout << "Extract Dfa's information..." << std::endl;
	std::vector<DFAINFO> vecDfaInfo;
	std::vector<size_t> vecWaitForGroup;
	ExtractDfaInfo(res, vecDfaInfo, vecWaitForGroup);
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
	PutInBySig(vecDfaInfo, res, groups, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Build group which has the same signature...
	std::cout << "Build group which has the same signatures..." << std::endl;
	CGROUPS newGroups;
	BuildGroupBySig(vecDfaInfo, newGroups, vecWaitForGroup);
	Merge(res, newGroups);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Extract the already used signatures...
	std::cout << "Extract the already used signatures..." << std::endl;
	std::vector<SIGNATURE> vecUsed;
	ExtractUsedSigs(groups, vecUsed);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Merge group which have the same signature...
	std::cout << "Merge group which have the same signatures..." << std::endl;
	MergeGroup(res, vecUsed, newGroups);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;
	
	//Add new groups...
	std::cout << "Add new groups..." << std::endl;
	AddNewGroups(newGroups, groups);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Clear up the result...
	std::cout << "Clear up the result..." << std::endl;
	ClearUpRes(res, groups, groupRes);

	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	std::cout << groupRes.GetGroups().Size() << std::endl;

	std::cout << "Total time: " << tAll.Reset() << " Sec." << std::endl;
}