#include "stdafx.h"
#include "compiler.h"
#include "../common/common.h"
#include "../rule2nfa/rule2nfa.h"
#include "../nfa2dfa/nfa2dfa.h"
#include "../pcre2nfa/pcre2nfa.h"

COMPILER CDfaTbl::CDfaTbl()
{
	m_dfaTbl = new std::vector<CDfa>;	
}

COMPILER CDfaTbl::CDfaTbl(const CDfaTbl& other)
{
	m_dfaTbl = new std::vector<CDfa>;
	*this = other;
}

COMPILER const CDfaTbl &CDfaTbl::operator=(const CDfaTbl &other)
{
	*m_dfaTbl = *other.m_dfaTbl;
	return *this;
}

COMPILER CDfaTbl::~CDfaTbl()
{
	delete m_dfaTbl;
}

COMPILER CDfa& CDfaTbl::operator[](size_t index)
{
	return (*m_dfaTbl)[index];
}

COMPILER const CDfa& CDfaTbl::operator[](size_t index) const
{
	return (*m_dfaTbl)[index];
}

COMPILER void CDfaTbl::Reserve(size_t nCount)
{
	m_dfaTbl->reserve(nCount);
}

COMPILER void CDfaTbl::Resize(size_t nSize)
{
	m_dfaTbl->resize(nSize);
}

COMPILER const size_t CDfaTbl::Size() const
{
	return m_dfaTbl->size();
}

COMPILER void CDfaTbl::PushBack(const CDfa &dfa)
{
	m_dfaTbl->push_back(dfa);
}

COMPILER CSidDfaIds::CSidDfaIds()
{
	m_ruleResult = new std::vector<COMPILEDRULE>;
}

COMPILER CSidDfaIds::CSidDfaIds(const CSidDfaIds& other)
{
	m_ruleResult = new std::vector<COMPILEDRULE>;
	*this = other;
}

COMPILER const CSidDfaIds &CSidDfaIds::operator=(const CSidDfaIds &other)
{
	*m_ruleResult = *other.m_ruleResult;
	return *this;
}
COMPILER CSidDfaIds::~CSidDfaIds()
{
	delete m_ruleResult;
}

COMPILER COMPILEDRULE& CSidDfaIds::operator[](size_t index)
{
	return (*m_ruleResult)[index];
}

COMPILER const COMPILEDRULE& CSidDfaIds::operator[](size_t index) const
{
	return (*m_ruleResult)[index];
}

COMPILER void CSidDfaIds::Reserve(size_t nCount)
{
	m_ruleResult->reserve(nCount);
}

COMPILER void CSidDfaIds::Resize(size_t nSize)
{
	m_ruleResult->resize(nSize);
}

COMPILER const size_t CSidDfaIds::Size() const
{
	return m_ruleResult->size();
}

COMPILER void CSidDfaIds::PushBack(const COMPILEDRULE &ruleResult)
{
	m_ruleResult->push_back(ruleResult);
}

COMPILER COMPILEDRULE& CSidDfaIds::Back()
{
	return m_ruleResult->back();
}

COMPILER CDfaTbl& CRes::GetDfaTable()
{
	return m_dfaTbl;
}

COMPILER CSidDfaIds& CRes::GetSidDfaIds()
{
	return m_sidDfaIds;
}

COMPILER const CDfaTbl& CRes::GetDfaTable() const
{
	return m_dfaTbl;
}

COMPILER const CSidDfaIds& CRes::GetSidDfaIds() const
{
	return m_sidDfaIds;
}

template<typename _Ty>
void WriteNum(std::ofstream &fout, _Ty _num, size_t nBytes = sizeof(_Ty))
{
	fout.write((char*)&_num, nBytes);
}

