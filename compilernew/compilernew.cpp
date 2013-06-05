#include "stdafx.h"
#include "compilernew.h"
#include "../common/common.h"
#include "../rule2nfa/rule2nfa.h"
#include "../pcre2nfa/pcre2nfa.h"
#include "../dfanew/dfanew.h"

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

//COMPILERNEW CDFASINFO::CDFASINFO()
//{
//	m_pDfas = new std::vector<DFAINFO>;
//}
//
//COMPILERNEW CDFASINFO::CDFASINFO(const CDFASINFO& other)
//{
//	m_pDfas = new std::vector<DFAINFO>;
//	*this = other;
//}
//
//COMPILERNEW const CDFASINFO &CDFASINFO::operator=(const CDFASINFO &other)
//{
//	*m_pDfas = *other.m_pDfas;
//	return *this;
//}
//
//COMPILERNEW CDFASINFO::~CDFASINFO()
//{
//	delete m_pDfas;
//}
//
//COMPILERNEW DFAINFO& CDFASINFO::operator[](size_t index)
//{
//	return (*m_pDfas)[index];
//}
//
//COMPILERNEW const DFAINFO& CDFASINFO::operator[](size_t index) const
//{
//	return (*m_pDfas)[index];
//}
//
//COMPILERNEW void CDFASINFO::Reserve(size_t nCount)
//{
//	m_pDfas->reserve(nCount);
//}
//
//COMPILERNEW void CDFASINFO::Resize(size_t nSize)
//{
//	m_pDfas->resize(nSize);
//}
//
//COMPILERNEW const size_t CDFASINFO::Size() const
//{
//	return m_pDfas->size();
//}
//
//COMPILERNEW void CDFASINFO::PushBack(const DFAINFO &dfaDetail)
//{
//	m_pDfas->push_back(dfaDetail);
//}
//
//COMPILERNEW DFAINFO& CDFASINFO::Back()
//{
//	return m_pDfas->back();
//}

COMPILERNEW CDfaTblNew& CResNew::GetDfaTable()
{
	return m_dfaTbl;
}

COMPILERNEW CSidDfaIdsNew& CResNew::GetSidDfaIds()
{
	return m_sidDfaIds;
}

//COMPILERNEW CDFASINFO &CResNew::GetDfasInfo()
//{
//	return m_DfasInfo;
//}

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

//COMPILERNEW const CDFASINFO &CResNew::GetDfasInfo() const
//{
//	return m_DfasInfo;
//}

COMPILERNEW const CRegRule &CResNew::GetRegexTbl() const
{
	return m_RegexTbl;
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
	WriteNum(fout, m_sidDfaIds.Size());
	//记录规则偏移填写位置
	std::streamoff ruleOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);
	//DFAs数量
	WriteNum(fout, m_dfaTbl.Size());
	//记录DFAs偏移填写位置
	std::streamoff dfaOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);
	////DfasInfo大小
	//WriteNum(fout, m_DfasInfo.Size());
	////记录DfasInfo偏移填写位置
	//std::streamoff dfasInfoOffsetPos = fout.tellp();
	//fout.seekp(4, std::ios_base::cur);
	//RegexTbl大小
	WriteNum(fout, m_RegexTbl.Size());
	//记录RegexTbl偏移填写位置
	std::streamoff RegexTblOffsetPos = fout.tellp();
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
		WriteNum(fout, ruleResult.m_nSid);
		WriteNum(fout, ruleResult.m_nResult);
		WriteNum(fout, ruleResult.m_dfaIds.Size());
		for (size_t j = 0; j < ruleResult.m_dfaIds.Size(); ++j)
		{
			WriteNum(fout, ruleResult.m_dfaIds[j]);
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
	////填写DfasInfo偏移
	//endPos = fout.tellp();
	//fout.seekp(dfasInfoOffsetPos, std::ios_base::beg);
	//WriteNum(fout, endPos, 4);
	////定位文件到末尾
	//fout.seekp(0, std::ios_base::end);
	//for (size_t i = 0; i < m_DfasInfo.Size(); ++i)
	//{
	//	WriteNum(fout, m_DfasInfo[i].dfaId);
	//	WriteNum(fout, m_DfasInfo[i].chainId);
	//}
	//填写RegexTbl偏移
	endPos = fout.tellp();
	fout.seekp(RegexTblOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);
	//定位文件到末尾
	fout.seekp(0, std::ios_base::end);
	//写RegexTbl
	for (size_t i = 0; i < m_RegexTbl.Size(); ++i)
	{
		WriteNum(fout, m_RegexTbl[i].Size());
		for (size_t j = 0; j < m_RegexTbl[i].Size(); ++j)
		{
			WriteNum(fout, m_RegexTbl[i][j].Size());
			const char *pString = m_RegexTbl[i][j].C_Str();
			fout.write(pString, strlen(pString));
		}
		WriteNum(fout, m_RegexTbl[i].GetSigCnt());
		for (size_t j = 0; j < m_RegexTbl[i].GetSigCnt(); ++j)
		{
			WriteNum(fout, m_RegexTbl[i].GetSig(j));
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
	////读DfasInfo大小
	//size_t dfasInfoSize;
	//fin.read((char*)&dfasInfoSize, 4);
	////跳过DfasInfo偏移
	//fin.seekg(4, std::ios_base::cur);
	//RegexTbl大小
	size_t RegexTblSize;
	fin.read((char*)&RegexTblSize, 4);
	//跳过RegexTbl偏移
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
	////开始读DfasInfo
	//m_DfasInfo.Resize(dfasInfoSize);
	//for (size_t i = 0; i < dfasInfoSize; ++i)
	//{
	//	fin.read((char*)&m_DfasInfo[i].dfaId, 4);
	//	fin.read((char*)&m_DfasInfo[i].chainId, 4);
	//}
	//写RegexTbl
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
			m_RegexTbl[i].PushBackSig(Sig);
		}
	}
	fin.close();
	fin.clear();

	return 0;
}

