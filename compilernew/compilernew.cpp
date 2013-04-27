#include "stdafx.h"
#include "compilernew.h"
#include "../common/common.h"
#include "../rule2nfa/rule2nfa.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../dfanew/dfanew.h"

COMPILERNEW CDfaTblNew::CDfaTblNew()
{
	m_pdfaTbl = new std::vector<CDfanew>;	
}

COMPILERNEW CDfaTblNew::CDfaTblNew(const CDfaTblNew& other)
{
	m_pdfaTbl = new std::vector<CDfanew>;
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

COMPILERNEW CDfanew& CDfaTblNew::operator[](size_t index)
{
	return (*m_pdfaTbl)[index];
}

COMPILERNEW const CDfanew& CDfaTblNew::operator[](size_t index) const
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

COMPILERNEW void CDfaTblNew::PushBack(const CDfanew &dfa)
{
	m_pdfaTbl->push_back(dfa);
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

COMPILERNEW const CDfaTblNew& CResNew::GetDfaTable() const
{
	return m_dfaTbl;
}

COMPILERNEW const CSidDfaIdsNew& CResNew::GetSidDfaIds() const
{
	return m_sidDfaIds;
}

template<typename _Ty>
void WriteNum(std::ofstream &fout, _Ty _num, size_t nBytes = sizeof(_Ty))
{
	fout.write((char*)&_num, nBytes);
}

COMPILERNEW size_t CResNew::WriteToFile(LPCTSTR filename)
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
	//填写文件尺寸
	endPos = fout.tellp();
	fout.seekp(fileSizePos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);
	fout.seekp(0, std::ios_base::end);
	fout.close();
	fout.clear();

	return 0;
}

COMPILERNEW size_t CResNew::ReadFromFile(LPCTSTR filename)
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
	fin.close();
	fin.clear();

	return 0;
}

double dAll;
double dInterpretRule;
double dnfatodfa;
double dmin;

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
	else if (nFlag & CSnortRule::RULE_HASNOSIG)
	{
		ruleResult.m_nResult = COMPILEDRULENEW::RES_HASNOSIG;
		return;
	}
	else
	{
		CNfaTree nfatree;
		CTimer t;
		//size_t flag = InterpretRule(rule, nfatree);
		CRegRule regrule;
		size_t flag = Rule2PcreList(rule, regrule);
		if(flag == 0)
		{
			for(size_t k = 0; k < regrule.Size(); ++k)
			{
				CNfa nfa;
				size_t nFlag = CRegChainToNFA(regrule[k], nfa);
			}
		}

		dInterpretRule += t.Reset();

		if (flag == SC_ERROR)
		{
			ruleResult.m_nResult = COMPILEDRULENEW::RES_ERROR;
			return;
		}
		else if (flag == SC_EXCEED)
		{
			ruleResult.m_nResult = COMPILEDRULENEW::RES_EXCEED;
			return;
		}
		else
		{
			ruleResult.m_nResult = COMPILEDRULENEW::RES_SUCCESS;
			const size_t nCursize = result.GetDfaTable().Size();
			const size_t nIncrement = nfatree.Size();
			result.GetDfaTable().Resize(nCursize + nIncrement);
			size_t nId;
			for (size_t i = 0; i < nIncrement; ++i)
			{
				nId = nCursize + i;
				CDfanew &dfa = result.GetDfaTable()[nId];
				t.Reset();
				dfa.FromNFA(nfatree[i], NULL, 0);
				dnfatodfa += t.Reset();
				if (dfa.Size() > SC_STATELIMIT)
				{
					ruleResult.m_nResult = COMPILEDRULENEW::RES_EXCEEDLIMIT;
					dfa.Clear();
					//ruleResult.m_dfaIds.Clear();
					//result.GetDfaTable().Resize(nCursize);
					//break;
				}
				else
				{
					t.Reset();
					dfa.Minimize();
					dmin += t.Reset();
				}
				dfa.SetId(nId);
				ruleResult.m_dfaIds.PushBack(nId);
			}
		}
	}
}

COMPILERNEW void compilenew(LPCTSTR filename, CResNew &result)
{
	CTimer t;
	CompileRuleSet(filename, Process, &result);
	dAll += t.Reset();
}