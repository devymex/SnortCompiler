/*
**	@file		nfarow.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of CNfaRow
**
**	Store a row for CNfa. Array of std::vector<ulong>. Each element of the
**	array stand by a column that indicates the jumping character.
**	Each number in vector is the next state would be jump to by the character.
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\unsary.h>

#ifndef NFAHDR_DS
#define NFAHDR __declspec(dllimport)
#else
#define NFAHDR __declspec(dllexport)
#endif

class NFAHDR CNfaRow
{
public:
	static const ulong COLUMNCNT = 260;

	CNfaRow();
	CNfaRow(const CNfaRow &other);
	virtual ~CNfaRow();

	CNfaRow&				operator =	(const CNfaRow &other);
	CUnsignedArray&			operator []	(ulong ulCol);
	const CUnsignedArray&	operator []	(ulong ulCol) const;
	
	void			SortAll();

protected:
	CUnsignedArray *m_pDestSet;
};