void AssignSig(CResNew &result, size_t BegIdx, size_t EndIdx)
{
	std::vector<SIGNATURE> vecRuleSigs; //本条规则的所有Signatrue
	for (size_t i = BegIdx; i < EndIdx; ++i)
	{
		for (size_t j = 0; j < result.GetRegexTbl()[i].GetSigCnt(); ++j)
		{
			if (std::find(vecRuleSigs.begin(), vecRuleSigs.end(), result.GetRegexTbl()[i].GetSig(j)) == vecRuleSigs.end())
			{
				vecRuleSigs.push_back(result.GetRegexTbl()[i].GetSig(j));
			}
		}
	}
	for (size_t i = BegIdx; i < EndIdx; ++i)
	{
		//if (result.GetRegexTbl()[i].GetSigCnt() == 0)
		//{
		//	for (size_t j = 0; j < vecRuleSigs.size(); ++j)
		//	{
		//		result.GetRegexTbl()[i].PushBackSig(vecRuleSigs[j]);
		//	}
		//}
		result.GetRegexTbl()[i].ClearSigList();
		for (size_t j = 0; j < vecRuleSigs.size(); ++j)
		{
			result.GetRegexTbl()[i].PushBackSig(vecRuleSigs[j]);
		}
	}
}

double rule2pcretime = 0.0;
double pcre2nfatime = 0.0;
double nfa2dfatime = 0.0;
double dfamintimetime = 0.0;

void Rule2Dfas(const CSnortRule &rule, CResNew &result, COMPILEDRULENEW &ruleResult)
{
	CRegRule regrule;

	CTimer ctime;//用于测试
	ctime.Reset();//用于测试
	size_t flag = Rule2PcreList(rule, regrule);//从content中取出sig了
	rule2pcretime += ctime.Reset();//用于测试

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
		//const size_t nDfasInfoSize = result.GetDfasInfo().Size();
		//result.GetDfasInfo().Resize(nDfasInfoSize + nIncrement);
		const size_t nRegexTblSize = result.GetRegexTbl().Size();
		result.GetRegexTbl().Resize(nRegexTblSize + nIncrement);
		size_t nDfaId;
		//size_t nDfasInfoId;
		size_t nChainId;
		bool bHasSigs = false;
		for (size_t i = 0; i < nIncrement; ++i)
		{
			CNfa nfa;

			ctime.Reset();//用于测试
			size_t nToNFAFlag = CRegChainToNFA(regrule[i], nfa);
			pcre2nfatime += ctime.Reset();//用于测试

			if (regrule[i].GetSigCnt() > 0)
			{
				bHasSigs = true;
			}

			nDfaId = nDfaTblSize + i;
			//nDfasInfoId = nDfasInfoSize + i;
			nChainId = nRegexTblSize + i;
			CDfaNew &dfa = result.GetDfaTable()[nDfaId];
			if (nToNFAFlag == SC_ERROR)
			{
				ruleResult.m_nResult = COMPILEDRULENEW::RES_ERROR;
				ruleResult.m_dfaIds.Clear();
				result.GetDfaTable().Resize(nDfaTblSize);
				//result.GetDfasInfo().Resize(nDfasInfoSize);
				result.GetRegexTbl().Resize(nRegexTblSize);
				return;
			}
			else if (nToNFAFlag == SC_EXCEED)
			{
				ruleResult.m_nResult = COMPILEDRULENEW::RES_EXCEED;
			}
			else
			{
				ctime.Reset();//用于测试
				dfa.SetId(nDfaId);
				size_t nToDFAFlag = dfa.FromNFA(nfa, NULL, 0);
				//				std::cout << "  "<< dfa.LinkSize() << ", " << dfa.GetGroupCount() * dfa.Size() << std::endl;
				nfa2dfatime += ctime.Reset();//用于测试
				//outPutDfa(dfa, "..//..//output//test.txt");

				if (nToDFAFlag == -1)
				{
					ruleResult.m_nResult = COMPILEDRULENEW::RES_EXCEEDLIMIT;
					dfa.Clear();
				}
				else
				{
					ctime.Reset();//用于测试
					//std::cout << dfa.GetTermCnt() << std::endl;
					size_t nr = dfa.Minimize();
					//std::cout << dfa.GetTermCnt() << std::endl;
					dfamintimetime += ctime.Reset();//用于测试
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
				//result.GetDfasInfo().Resize(nDfasInfoSize);
				result.GetRegexTbl().Resize(nRegexTblSize);
				return;
			}
			ruleResult.m_dfaIds.PushBack(nDfaId);
			//result.GetDfasInfo()[nDfasInfoId].dfaId = nDfaId;
			//result.GetDfasInfo()[nDfasInfoId].chainId = nChainId;
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
	//else if (nFlag & CSnortRule::RULE_HASNOSIG)
	//{
	//	ruleResult.m_nResult = COMPILEDRULENEW::RES_HASNOSIG;
	//	return;
	//}
	else
	{
		Rule2Dfas(rule, result, ruleResult);
	}
}

COMPILERNEW void compilenew(LPCTSTR filename, CResNew &result)
{
	CompileRuleSet(filename, Process, &result);

	std::cout << "rule2pcretime:" << rule2pcretime << std::endl;
	std::cout << "pcre2nfatime:" << pcre2nfatime << std::endl;
	std::cout << "nfa2dfatime:" << nfa2dfatime << std::endl;
	std::cout << "dfamintimetime:" << dfamintimetime << std::endl;
}