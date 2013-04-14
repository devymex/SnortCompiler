#pragma once

#include <vector>

#define CHARSETSIZE 260


#ifndef COMMON_H_
#define COMMONSC __declspec(dllimport)
#else
#define COMMONSC __declspec(dllexport)
#endif

class COMMONSC CStateSet
{
public:
	CStateSet();
	~CStateSet();
	CStateSet(const CStateSet &other);
	const CStateSet& operator = (const CStateSet &other);
	size_t& operator[](size_t nIdx);
	const size_t& operator[](size_t nIdx) const;
	bool operator == (const CStateSet &other);

	const size_t Size() const;
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

class COMMONSC CNfaRow
{
public:
	CNfaRow();
	~CNfaRow();
	CNfaRow(const CNfaRow &other);
	CNfaRow& operator=(const CNfaRow &other);
	size_t Size(size_t nChar);
	void GetSet(size_t nChar, size_t *pSet);

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
	void reserve(size_t _Count);
	void resize(size_t _Newsize);
	size_t size() const;
	void push_back(const CNfaRow &row);

	CNfaRow &back();
	CNfaRow &operator[](size_t index);
	const CNfaRow &operator[](size_t index) const;
	void pop_back();
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
	void SetnFlag(unsigned long flag);
	unsigned long GetnFlag();
private:
	unsigned long nFlag;
	size_t m_pDest[CHARSETSIZE];
};

class COMMONSC CDfa
{
public:
	CDfa();
	~CDfa();
	CDfa(const CDfa &other);
	CDfa& operator=(const CDfa &other);
	void reserve(size_t _Count);
	void resize(size_t _Newsize);
	size_t size() const;
	CDfaRow &back();
	CDfaRow& operator[](size_t index);
private:
	std::vector<CDfaRow> *m_pDfa;
};

class COMMONSC CNfaChain
{
public:
	CNfaChain();
	~CNfaChain();
	CNfaChain(const CNfaChain &other);
	const CNfaChain& operator = (const CNfaChain &other);

	size_t Size() const;
	void PushBack(CNfa &cnfa);
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

enum OPTIONCONTENTFLAGS
{
	CF_NOCASE		= (1 << 0),//1
	CF_OFFSET		= (1 << 1),//2
	CF_DEPTH		= (1 << 2),//4
	CF_DISTANCE		= (1 << 3),//8
	CF_WITHIN		= (1 << 4),//16
};

enum PCREFLAGS
{
	PF_i = (1 << 0),
	PF_s = (1 << 1),
	PF_m = (1 << 2),
	PF_x = (1 << 3),
	PF_A = (1 << 4),
	PF_E = (1 << 5),
	PF_G = (1 << 6),
	PF_R = (1 << 7),
	PF_U = (1 << 8),
	PF_B = (1 << 9),
	PF_P = (1 << 10),
	PF_H = (1 << 11),
	PF_M = (1 << 12),
	PF_C = (1 << 13),
	PF_O = (1 << 14),
	PF_I = (1 << 15),
	PF_D = (1 << 16),
	PF_K = (1 << 17),
	PF_S = (1 << 18),
	PF_Y = (1 << 19)
};

class COMMONSC CSnortRule
{
public:
	CSnortRule();
	CSnortRule(const CSnortRule &other);
	~CSnortRule();
	void SetSid(size_t sid);
	void PushBack(RULEOPTION* ruleoption);
	void PopBack();
	size_t Size() const;
private:
	size_t m_nSid;
	std::vector<RULEOPTION*> *m_pOptions;
};


