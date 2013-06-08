#pragma once

#include <Windows.h>
#include <vector>
#include <list>

#define CHARSETSIZE 260
#define EMPTYEDGE 256
#define DFACOLSIZE 256

#ifndef COMMON_H_
#define COMMONSC __declspec(dllimport)
#else
#define COMMONSC __declspec(dllexport)
#endif

double COMMONSC g_dTimer;

//typedef BYTE STATEID;
typedef WORD STATEID;
typedef std::list<STATEID> STALIST;
typedef std::list<STATEID>::iterator STALIST_ITER;
typedef std::vector<std::vector<STATEID>> SETVEC;
typedef std::list<std::list<STATEID>>::iterator SETLIST_ITER;
typedef DWORD SIGNATURE;

class COMMONSC CVectorNumber
{
public:
	CVectorNumber();
	~CVectorNumber();
	CVectorNumber(const CVectorNumber &other);
	const CVectorNumber& operator = (const CVectorNumber &other);
	size_t& operator[](size_t nIdx);
	const size_t& operator[](size_t nIdx) const;
	bool operator == (const CVectorNumber &other);

	const size_t Size() const;
	void PopBack();
	void PushBack(size_t nState);
	void Reserve(size_t nCount);
	void Resize(size_t nSize);
	size_t& Back();
	void Sort();
	void Unique();
	void Fill(size_t _Val);
	void Clear();
private:
	std::vector<size_t> *m_pSet;
};

typedef CVectorNumber CStateSet;


class COMMONSC CCString
{
public:
	CCString();
	explicit CCString(const char *pStr);
	~CCString();
	CCString(const CCString &other);
	CCString& operator = (const CCString &other);
	char operator[](size_t nIdx) const;
	void Append(const char* pChar);
	const char* GetString();
	const char* C_Str();

	const size_t Size() const;
	void PushBack(const char nChar);
	char Back() const;
	void Clear();
	bool Empty();
	
private:
	std::string *m_pString;
};

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

private:
	size_t m_nSize;
	std::vector<size_t> *m_pDestSet;
};

class COMMONSC CDfaRow
{
public:
	enum STATEFLAG
	{
		NORMAL   = 1 << 0,
		START    = 1 << 1,
		TERMINAL = 1 << 2
	};
	explicit CDfaRow(size_t col);
	void Fill(STATEID _Val);
	~CDfaRow();
	CDfaRow(const CDfaRow &other);
	CDfaRow& operator=(const CDfaRow &other);
	STATEID& operator[](STATEID index);
	const STATEID& operator[](STATEID index) const;
	void SetFlag(size_t nFlag);
	size_t GetFlag() const;
	size_t GetColNum() const;
private:
	size_t m_nFlag;//标记该状态/行的属性：NORMAL、START、TERMINAL
	size_t m_nColNum;
	std::vector<STATEID> *m_pDest;
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
private:
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

private:
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
private:
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
private:
	std::vector<CRegChain> *m_pRegVec;
};

struct COMMONSC RULEOPTION
{
	RULEOPTION();
	RULEOPTION(const RULEOPTION &other);
	const RULEOPTION& operator=(const RULEOPTION &other);
	virtual ~RULEOPTION();

	void SetPattern(LPCSTR lpStr);
	size_t GetPattern(LPSTR lpStr, size_t nLen) const;

	size_t nFlags;
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
	void PushBack(RULEOPTION* ruleoption);
	void PopBack();
	size_t Size() const;
	RULEOPTION* operator[](size_t nIdx) const;
private:
	size_t m_nSid;
	size_t m_nFlag;
	std::vector<RULEOPTION*> *m_pOptions;
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
private:
	__int64 m_nFreq;
	__int64 m_nStart;
};

COMMONSC void printNfa(const CNfa &nfa);
