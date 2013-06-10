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
#include <hwprj\nfarow.h>

/* Store one nfa in table format. Each row of the table is a CNfaRow */
class NFAHDR CNfa
{
public:
	CNfa();
	~CNfa();
	CNfa(const CNfa &other);
	CNfa& operator=(const CNfa &other);

	CNfaRow &operator[](ULONG index);
	const CNfaRow &operator[](ULONG index) const;

	ULONG Size() const;
	void Clear();
	void Resize(ULONG _Newsize);
	void Reserve(ULONG _Count);
	void Shrink();

	CNfaRow &Back();
	void PushBack(const CNfaRow &row);
	void PopBack();

	void SortAll();

	void Dump(const char *pFile) const;

protected:
	std::vector<CNfaRow> *m_pNfa;
};
