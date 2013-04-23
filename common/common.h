#pragma once

#include <Windows.h>
#include <vector>

#define CHARSETSIZE 260
#define EMPTYEDGE 256
#define DFACOLSIZE 256

#ifndef COMMON_H_
#define COMMONSC __declspec(dllimport)
#else
#define COMMONSC __declspec(dllexport)
#endif

typedef BYTE STATEID;

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

class COMMONSC CNfaRow
{
public:
	CNfaRow();
	~CNfaRow();
	CNfaRow(const CNfaRow &other);
	CNfaRow& operator=(const CNfaRow &other);

	CStateSet& operator[](size_t nChar);
	const CStateSet& operator[](size_t nChar) const;
private:
	CStateSet m_pDestSet[CHARSETSIZE];
};

//class COMMONSC CDfaRow
//{
//public:
//	enum STATEFLAG
//	{
//		NORMAL   = 1 << 0,
//		START    = 1 << 1,
//		TERMINAL = 1 << 2
//	};
//	CDfaRow();
//	~CDfaRow();
//	CDfaRow(const CDfaRow &other);
//	CDfaRow& operator=(const CDfaRow &other);
//	size_t& operator[](size_t index);
//	const size_t& operator[](size_t index) const;
//	void SetFlag(size_t nFlag);
//	size_t GetFlag();
//private:
//	size_t m_nFlag;
//	size_t m_pDest[CHARSETSIZE];
//};

class COMMONSC CDfaRow
{
public:
	enum STATEFLAG
	{
		NORMAL   = 1 << 0,
		START    = 1 << 1,
		TERMINAL = 1 << 2
	};
	CDfaRow(size_t col);
	void Fill(STATEID _Val);
	~CDfaRow();
	CDfaRow(const CDfaRow &other);
	CDfaRow& operator=(const CDfaRow &other);
	STATEID& operator[](BYTE index);
	const STATEID& operator[](BYTE index) const;
	void SetFlag(size_t nFlag);
	size_t GetFlag() const;
	size_t GetColNum() const;
private:
	size_t m_nFlag;
	size_t m_nColNum;
	std::vector<STATEID> *m_pDest;
};

class COMMONSC CDfa
{
public:
	CDfa();
	~CDfa();
	CDfa(const CDfa &other);
	CDfa& operator=(const CDfa &other);
	void Reserve(size_t _Count);
	void Resize(STATEID _Newsize);
	STATEID Size() const;
	CDfaRow &Back();
	CDfaRow& operator[](STATEID index);
	const CDfaRow& operator[](STATEID index) const;
	void Clear();
	size_t GetId();
	void SetId(size_t id);
	size_t GetColNum();
	void SetGroup(const BYTE *pGroup);
	BYTE GetGroup(size_t nIdx);
	const BYTE* GetGroup() const;
	STATEID GetStartId()const;
	void SetStartId(STATEID id);
private:
	size_t m_nId;
	size_t m_nColNum;
	STATEID m_StartId;
	BYTE m_pGroup[DFACOLSIZE];
	std::vector<CDfaRow> *m_pDfa;
	std::vector<std::pair<STATEID, size_t>> m_termSet;
};

class COMMONSC CNfa
{
public:
	CNfa();
	~CNfa();
	CNfa(const CNfa &other);
	CNfa& operator=(const CNfa &other);
	//size_t GetRowNum(void);
	void Reserve(size_t _Count);
	void Resize(size_t _Newsize);
	size_t Size() const;
	void FromDfa(CDfa &dfa);
	void PushBack(const CNfaRow &row);
	void PopBack();
	void SetPcre(const char* lpPcre);
	void PushDfaTerms(std::pair<size_t, size_t> pair);
	std::vector<std::pair<size_t, size_t>> GetDfaTerms();
	const char* GetPcre() const;

	CNfaRow &Back();
	CNfaRow &operator[](size_t index);
	const CNfaRow &operator[](size_t index) const;
private:
	//std::string *m_pRegex;
	std::vector<CNfaRow> *m_pNfa;
	std::string *m_pPcre;
	//如果该nfa是由dfa合并过程中生成的，该成员用于记录哪个状态能够识别哪个dfa终态
	std::vector<std::pair<size_t, size_t>> m_DfaTerms;
};

//class COMMONSC CDfa
//{
//public:
//	CDfa();
//	~CDfa();
//	CDfa(const CDfa &other);
//	CDfa& operator=(const CDfa &other);
//	void Reserve(size_t _Count);
//	void Resize(size_t _Newsize);
//	size_t Size() const;
//	CDfaRow &Back();
//	CDfaRow& operator[](size_t index);
//	const CDfaRow& operator[](size_t index) const;
//	void Clear();
//private:
//	std::vector<CDfaRow> *m_pDfa;
//};

//class COMMONSC CAndDfaRow : public CDfaRow
//{
//public:
//	enum DFAFLAG
//	{
//		NONE = 0,
//		FIRST = 1 << 0,
//		SECOND = 1 << 1,
//		ALL = 1 | 1 << 1
//	};
//	CAndDfaRow();
//	void SetDFlag(size_t cFlag);
//	char GetDFlag();
//private:
//	size_t m_cFlag;
//	size_t m_pAndDest[CHARSETSIZE];
//};


//class COMMONSC CAndDfa
//{
//public:
//	CAndDfa();
//	~CAndDfa();
//	CAndDfa(const CDfa &other);
//	CAndDfa& operator=(const CAndDfa &other);
//	void Reserve(size_t _Count);
//	void Resize(size_t _Newsize);
//	size_t Size() const;
//	CAndDfaRow &Back();
//	CAndDfaRow& operator[](size_t index);
//private:
//	std::vector<CAndDfaRow> *m_pAndDfa;
//};
//

//class COMMONSC CNfaChain
//{
//public:
//	CNfaChain();
//	~CNfaChain();
//	CNfaChain(const CNfaChain &other);
//	const CNfaChain& operator = (const CNfaChain &other);
//
//	size_t Size() const;
//	void Resize(size_t nSize);
//	CNfa& Back();
//	void PushBack(const CNfa &cnfa);
//	CNfa& operator[](size_t nIdx);
//	const CNfa& operator[](size_t nIdx) const;
//private:
//	std::vector<CNfa> *m_pChain;
//};


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
	//std::vector<CNfaChain> *m_pTree;
	std::vector<CNfa> *m_pTree;
};

class COMMONSC CRegChain
{
public:
	CRegChain();
	~CRegChain();
	CRegChain(const CRegChain &other);
	size_t Size() const;
	std::string& Back() const;
	void PushBack(std::string &pcreStr);
	std::string& operator[](size_t nIdx);
	const CRegChain& operator = (const CRegChain &other);
private:
	std::vector<std::string> *m_pRegList;
};

class COMMONSC CRegRule
{
public:
	CRegRule();
	~CRegRule();
	CRegRule(const CRegRule &other);

	size_t Size() const;
	std::string& Back() const;
	void PushBack(std::string &pcreStr);
	std::string& operator[](size_t nIdx);
	const CRegChain& operator = (const CRegChain &other);
private:
	std::vector<std::string> *m_pRegList;
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
