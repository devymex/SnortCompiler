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

bool ColumnEqual(const CDfa &dfa, size_t &c1, size_t &c2)
{
	for (size_t i = 0; i < dfa.Size(); ++i)
	{
		if (dfa[i][c1] != dfa[i][c2])
		{
			return false;
		}
	}

	return true;
}

void AvaiDfaEdges(const CDfa &dfa, std::vector<std::vector<size_t>> &charGroups)
{
	charGroups.clear();
	std::vector<size_t> fullSet;

	for (size_t i = 0; i < CHARSETSIZE - 4; ++i)
	{
		fullSet.push_back(i);
	}

	for (; !fullSet.empty();)
	{
		charGroups.push_back(std::vector<size_t>());
		std::vector<size_t> &curGroup = charGroups.back();
		curGroup.push_back(fullSet.front());
		fullSet.erase(fullSet.begin());
		for (std::vector<size_t>::iterator i = fullSet.begin(); i != fullSet.end() && !fullSet.empty();)
		{
			if (ColumnEqual(dfa, curGroup.front(), *i))
			{
				curGroup.push_back(*i);
				i = fullSet.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
}

COMPILER size_t CRes::WriteToFile(LPCTSTR filename)
{
	std::ofstream fout(filename, std::ios::binary);
	if (!fout)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (size_t)-1;
	}
	//��־��ΪDFAS
	std::string symbolStr = "DFAS";
	for (std::string::iterator i = symbolStr.begin(); i != symbolStr.end(); ++i)
	{
		WriteNum(fout, *i);
	}
	//��¼�ļ��ߴ���дλ��
	std::streamoff fileSizePos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);
	//��������
	WriteNum(fout, m_sidDfaIds.Size(), 4);
	//��¼����ƫ����дλ��
	std::streamoff ruleOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);
	//DFAs����
	WriteNum(fout, m_dfaTbl.Size(), 4);
	//��¼DFAsƫ����дλ��
	std::streamoff dfaOffsetPos = fout.tellp();
	fout.seekp(4, std::ios_base::cur);
	//��д����ƫ��
	std::streamoff endPos = fout.tellp();
	fout.seekp(ruleOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);
	//��λ�ļ���ĩβ
	fout.seekp(endPos, std::ios_base::beg);
	//��ʼд�����
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
	//��дDFAsƫ��
	endPos = fout.tellp();
	fout.seekp(dfaOffsetPos, std::ios_base::beg);
	WriteNum(fout, endPos, 4);
	//��λ�ļ���ĩβ
	fout.seekp(0, std::ios_base::end);
	//��ʼдDFAs
	////��¼Ԫ������ƫ��λ��
	//std::streamoff dfaElemNumPos;
	////��¼Ԫ������
	//size_t nCnt = 0;
	for (size_t i = 0; i < m_dfaTbl.Size(); ++i)
	{
		WriteNum(fout, m_dfaTbl[i].Size(), 4);
		//dfaElemNumPos = fout.tellp();
		//fout.seekp(4, std::ios_base::cur);
		//дDFA��ת��
		std::vector<std::vector<size_t>> charGroups;
		AvaiDfaEdges(m_dfaTbl[i], charGroups);
		//д����
		//д��������
		WriteNum(fout, charGroups.size(), 4);
		//ÿ������д���
		std::vector<size_t> colIds;
		for (size_t j = 0; j < charGroups.size(); ++j)
		{
			//дÿһ��ĸ���
			if (!charGroups[j].empty())
			{
				colIds.push_back(charGroups[j][0]);
			}
			WriteNum(fout, charGroups[j].size(), 4);
			for (size_t k = 0; k < charGroups[j].size(); ++k)
			{
				//д���е��к�
				WriteNum(fout, charGroups[j][k], 4);
			}
		}
		for (size_t j = 0; j < m_dfaTbl[i].Size(); ++j)
		{
			for (size_t k = 0; k < colIds.size(); ++k)
			{
				WriteNum(fout, m_dfaTbl[i][j][colIds[k]], 4);
			}
		}
	}
	//��д�ļ��ߴ�
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
	//�鿴��־���Ƿ�ΪDFAS
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
	//��ȡ�ļ��ߴ�
	size_t fileSize;
	fin.read((char*)&fileSize, 4);
	//��ȡ��������
	size_t ruleNum;
	fin.read((char*)&ruleNum, 4);
	//��������ƫ��
	fin.seekg(4, std::ios_base::cur);
	//DFAs����
	size_t dfaNum;
	fin.read((char*)&dfaNum, 4);
	//����DFAsƫ��
	fin.seekg(4, std::ios_base::cur);
	//��ʼ�������
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
	//��ʼ��DFAs
	m_dfaTbl.Resize(dfaNum);
	size_t dfaSize;//һ��DFA��״̬��
	size_t groupNum;//�������
	size_t groupSize;//һ�������������
	for (size_t i = 0; i < dfaNum; ++i)
	{
		CDfa &dfa = m_dfaTbl[i];
		fin.read((char*)&dfaSize, 4);
		dfa.Resize(dfaSize);
		//��DFA��ת��
		//������
		//����������
		fin.read((char*)&groupNum, 4);
		std::vector<std::vector<size_t>> charGroups(groupNum);//��ŷ������
		for (size_t j = 0; j < groupNum; ++j)
		{
			//��ÿһ��ĸ���
			fin.read((char*)&groupSize, 4);
			charGroups[j].resize(groupSize);
			for (size_t k = 0; k < groupSize; ++k)
			{
				//�����е��к�
				fin.read((char*)&charGroups[j][k], 4);
			}
		}
		for (size_t j = 0; j < dfaSize; ++j)
		{
			for (size_t k = 0; k < charGroups.size(); ++k)
			{
				fin.read((char*)&dfa[j][charGroups[k][0]], 4);
				for (size_t c = 1; c < charGroups[k].size(); ++c)
				{
					dfa[j][charGroups[k][c]] = dfa[j][charGroups[k][0]];
				}
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
	if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_HASBYTE;
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_HASNOT;
		return;
	}
	else if (rule.Size() == 0)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_EMPTY;
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
				CNfa nfa;
				SerializeNfa(nfatree[i], nfa);
				nId = nCursize + i;
				CDfa &dfa = result.GetDfaTable()[nId];
				NfaToDfa(nfa, dfa);
				if (dfa.Size() > SC_STATELIMIT)
				{
					dfa.Clear();
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