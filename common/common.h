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

/* compile configurations */
#define SC_STATELIMIT	500
#define DFA_SIZE_LIMIT	255
#define DFACOLSIZE		256
#define CHARSETSIZE		260


/* Common type defination */
typedef unsigned short			STATEID;
typedef unsigned long			SIGNATURE;
typedef std::list<STATEID>		STATELIST;
typedef std::vector<STATEID>	STATEVEC;
typedef STATELIST::iterator		STATELIST_ITER;
typedef STATEVEC::iterator		STATEVEC_ITER;
typedef class CVectorUnsigned	CStateSet;

/* Capsulate the std::vector<size_t> for dll using. */
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

class COMMONSC CSignatures
{
public:
	CSignatures();
	CSignatures(const CSignatures& other);
	const CSignatures &operator=(const CSignatures &other);
	~CSignatures();
	const size_t Size() const;
	void Resize(size_t nSize);
	void PushBack(SIGNATURE Sig);
	SIGNATURE &operator[](size_t nIdx);
	const SIGNATURE &operator[](size_t nIdx) const;
	void Clear();
	void Unique();
protected:
	std::vector<SIGNATURE> *m_pSigs;
};

/* Capsulate the std::string for dll using. */
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

/* Store one nfa in table format. Each row of the table is a CNfaRow */
class COMMONSC CNfa
{
public:
	CNfa();
	~CNfa();
	CNfa(const CNfa &other);
	CNfa& operator=(const CNfa &other);

	CNfaRow &operator[](size_t index);
	const CNfaRow &operator[](size_t index) const;

	size_t Size() const;
	void Clear();
	void Resize(size_t _Newsize);
	void Reserve(size_t _Count);
	void Shrink();

	CNfaRow &Back();
	void PushBack(const CNfaRow &row);
	void PopBack();

	void SortAll();

	void Dump(const char *pFile) const;

protected:
	std::vector<CNfaRow> *m_pNfa;
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
