/**
**  @file        common.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CUnsignedArray, CDllString
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
* Store a row for CNfa. Array of std::vector<ulong>. Each element of the
* array stand by a column that indicates the jumping character.
* Each number in vector is the next state would be jump to by the character.
*/
class NFAHDR CNfaRow
{
public:
	explicit CNfaRow(ulong nSize = SC_CHARSETSIZE);
	~CNfaRow();
	CNfaRow(const CNfaRow &other);
	CNfaRow& operator=(const CNfaRow &other);

	void Resize(ulong nSize);
	ulong Size() const;
	ulong DestCnt(ulong nCol) const;
	ulong& GetDest(ulong nCol, ulong nIdx);
	const ulong& GetDest(ulong nCol, ulong nIdx) const;
	ulong* GetCol(ulong nCol);
	const ulong* GetCol(ulong nCol) const;
	void CopyCol(ulong nCol, ulong *pOut) const;
	void AddDest(ulong nCol, ulong nDest);
	void SortAll();

protected:
	ulong m_nSize;
	std::vector<ulong> *m_pDestSet;
};