COMPILER size_t CRes::WriteToFile(LPCTSTR filename)
{
	std::ofstream fout(filename, std::ios::binary);
	if (!fout)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (size_t)-1;
	}
	//标志记为DFAS
	std::string symbolStr = "DFAS";
	for (std::string::iterator i = symbolStr.begin(); i != symbolStr.end(); ++i)
	{
		WriteNum(fout, *i);
	}
	//记录文件尺寸填写位置
	std::streamoff fileSizePos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);
	//规则数量
	WriteNum(fout, m_sidDfaIds.Size(), 4);
	//记录规则偏移填写位置
	std::streamoff ruleOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);
	//DFAs数量
	WriteNum(fout, m_dfaTbl.Size(), 4);
	//记录DFAs偏移填写位置
	std::streamoff dfaOffsetPos = fout.tellp();
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
		COMPILEDRULE &ruleResult = m_sidDfaIds[i];
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
	for (size_t i = 0; i < m_dfaTbl.Size(); ++i)
	{
		WriteNum(fout, m_dfaTbl[i].Size(), sizeof(STATEID));
		//写分组
		for (size_t j = 0; j < DFACOLSIZE; ++j)
		{
			WriteNum(fout, m_dfaTbl[i].GetGroup(j), sizeof(BYTE));
		}
		//写DFA跳转表
		size_t nColNum = m_dfaTbl[i].GetColNum();
		for (STATEID j = 0; j < m_dfaTbl[i].Size(); ++j)
		{
			for (size_t k = 0; k < nColNum; ++k)
			{
				WriteNum(fout, m_dfaTbl[i][j][(BYTE)k], sizeof(STATEID));
			}
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

COMPILER size_t CRes::ReadFromFile(LPCTSTR filename)
{
	std::ifstream fin(filename, std::ios::binary);
	if (!fin)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (size_t)-1;
	}
	//查看标志记是否为DFAS
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
	//开始读规则表
	m_sidDfaIds.Resize(ruleNum);
	size_t SidDfaNum;
	for (size_t i = 0; i < ruleNum; ++i)
	{
		COMPILEDRULE &ruleResult = m_sidDfaIds[i];
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
	STATEID dfaSize;//一个DFA的状态数
	for (size_t i = 0; i < dfaNum; ++i)
	{
		CDfa &dfa = m_dfaTbl[i];
		fin.read((char*)&dfaSize, sizeof(STATEID));
		//读分组
		BYTE pGroup[DFACOLSIZE];
		for (size_t j = 0; j < DFACOLSIZE; ++j)
		{
			fin.read((char*)&pGroup[j], sizeof(BYTE));
		}
		dfa.SetGroup(pGroup);
		dfa.Resize(dfaSize);
		//读DFA跳转表
		size_t nColNum = dfa.GetColNum();
		for (STATEID j = 0; j < dfaSize; ++j)
		{
			for (size_t k = 0; k < nColNum; ++k)
			{
				fin.read((char*)&dfa[j][(BYTE)k], sizeof(STATEID));
			}
		}
	}
	fin.close();
	fin.clear();

	return 0;
}

void CALLBACK Process(const CSnortRule &rule, LPVOID lpVoid)
{
	CRes &result = *(CRes*)lpVoid;
	size_t nFlag = rule.GetFlag();
	size_t nNewSize = result.GetSidDfaIds().Size() + 1;
	result.GetSidDfaIds().Resize(nNewSize);
	COMPILEDRULE &ruleResult = result.GetSidDfaIds().Back();
	ruleResult.m_nSid = rule.GetSid();
	if (rule.Size() == 0)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_EMPTY;
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_HASNOT;
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_HASBYTE;
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASNOSIG)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_HASNOSIG;
		return;
	}
	else
	{
		CNfaTree nfatree;
		size_t flag = InterpretRule(rule, nfatree);
		if (flag == SC_ERROR)
		{
			ruleResult.m_nResult = COMPILEDRULE::RES_ERROR;
			return;
		}
		else if (flag == SC_EXCEED)
		{
			ruleResult.m_nResult = COMPILEDRULE::RES_EXCEED;
			return;
		}
		else
		{
			ruleResult.m_nResult = COMPILEDRULE::RES_SUCCESS;
			const size_t nCursize = result.GetDfaTable().Size();
			const size_t nIncrement = nfatree.Size();
			result.GetDfaTable().Resize(nCursize + nIncrement);
			size_t nId;
			for (size_t i = 0; i < nIncrement; ++i)
			{
				nId = nCursize + i;
				CDfa &dfa = result.GetDfaTable()[nId];
				dfa.SetId(nId);
				NfaToDfa(nfatree[i], dfa);
				if (dfa.Size() > SC_STATELIMIT)
				{
					ruleResult.m_nResult = COMPILEDRULE::RES_EXCEEDLIMIT;
					ruleResult.m_dfaIds.Clear();
					result.GetDfaTable().Resize(nCursize);
					break;
				}
				ruleResult.m_dfaIds.PushBack(nId);
			}
		}
	}
}

COMPILER void compile(LPCTSTR filename, CRes &result)
{
	CompileRuleSet(filename, Process, &result);
}