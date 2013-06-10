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


#include <hwprj\common.h>

#ifndef NFAHDR_DS
#define NFAHDR __declspec(dllimport)
#else
#define NFAHDR __declspec(dllexport)
#endif

/*
* Store a row for CNfa. Array of std::vector<ULONG>. Each element of the
* array stand by a column that indicates the jumping character.
* Each number in vector is the next state would be jump to by the character.
*/
class NFAHDR CNfaRow
{
public:
	explicit CNfaRow(ULONG nSize = CHARSETSIZE);
	~CNfaRow();
	CNfaRow(const CNfaRow &other);
	CNfaRow& operator=(const CNfaRow &other);

	void Resize(ULONG nSize);
	ULONG Size() const;
	ULONG DestCnt(ULONG nCol) const;
	ULONG& GetDest(ULONG nCol, ULONG nIdx);
	const ULONG& GetDest(ULONG nCol, ULONG nIdx) const;
	ULONG* GetCol(ULONG nCol);
	const ULONG* GetCol(ULONG nCol) const;
	void CopyCol(ULONG nCol, ULONG *pOut) const;
	void AddDest(ULONG nCol, ULONG nDest);
	void SortAll();

protected:
	ULONG m_nSize;
	std::vector<ULONG> *m_pDestSet;
};
