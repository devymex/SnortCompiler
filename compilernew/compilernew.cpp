/**
**  @file        compilernew.cpp
**
**  @author      Lab 435, Xidian University
**
**  @brief       Support functions for compile rule to dfa
**
**  This implements compile rule to dfa algorithm
**
*/

#include "stdafx.h"
#include "../common/common.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../dfanew/dfanew.h"
#include "compilernew.h"

COMPILERNEW CDfaTblNew::CDfaTblNew()
{
	m_pdfaTbl = new std::vector<CDfaNew>;	
}

COMPILERNEW CDfaTblNew::CDfaTblNew(const CDfaTblNew& other)
{
	m_pdfaTbl = new std::vector<CDfaNew>;
	*this = other;
}

COMPILERNEW const CDfaTblNew &CDfaTblNew::operator=(const CDfaTblNew &other)
{
	*m_pdfaTbl = *other.m_pdfaTbl;
	return *this;
}

COMPILERNEW CDfaTblNew::~CDfaTblNew()
{
	delete m_pdfaTbl;
}

COMPILERNEW CDfaNew& CDfaTblNew::operator[](size_t index)
{
	return (*m_pdfaTbl)[index];
}

COMPILERNEW const CDfaNew& CDfaTblNew::operator[](size_t index) const
{
	return (*m_pdfaTbl)[index];
}

COMPILERNEW void CDfaTblNew::Reserve(size_t nCount)
{
	m_pdfaTbl->reserve(nCount);
}

COMPILERNEW void CDfaTblNew::Resize(size_t nSize)
{
	m_pdfaTbl->resize(nSize);
}

COMPILERNEW const size_t CDfaTblNew::Size() const
{
	return m_pdfaTbl->size();
}

COMPILERNEW void CDfaTblNew::PushBack(const CDfaNew &dfa)
{
	m_pdfaTbl->push_back(dfa);
}

COMPILERNEW void CDfaTblNew::Clear()
{
	m_pdfaTbl->clear();
}

COMPILERNEW CSidDfaIdsNew::CSidDfaIdsNew()
{
	m_ruleResult = new std::vector<COMPILEDRULENEW>;
}

COMPILERNEW CSidDfaIdsNew::CSidDfaIdsNew(const CSidDfaIdsNew& other)
{
	m_ruleResult = new std::vector<COMPILEDRULENEW>;
	*this = other;
}

COMPILERNEW const CSidDfaIdsNew &CSidDfaIdsNew::operator=(const CSidDfaIdsNew &other)
{
	*m_ruleResult = *other.m_ruleResult;
	return *this;
}
COMPILERNEW CSidDfaIdsNew::~CSidDfaIdsNew()
{
	delete m_ruleResult;
}

COMPILERNEW COMPILEDRULENEW& CSidDfaIdsNew::operator[](size_t index)
{
	return (*m_ruleResult)[index];
}

COMPILERNEW const COMPILEDRULENEW& CSidDfaIdsNew::operator[](size_t index) const
{
	return (*m_ruleResult)[index];
}

COMPILERNEW void CSidDfaIdsNew::Reserve(size_t nCount)
{
	m_ruleResult->reserve(nCount);
}

COMPILERNEW void CSidDfaIdsNew::Resize(size_t nSize)
{
	m_ruleResult->resize(nSize);
}

COMPILERNEW const size_t CSidDfaIdsNew::Size() const
{
	return m_ruleResult->size();
}

COMPILERNEW void CSidDfaIdsNew::PushBack(const COMPILEDRULENEW &ruleResult)
{
	m_ruleResult->push_back(ruleResult);
}

COMPILERNEW COMPILEDRULENEW& CSidDfaIdsNew::Back()
{
	return m_ruleResult->back();
}

COMPILERNEW CDfaTblNew& CResNew::GetDfaTable()
{
	return m_dfaTbl;
}

COMPILERNEW CSidDfaIdsNew& CResNew::GetSidDfaIds()
{
	return m_sidDfaIds;
}

COMPILERNEW CRegRule &CResNew::GetRegexTbl()
{
	return m_RegexTbl;
}

COMPILERNEW const CDfaTblNew& CResNew::GetDfaTable() const
{
	return m_dfaTbl;
}

COMPILERNEW const CSidDfaIdsNew& CResNew::GetSidDfaIds() const
{
	return m_sidDfaIds;
}

COMPILERNEW const CRegRule &CResNew::GetRegexTbl() const
{
	return m_RegexTbl;
}

template<typename _Ty>
void WriteNum(std::ofstream &fout, _Ty _num, size_t nBytes = sizeof(_Ty))
{
	fout.write((char*)&_num, nBytes);
}

/* Write the relationship between sid and dfa ids, dfa table and option list
table to file

Arguments:
  filename    path of the file waiting for written

Returns:      0 success
              -1 error occurred
*/

