/**
**  @file        nfarow.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Declaration of CNfaRow
**
** Store a row for CNfa. Array of std::vector<ulong>. Each element of the
** array stand by a column that indicates the jumping character.
** Each number in vector is the next state would be jump to by the character.
**
*/

#pragma once

#include <hwprj\common.h>

#ifndef NFAHDR_DS
class ULONGVEC;
#define NFAHDR __declspec(dllimport)
#else
#define NFAHDR __declspec(dllexport)
#endif

class NFAHDR CNfaRow
{
public:
	explicit CNfaRow(ulong nSize = SC_CHARSETSIZE);
	CNfaRow(const CNfaRow &other);
	virtual ~CNfaRow();

	CNfaRow& operator=(const CNfaRow &other);

	void			Resize(ulong nSize);
	ulong			Size() const;
	ulong			DestCnt(ulong nCol) const;
	ulong&			GetDest(ulong nCol, ulong nIdx);
	const ulong&	GetDest(ulong nCol, ulong nIdx) const;
	ulong*			GetCol(ulong nCol);
	const ulong*	GetCol(ulong nCol) const;
	void			CopyCol(ulong nCol, ulong *pOut) const;
	void			AddDest(ulong nCol, ulong nDest);
	void			SortAll();


protected:
	ulong m_nSize;
	ULONGVEC *m_pDestSet;
};
