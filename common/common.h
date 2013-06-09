/**
**  @file        common.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CVectorUnsigned, CCString
**
*/

#pragma once

#include <windows.h>
#include <vector>
#include <list>

#ifndef COMMON_H_
#define COMMONSC __declspec(dllimport)
#else
#define COMMONSC __declspec(dllexport)
#endif

#define CHARSETSIZE 260
#define EMPTYEDGE 256
#define DFACOLSIZE 256

/*
* Common type define
*/
//typedef unsigned char STATEID;
typedef unsigned short			STATEID;
typedef unsigned long			SIGNATURE;
typedef std::list<STATEID>		STATELIST;
typedef std::vector<STATEID>	STATEVEC;
typedef STATELIST::iterator		STATELIST_ITER;
typedef STATEVEC::iterator		STATEVEC_ITER;
typedef class CVectorUnsigned	CStateSet;

/*
* Capsulate the std::vector<size_t> for dll using.
*/
class COMMONSC CVectorUnsigned
{
public:
// CDCA
	CVectorUnsigned();
	~CVectorUnsigned();
	CVectorUnsigned(const CVectorUnsigned &other);
	const CVectorUnsigned& operator = (const CVectorUnsigned &other);

// Overided operators
	size_t& operator[](size_t nIdx);
	const size_t& operator[](size_t nIdx) const;
	bool operator == (const CVectorUnsigned &other);

// Access member
	void Clear();
	size_t Size() const;
	void PopBack();
	void PushBack(size_t nState);
	void Reserve(size_t nCount);
	void Resize(size_t nSize);
	size_t& Back();

// Algorithms
	void Sort();
	void Unique();
	void Fill(size_t _Val);

protected:
	std::vector<size_t> *m_pSet;
};

/*
* Capsulate the std::string for dll using.
*/
class COMMONSC CCString
{
public:
// CDCA
	CCString();
	explicit CCString(const char *pStr);
	~CCString();
	CCString(const CCString &other);
	CCString& operator = (const CCString &other);

// Overided operators
	char operator[](size_t nIdx) const;

	const size_t Size() const;
	bool Empty();
	void Clear();
	void Append(const char* pChar);
	char Back() const;
	void PushBack(const char nChar);
	const char* GetStr();
	
protected:
	std::string *m_pString;
};

/*
* Store a row for CNfa. Array of std::vector<size_t>. Each element of the
* array stand by a column that indicates the jumping character.
* Each number in vector is the next state would be jump to by the character.
*/
class COMMONSC CNfaRow
{
public:
	explicit CNfaRow(size_t nSize = CHARSETSIZE);
	~CNfaRow();
	CNfaRow(const CNfaRow &other);
	CNfaRow& operator=(const CNfaRow &other);

	void Resize(size_t nSize);
	size_t Size() const;
	size_t DestCnt(size_t nCol) const;
	size_t& GetDest(size_t nCol, size_t nIdx);
	const size_t& GetDest(size_t nCol, size_t nIdx) const;
	size_t* GetCol(size_t nCol);
	const size_t* GetCol(size_t nCol) const;
	void CopyCol(size_t nCol, size_t *pOut) const;
	void AddDest(size_t nCol, size_t nDest);
	void SortAll();

protected:
	size_t m_nSize;
	std::vector<size_t> *m_pDestSet;
};

class COMMONSC CNfa
{
public:
	struct DFATERMS
	{
		size_t nfaSta;
		size_t dfaId;
	};
	CNfa();
	~CNfa();
	CNfa(const CNfa &other);
	CNfa& operator=(const CNfa &other);
	void Reserve(size_t _Count);
	void Resize(size_t _Newsize);
	void Shrink();
	size_t Size() const;
	void PushBack(const CNfaRow &row);
	void PopBack();
	void SetPcre(const char* lpPcre);
	void PushDfaTerms(DFATERMS dfaTerms);
	size_t GetDfaTermsNum();
	DFATERMS GetDfaTerms(size_t num);
	const char* GetPcre() const;
	void Clear();
	void SortAll();

	CNfaRow &Back();
	CNfaRow &operator[](size_t index);
	const CNfaRow &operator[](size_t index) const;
protected:
	std::vector<CNfaRow> *m_pNfa;
	std::string *m_pPcre;
	//如果该nfa是由dfa合并过程中生成的，该成员用于记录哪个状态能够识别哪个dfa终态
	std::vector<DFATERMS> *m_DfaTerms;
};

