/**
**  @file        common.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CDllArray, CDllString
**
*/

#pragma once

#include <windows.h>
#include <vector>
#include <hwprj\common.h>

#ifndef UNSARYHDR_DS
#define UNSARYHDR __declspec(dllimport)
#else
#define UNSARYHDR __declspec(dllexport)
#endif

/* Capsulate the std::vector<ULONG> for dll using. */
class UNSARYHDR CDllArray
{
public:
	// CDCA
	CDllArray();
	~CDllArray();
	CDllArray(const CDllArray &other);
	const CDllArray& operator = (const CDllArray &other);

	// Overided operators
	ULONG& operator[](ULONG nIdx);
	const ULONG& operator[](ULONG nIdx) const;
	bool operator == (const CDllArray &other);

	// Access member
	void Clear();
	ULONG Size() const;
	void PopBack();
	void PushBack(ULONG nState);
	void Reserve(ULONG nCount);
	void Resize(ULONG nSize);
	ULONG& Back();

	// Algorithms
	void Sort();
	void Unique();
	void Fill(ULONG _Val);

protected:
	std::vector<ULONG> *m_pSet;
};