COMPILERNEW size_t CResNew::WriteToFile(LPCTSTR filename)
{
	std::ofstream fout(filename, std::ios::binary);
	if (!fout)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (size_t)-1;
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
	for (size_t i = 0; i < m_sidDfaIds.Size(); ++i)
	{
		COMPILEDRULENEW &ruleResult = m_sidDfaIds[i];
		WriteNum(fout, ruleResult.m_nSid);
		WriteNum(fout, ruleResult.m_nResult);
		WriteNum(fout, ruleResult.m_dfaIds.Size());
		for (size_t j = 0; j < ruleResult.m_dfaIds.Size(); ++j)
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
	for (size_t i = 0; i < m_dfaTbl.Size(); ++i)
	{
		size_t len = m_dfaTbl[i].Save(dfaDetails);
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
	for (size_t i = 0; i < m_RegexTbl.Size(); ++i)
	{
		WriteNum(fout, m_RegexTbl[i].Size());
		for (size_t j = 0; j < m_RegexTbl[i].Size(); ++j)
		{
			WriteNum(fout, m_RegexTbl[i][j].Size());
			const char *pString = m_RegexTbl[i][j].GetStr();
			fout.write(pString, strlen(pString));
		}
		WriteNum(fout, m_RegexTbl[i].GetSigs().Size());
		for (size_t j = 0; j < m_RegexTbl[i].GetSigs().Size(); ++j)
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

COMPILERNEW size_t CResNew::ReadFromFile(LPCTSTR filename)
{
	std::ifstream fin(filename, std::ios::binary);
	if (!fin)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (size_t)-1;
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
			return (size_t)-1;
		}
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

	//read the number of regexes
	size_t RegexTblSize;
	fin.read((char*)&RegexTblSize, 4);

	//skip the offset of regex
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

	//start to read regexes
	m_RegexTbl.Resize(RegexTblSize);
	size_t ChainSize;
	size_t RegSize;
	size_t SigSize;
	for (size_t i = 0; i < RegexTblSize; ++i)
	{
		fin.read((char*)&ChainSize, 4);
		for (size_t j = 0; j < ChainSize; ++j)
		{
			fin.read((char*)&RegSize, 4);
			char *pString = new char[RegSize + 1];
			pString[RegSize] = '\0';
			fin.read(pString, RegSize);
			m_RegexTbl[i].PushBack(CCString(pString));
		}
		fin.read((char*)&SigSize, 4);
		for (size_t j = 0; j < SigSize; ++j)
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

/* assign all the signatures of each rule to all its option list

Arguments:
  result      the compile result
  BegIdx      the index of the first regex which belongs to this rule
  EndIdx      the next index of the last regex which belongs to this rule

Returns:      nothing

*/

void AssignSig(CResNew &result, size_t BegIdx, size_t EndIdx)
{
	//all the signatures of this rule
	std::vector<SIGNATURE> vecRuleSigs;
	for (size_t i = BegIdx; i < EndIdx; ++i)
	{
		for (size_t j = 0; j < result.GetRegexTbl()[i].GetSigs().Size(); ++j)
		{
			if (std::find(vecRuleSigs.begin(), vecRuleSigs.end(),
				result.GetRegexTbl()[i].GetSigs()[j]) == vecRuleSigs.end())
			{
				vecRuleSigs.push_back(result.GetRegexTbl()[i].GetSigs()[j]);
			}
		}
	}

	//assign all the signatures of each rule to all its option list
	for (size_t i = BegIdx; i < EndIdx; ++i)
	{
		result.GetRegexTbl()[i].GetSigs().Clear();
		for (size_t j = 0; j < vecRuleSigs.size(); ++j)
		{
			result.GetRegexTbl()[i].GetSigs().PushBack(vecRuleSigs[j]);
		}
	}
}

double rule2pcretime = 0.0;
double pcre2nfatime = 0.0;
double nfa2dfatime = 0.0;
double dfamintimetime = 0.0;

/* complie one rule to several dfas

Arguments:
  rule        the snort rule
  result      the compile result
  ruleResult  the relationship between sid and dfa ids

Returns:      nothing

*/

void Rule2Dfas(const CSnortRule &rule, CResNew &result, COMPILEDRULENEW &ruleResult)
{
	CRegRule regrule;
	CTimer ctime;//for test
	ctime.Reset();//for test

	size_t flag = Rule2PcreList(rule, regrule);
	rule2pcretime += ctime.Reset();//for test

	if (flag == SC_ERROR)
	{
		ruleResult.m_nResult = COMPILEDRULENEW::RES_ERROR;
		return;
	}
	else
	{
		ruleResult.m_nResult = COMPILEDRULENEW::RES_SUCCESS;
		const size_t nDfaTblSize = result.GetDfaTable().Size();
		const size_t nIncrement = regrule.Size();
		result.GetDfaTable().Resize(nDfaTblSize + nIncrement);
		const size_t nRegexTblSize = result.GetRegexTbl().Size();
		result.GetRegexTbl().Resize(nRegexTblSize + nIncrement);
		size_t nDfaId;
		size_t nChainId;
		bool bHasSigs = false;
		for (size_t i = 0; i < nIncrement; ++i)
		{
			CNfa nfa;

			ctime.Reset();//for test
			size_t nToNFAFlag = CRegChainToNFA(regrule[i], nfa);
			pcre2nfatime += ctime.Reset();//for test

			if (regrule[i].GetSigs().Size() > 0)
			{
				bHasSigs = true;
			}

			nDfaId = nDfaTblSize + i;
			nChainId = nRegexTblSize + i;
			CDfaNew &dfa = result.GetDfaTable()[nDfaId];
			if (nToNFAFlag == SC_ERROR)
			{
				ruleResult.m_nResult = COMPILEDRULENEW::RES_ERROR;
				ruleResult.m_dfaIds.Clear();
				result.GetDfaTable().Resize(nDfaTblSize);
				result.GetRegexTbl().Resize(nRegexTblSize);
				return;
			}
			else if (nToNFAFlag == SC_EXCEED)
			{
				ruleResult.m_nResult = COMPILEDRULENEW::RES_EXCEED;
			}
			else
			{
				ctime.Reset();//for test
				dfa.SetId(nDfaId);
				size_t nToDFAFlag = dfa.FromNFA(nfa);
				nfa2dfatime += ctime.Reset();//for test

				if (nToDFAFlag == -1)
				{
					ruleResult.m_nResult = COMPILEDRULENEW::RES_EXCEEDLIMIT;
					dfa.Clear();
				}
				else
				{
					ctime.Reset();//for test
					size_t nr = dfa.Minimize();
					if (dfa.GetFinalState().Size() == 0)
					{
						system("pause");
					}
					dfamintimetime += ctime.Reset();//for test
					if (0 != nr || dfa.Size() > DFA_SIZE_LIMIT)
					{
						ruleResult.m_nResult = COMPILEDRULENEW::RES_EXCEEDLIMIT;
						dfa.Clear();
					}
				}
			}
			if (dfa.Size() == 0)
			{
				ruleResult.m_dfaIds.Clear();
				result.GetDfaTable().Resize(nDfaTblSize);
				result.GetRegexTbl().Resize(nRegexTblSize);
				return;
			}
			ruleResult.m_dfaIds.PushBack(nDfaId);
			result.GetRegexTbl()[nChainId] = regrule[i];
		}

		if (!bHasSigs)
		{
			ruleResult.m_nResult = COMPILEDRULENEW::RES_HASNOSIG;
			ruleResult.m_dfaIds.Clear();
			result.GetDfaTable().Resize(nDfaTblSize);
			result.GetRegexTbl().Resize(nRegexTblSize);
			return;
		}

		if (ruleResult.m_nResult != COMPILEDRULENEW::RES_ERROR)
		{
			AssignSig(result, nRegexTblSize, nRegexTblSize + nIncrement);
		}
	}
}

/* complie one rule

Arguments:
  rule        the snort rule
  lpVoid      the compile result

Returns:      nothing

*/

void CALLBACK Process(const CSnortRule &rule, LPVOID lpVoid)
{
	CResNew &result = *(CResNew*)lpVoid;
	size_t nFlag = rule.GetFlag();
	size_t nNewSize = result.GetSidDfaIds().Size() + 1;
	result.GetSidDfaIds().Resize(nNewSize);
	COMPILEDRULENEW &ruleResult = result.GetSidDfaIds().Back();
	ruleResult.m_nSid = rule.GetSid();
	if (rule.Size() == 0)
	{
		ruleResult.m_nResult = COMPILEDRULENEW::RES_EMPTY;
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		ruleResult.m_nResult = COMPILEDRULENEW::RES_HASNOT;
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		ruleResult.m_nResult = COMPILEDRULENEW::RES_HASBYTE;
		return;
	}
	else
	{
		Rule2Dfas(rule, result, ruleResult);
	}
}

/* complie all rules

Arguments:
  filename    path of the file where all rules are saved in
  result      the compile result

Returns:      nothing

*/

COMPILERNEW void compilenew(LPCTSTR filename, CResNew &result)
{
	CompileRuleSet(filename, Process, &result);

	std::cout << "rule2pcretime:" << rule2pcretime << std::endl;
	std::cout << "pcre2nfatime:" << pcre2nfatime << std::endl;
	std::cout << "nfa2dfatime:" << nfa2dfatime << std::endl;
	std::cout << "dfamintimetime:" << dfamintimetime << std::endl;
}