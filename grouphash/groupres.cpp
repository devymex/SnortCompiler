#include "stdafx.h"
#include <hwprj\groupres.h>

GROUPRESHDR CDfaArray &CGroupRes::GetDfaTable()
{
	return m_dfaTbl;
}

GROUPRESHDR CSidDfaIds &CGroupRes::GetSidDfaIds()
{
	return m_sidDfaIds;
}

GROUPRESHDR CGroups &CGroupRes::GetGroups()
{
	return m_groups;
}

GROUPRESHDR const CDfaArray &CGroupRes::GetDfaTable() const
{
	return m_dfaTbl;
}

GROUPRESHDR const CSidDfaIds &CGroupRes::GetSidDfaIds() const
{
	return m_sidDfaIds;
}

GROUPRESHDR	const CGroups &CGroupRes::GetGroups() const
{
	return m_groups;
}

GROUPRESHDR	ulong &CGroupRes::GetBucketCnt()
{
	return nBucketCnt;
}

GROUPRESHDR const ulong &CGroupRes::GetBucketCnt() const
{
	return nBucketCnt;
}

GROUPRESHDR	void CGroupRes::SetBucketCnt(ulong nBucketSize)
{
	nBucketCnt = nBucketSize;
}

template<typename _Ty>
void WriteNum(std::ofstream &fout, _Ty _num, ulong nBytes = sizeof(_Ty))
{
	fout.write((char*)&_num, nBytes);
}

/* Write the relationship between sid and dfa ids, dfa table and result of grouping
to file

Arguments:
  filename	 path of the file waiting for written

Returns:		0 success
				-1 error occurred
*/

GROUPRESHDR ulong CGroupRes::WriteToFile(const char *filename)
{
	std::ofstream fout(filename, std::ios::binary);
	if (!fout)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (ulong)-1;
	}

	//used by hash function
	WriteNum(fout, nBucketCnt);

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
	for (ulong i = 0; i < m_sidDfaIds.Size(); ++i)
	{
		COMPILEDINFO &ruleResult = m_sidDfaIds[i];
		WriteNum(fout, ruleResult.m_nSid, 4);
		WriteNum(fout, ruleResult.m_nResult, 4);
		WriteNum(fout, ruleResult.m_dfaIds.Size(), 4);
		for (ulong j = 0; j < ruleResult.m_dfaIds.Size(); ++j)
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
	for (ulong i = 0; i < m_dfaTbl.Size(); ++i)
	{
		ulong len = m_dfaTbl[i].CalcStoreSize();
		byte *dfaDetails = new byte[len];
		m_dfaTbl[i].Save(dfaDetails);
		WriteNum(fout, len);
		fout.write((char*)dfaDetails, len * sizeof(byte));
		delete []dfaDetails;
	}

	//write the offset of group
	endPos = fout.tellp();
	fout.seekp(groupOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);

	//location to the end of the file
	fout.seekp(0, std::ios_base::end);

	//start to write groups
	for (ulong i = 0; i < m_groups.Size(); ++i)
	{
		WriteNum(fout, m_groups[i].DfaIds.Size());
		for (ulong j = 0; j < m_groups[i].DfaIds.Size(); ++j)
		{
			WriteNum(fout, m_groups[i].DfaIds[j]);
		}
		WriteNum(fout, m_groups[i].ComSigs.Size());
		for (ulong j = 0; j < m_groups[i].ComSigs.Size(); ++j)
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
  filename	 path of the file to read from

Returns:		0 success
				-1 error occurred
*/

GROUPRESHDR ulong CGroupRes::ReadFromFile(const char *filename)
{
	std::ifstream fin(filename, std::ios::binary);
	if (!fin)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (ulong)-1;
	}

	//used by hash function
	fin.read((char*)&nBucketCnt, 4);

	//read the file size
	ulong fileSize;
	fin.read((char*)&fileSize, 4);

	//read the number of rules
	ulong ruleNum;
	fin.read((char*)&ruleNum, 4);

	//skip the offset of rule
	fin.seekg(4, std::ios_base::cur);

	//read the number of dfas
	ulong dfaNum;
	fin.read((char*)&dfaNum, 4);

	//skip the offset of dfa
	fin.seekg(4, std::ios_base::cur);

	//read the number of groups
	ulong groupNum;
	fin.read((char*)&groupNum, 4);

	//skip the offset of group
	fin.seekg(4, std::ios_base::cur);

	//start to read the relationship between sid and dfa ids
	m_sidDfaIds.Resize(ruleNum);
	ulong SidDfaNum;
	for (ulong i = 0; i < ruleNum; ++i)
	{
		COMPILEDINFO &ruleResult = m_sidDfaIds[i];
		fin.read((char*)&ruleResult.m_nSid, 4);
		fin.read((char*)&ruleResult.m_nResult, 4);
		fin.read((char*)&SidDfaNum, 4);
		ruleResult.m_dfaIds.Resize(SidDfaNum);
		for (ulong j = 0; j < SidDfaNum; ++j)
		{
			fin.read((char*)&(ruleResult.m_dfaIds[j]), 4);
		}
	}

	//start to read dfas
	m_dfaTbl.Resize(dfaNum);
	for (ulong i = 0; i < dfaNum; ++i)
	{
		CDfa &dfa = m_dfaTbl[i];
		ulong len;
		fin.read((char*)&len, 4);
		byte *dfaDetails = new byte[len];
		fin.read((char*)dfaDetails, len * sizeof(byte));
		dfa.Load(dfaDetails);
		delete []dfaDetails;
	}

	//start to read groups
	m_groups.Resize(groupNum);
	ulong nDfaIdNum;
	ulong nSigNum;
	for (ulong i = 0; i < groupNum; ++i)
	{
		ONEGROUP &oneGroup = m_groups[i];
		fin.read((char*)&nDfaIdNum, 4);
		oneGroup.DfaIds.Resize(nDfaIdNum);
		for (ulong j = 0; j < nDfaIdNum; ++j)
		{
			ulong &nDfaId = oneGroup.DfaIds[j];
			fin.read((char*)&nDfaId, 4);
		}
		fin.read((char*)&nSigNum, 4);
		oneGroup.ComSigs.Resize(nSigNum);
		for (ulong j = 0; j < nSigNum; ++j)
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
