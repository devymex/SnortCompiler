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
#include <hwprj\nfarow.h>

#ifndef NFAHDR_DS
class NFAROWARY;
#endif

/* Store one nfa in table format. Each row of the table is a CNfaRow */
class NFAHDR CNfa
{
public:
	CNfa();
	~CNfa();
	CNfa(const CNfa &other);
	CNfa& operator=(const CNfa &other);

	CNfaRow &operator[](ulong index);
	const CNfaRow &operator[](ulong index) const;

	ulong Size() const;
	void Clear();
	void Resize(ulong _Newsize);
	void Reserve(ulong _Count);
	void Shrink();

	CNfaRow &Back();
	void PushBack(const CNfaRow &row);
	void PopBack();

	void SortAll();

	void Dump(const char *pFile) const;

protected:
	NFAROWARY *m_pNfa;
};