class COMMONSC CNfaTree
{
public:
	CNfaTree();
	~CNfaTree();
	CNfaTree(const CNfaTree &other);
	const CNfaTree& operator = (const CNfaTree &other);

	size_t Size() const;
	void Reserve(size_t nCount);
	void Resize(size_t nSize);
	CNfa& Back();
	void PushBack(const CNfa &cnfachain);
	CNfa& operator[](size_t nIdx);
	const CNfa& operator[](size_t nIdx) const;

protected:
	std::vector<CNfa> *m_pTree;
};

class COMMONSC CRegChain
{
public:
	CRegChain();
	~CRegChain();
	CRegChain(const CRegChain &other);
	size_t Size() const;
	CCString& Back() const;
	void PushBack(CCString &pcreStr);
	CCString& operator[](size_t nIdx);
	const CRegChain& operator = (const CRegChain &other);
	void Resize(size_t nSize);
	size_t GetSigCnt() const;
	void PushBackSig(SIGNATURE &signature);
	SIGNATURE& GetSig(size_t nIdx) const;
	void Unique();
	void ClearSigList();
protected:
	std::vector<CCString> *m_pRegList;
	std::vector<SIGNATURE> *m_pSigList;
};

class COMMONSC CRegRule
{
public:
	CRegRule();
	~CRegRule();
	CRegRule(const CRegRule &other);

	size_t Size() const;
	CRegChain& Back() const;
	void Reserve(size_t nCount);
	void Resize(size_t nSize);
	void PushBack(CRegChain &nRegChain);
	CRegChain& operator[](size_t nIdx);
	const CRegChain& operator[](size_t nIdx) const;
	const CRegRule& operator = (const CRegRule &other);

protected:
	std::vector<CRegChain> *m_pRegVec;
};

class COMMONSC CRuleOption
{
public:
	CRuleOption();
	CRuleOption(const CRuleOption &other);
	const CRuleOption& operator=(const CRuleOption &other);
	virtual ~CRuleOption();

	void SetPattern(LPCSTR lpStr);
	size_t GetPattern(LPSTR lpStr, size_t nLen) const;

	size_t GetFlag() const;
	void SetFlag(size_t nFlag);
	void AddFlag(size_t nFlag);
	BOOL TestFlag(size_t nFlag) const;

protected:
	size_t m_nFlag;
	std::string *m_pPattern;
};


class COMMONSC CSnortRule
{
public:
	enum {RULE_HASBYTE = 0x0001, RULE_HASNOT = 0x0002, RULE_HASNOSIG = 0x0004};
	CSnortRule();
	CSnortRule(const CSnortRule &other);
	const CSnortRule& operator = (const CSnortRule &other);
	~CSnortRule();
	void Release();
	void SetSid(size_t sid);
	size_t GetSid() const;
	void SetFlag(size_t flag);
	size_t GetFlag() const;
	void PushBack(CRuleOption* ruleoption);
	void PopBack();
	size_t Size() const;
	CRuleOption* operator[](size_t nIdx) const;

protected:
	size_t m_nSid;
	size_t m_nFlag;
	std::vector<CRuleOption*> *m_pOptions;
};

class CTimer
{
public:
	__forceinline CTimer()
	{
		QueryPerformanceFrequency((PLARGE_INTEGER)&m_nFreq);
		QueryPerformanceCounter((PLARGE_INTEGER)&m_nStart);
	}
	__forceinline double Cur()
	{
		__int64 nCur;
		double dCur;

		QueryPerformanceCounter((PLARGE_INTEGER)&nCur);
		dCur = double(nCur - m_nStart) / double(m_nFreq);

		return dCur;
	}
	__forceinline double Reset()
	{
		__int64 nCur;
		double dCur;

		QueryPerformanceCounter((PLARGE_INTEGER)&nCur);
		dCur = double(nCur - m_nStart) / double(m_nFreq);
		m_nStart = nCur;

		return dCur;
	}
protected:
	__int64 m_nFreq;
	__int64 m_nStart;
};

COMMONSC void PrintNfaToText(CNfa &nfa, const char* fileName);//用于测试输出一个nfa
