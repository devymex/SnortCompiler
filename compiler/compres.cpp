#include "stdafx.h"
#include <hwprj\compres.h>


template<typename _Ty>
void WriteNum(std::ofstream &fout, _Ty _num, ULONG nBytes = sizeof(_Ty))
{
	fout.write((char*)&_num, nBytes);
}


COMPRESHDR CDfaArray& CCompileResults::GetDfaTable()
{
	return m_dfaTbl;
}

COMPRESHDR CSidDfaIds& CCompileResults::GetSidDfaIds()
{
	return m_sidDfaIds;
}

COMPRESHDR CRegRule &CCompileResults::GetRegexTbl()
{
	return m_RegexTbl;
}

COMPRESHDR const CDfaArray& CCompileResults::GetDfaTable() const
{
	return m_dfaTbl;
}

COMPRESHDR const CSidDfaIds& CCompileResults::GetSidDfaIds() const
{
	return m_sidDfaIds;
}

COMPRESHDR const CRegRule &CCompileResults::GetRegexTbl() const
{
	return m_RegexTbl;
}

/* Write the relationship between sid and dfa ids, dfa table and option list
table to file

Arguments:
  filename    path of the file waiting for written

Returns:      0 success
              -1 error occurred
*/
COMPRESHDR ULONG CCompileResults::WriteToFile(LPCTSTR filename)
{
	std::ofstream fout(filename, std::ios::binary);
	if (!fout)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (ULONG)-1;
	}

	//file label DFAS
	std::string symbolStr = "DFAS";
	for (std::string::iterator i = symbolStr.begin(); i != symbolStr.end(); ++i)
	{
		WriteNum(fout, *i);
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

	//write the number of regexes
	WriteNum(fout, m_RegexTbl.Size());

	//mark the position used for the offset of regex
	std::streamoff RegexTblOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);

	//write the offset of rule
	std::streamoff endPos = fout.tellp();
	fout.seekp(ruleOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);

	//location to the end of the file
	fout.seekp(endPos, std::ios_base::beg);

	//start to write the relationship between sid and dfa ids
	for (ULONG i = 0; i < m_sidDfaIds.Size(); ++i)
	{
		COMPILEDRULENEW &ruleResult = m_sidDfaIds[i];
		WriteNum(fout, ruleResult.m_nSid);
		WriteNum(fout, ruleResult.m_nResult);
		WriteNum(fout, ruleResult.m_dfaIds.Size());
		for (ULONG j = 0; j < ruleResult.m_dfaIds.Size(); ++j)
		{
			WriteNum(fout, ruleResult.m_dfaIds[j]);
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
	for (ULONG i = 0; i < m_dfaTbl.Size(); ++i)
	{
		ULONG len = m_dfaTbl[i].Save(dfaDetails);
		WriteNum(fout, len);
		fout.write((char*)dfaDetails, len * sizeof(BYTE));
	}

	//write the offset of regex
	endPos = fout.tellp();
	fout.seekp(RegexTblOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);

	//location to the end of the file
	fout.seekp(0, std::ios_base::end);

	//start to write regexes
	for (ULONG i = 0; i < m_RegexTbl.Size(); ++i)
	{
		WriteNum(fout, m_RegexTbl[i].Size());
		for (ULONG j = 0; j < m_RegexTbl[i].Size(); ++j)
		{
			WriteNum(fout, m_RegexTbl[i][j].Size());
			const char *pString = m_RegexTbl[i][j].GetStr();
			fout.write(pString, strlen(pString));
		}
		WriteNum(fout, m_RegexTbl[i].GetSigs().Size());
		for (ULONG j = 0; j < m_RegexTbl[i].GetSigs().Size(); ++j)
		{
			WriteNum(fout, m_RegexTbl[i].GetSigs()[j]);
		}
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

/* read the relationship between sid and dfa ids, dfa table and option list
table from file

Arguments:
  filename    path of the file waiting for written

Returns:      0 success
              -1 error occurred
*/

COMPRESHDR ULONG CCompileResults::ReadFromFile(LPCTSTR filename)
{
	std::ifstream fin(filename, std::ios::binary);
	if (!fin)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (ULONG)-1;
	}

	//check file label
	std::string symbolStr = "DFAS";
	unsigned char c;
	for (std::string::iterator i = symbolStr.begin(); i != symbolStr.end(); ++i)
	{
		fin.read((char*)&c, 1);
		if (c != *i)
		{
			std::cerr << "File format error!" << std::endl;
			return (ULONG)-1;
		}
	}

	//read the file size
	ULONG fileSize;
	fin.read((char*)&fileSize, 4);

	//read the number of rules
	ULONG ruleNum;
	fin.read((char*)&ruleNum, 4);

	//skip the offset of rule
	fin.seekg(4, std::ios_base::cur);

	//read the number of dfas
	ULONG dfaNum;
	fin.read((char*)&dfaNum, 4);

	//skip the offset of dfa
	fin.seekg(4, std::ios_base::cur);

	//read the number of regexes
	ULONG RegexTblSize;
	fin.read((char*)&RegexTblSize, 4);

	//skip the offset of regex
	fin.seekg(4, std::ios_base::cur);

	//start to read the relationship between sid and dfa ids
	m_sidDfaIds.Resize(ruleNum);
	ULONG SidDfaNum;
	for (ULONG i = 0; i < ruleNum; ++i)
	{
		COMPILEDRULENEW &ruleResult = m_sidDfaIds[i];
		fin.read((char*)&ruleResult.m_nSid, 4);
		fin.read((char*)&ruleResult.m_nResult, 4);
		fin.read((char*)&SidDfaNum, 4);
		ruleResult.m_dfaIds.Resize(SidDfaNum);
		for (ULONG j = 0; j < SidDfaNum; ++j)
		{
			fin.read((char*)&(ruleResult.m_dfaIds[j]), 4);
		}
	}

	//start to read dfas
	m_dfaTbl.Resize(dfaNum);
	BYTE dfaDetails[100000];
	for (ULONG i = 0; i < dfaNum; ++i)
	{
		CDfa &dfa = m_dfaTbl[i];
		ULONG len;
		fin.read((char*)&len, 4);
		fin.read((char*)dfaDetails, len * sizeof(BYTE));
		dfa.Load(dfaDetails, len);
	}

	//start to read regexes
	m_RegexTbl.Resize(RegexTblSize);
	ULONG ChainSize;
	ULONG RegSize;
	ULONG SigSize;
	for (ULONG i = 0; i < RegexTblSize; ++i)
	{
		fin.read((char*)&ChainSize, 4);
		for (ULONG j = 0; j < ChainSize; ++j)
		{
			fin.read((char*)&RegSize, 4);
			char *pString = new char[RegSize + 1];
			pString[RegSize] = '\0';
			fin.read(pString, RegSize);
			m_RegexTbl[i].PushBack(CDllString(pString));
		}
		fin.read((char*)&SigSize, 4);
		for (ULONG j = 0; j < SigSize; ++j)
		{
			SIGNATURE Sig;
			fin.read((char*)&Sig, 4);
			m_RegexTbl[i].GetSigs().PushBack(Sig);
		}
	}
	fin.close();
	fin.clear();

	return 0;
}