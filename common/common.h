#pragma once

#include <vector>

#define CHARSETSIZE 260
#define EMPTYEDGE 256

#ifndef COMMON_H_
#define COMMONSC __declspec(dllimport)
#else
#define COMMONSC __declspec(dllexport)
#endif

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
	bool Empty();
	void PopBack();
	void PushBack(size_t nState);
	void Reserve(size_t nCount);
	void Resize(size_t nSize);
	size_t& Back();
	void Sort();
	void Unique();
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

class COMMONSC CNfa
{
public:
	CNfa();
	~CNfa();
	CNfa(const CNfa &other);
	CNfa& operator=(const CNfa &other);
	size_t GetRowNum(void);
	void Reserve(size_t _Count);
	void Resize(size_t _Newsize);
	size_t Size() const;
	void PushBack(const CNfaRow &row);
	void PopBack();

	CNfaRow &Back();
	CNfaRow &operator[](size_t index);
	const CNfaRow &operator[](size_t index) const;
private:
	std::vector<CNfaRow> *m_pNfa;
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
	CDfaRow();
	~CDfaRow();
	CDfaRow(const CDfaRow &other);
	CDfaRow& operator=(const CDfaRow &other);
	size_t& operator[](size_t index);
	void SetFlag(size_t nFlag);
	size_t GetFlag();
private:
	size_t m_nFlag;
	size_t m_pDest[CHARSETSIZE];
};

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

class COMMONSC CDfa
{
public:
	CDfa();
	~CDfa();
	CDfa(const CDfa &other);
	CDfa& operator=(const CDfa &other);
	void Reserve(size_t _Count);
	void Resize(size_t _Newsize);
	size_t Size() const;
	bool Empty();
	CDfaRow &Back();
	CDfaRow& operator[](size_t index);
private:
	std::vector<CDfaRow> *m_pDfa;
};

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
class COMMONSC CNfaChain
{
public:
	CNfaChain();
	~CNfaChain();
	CNfaChain(const CNfaChain &other);
	const CNfaChain& operator = (const CNfaChain &other);

	size_t Size() const;
	CNfa& Back();
	void PushBack(const CNfa &cnfa);
	CNfa& operator[](size_t nIdx);
	const CNfa& operator[](size_t nIdx) const;
private:
	std::vector<CNfa> *m_pChain;
};

class COMMONSC CNfaTree
{
public:
	CNfaTree();
	~CNfaTree();
	CNfaTree(const CNfaTree &other);
	const CNfaTree& operator = (const CNfaTree &other);

	size_t Size() const;
	CNfaChain& Back();
	void PushBack(const CNfaChain &cnfachain);
	CNfaChain& operator[](size_t nIdx);
	const CNfaChain& operator[](size_t nIdx) const;

private:
	std::vector<CNfaChain> *m_pTree;
};

struct COMMONSC RULEOPTION
{
	virtual ~RULEOPTION();
	size_t nFlags;
};


class COMMONSC CSnortRule
{
public:
	enum {RULE_HASBYTE = 0x0001, RULE_HASNOT = 0x0002};
	CSnortRule();
	CSnortRule(const CSnortRule &other);
	~CSnortRule();
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
