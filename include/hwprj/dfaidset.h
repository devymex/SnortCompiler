/*
**	@file		dfaidset.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CDfaIdSet class
**
**	Store the corresponding ID for each CDfa
**
*/

#pragma once

#include <hwprj\unsary.h>

#ifndef DFAIDSETHDR_DS
class DFAIDSET;
#define DFAIDSETHDR __declspec(dllimport)
#else
#define DFAIDSETHDR __declspec(dllexport)
#endif

class DFAIDSETHDR CDfaIdSet
{
public:
	CDfaIdSet();
	CDfaIdSet(const CDfaIdSet &other);
	virtual ~CDfaIdSet();

	const CDfaIdSet&	operator =	(const CDfaIdSet &other);
	bool				operator == (const CDfaIdSet &other) const;
	bool				operator <	(const CDfaIdSet &other) const;

	ulong	Size() const;
	void	Clear();
	void	Append(const CDfaIdSet &other);
	void	Add(DFAID dfaId);
	void	CopyTo(CUnsignedArray &idAry) const;
private:
	DFAIDSET *m_pSet;
};

