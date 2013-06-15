/*
**	@file		nfa.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CNfa class		
**
**	Functions declaration of nfa's essential algorithm
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

	CNfa&			operator =	(const CNfa &other);
	CNfaRow&		operator []	(ulong ulIdx);
	const CNfaRow&	operator []	(ulong ulIdx) const;

	ulong		Size() const;
	CNfaRow&	Back();
	void		Clear();
	void		Resize(ulong ulSize);
	void		Reserve(ulong ulSize);
	void		Shrink();
	void		PushBack(const CNfaRow &row);
	void		PopBack();
	void		SortAll();

	void		Dump(const char *pFile) const;

protected:
	NFAROWARY *m_pNfa;
};
