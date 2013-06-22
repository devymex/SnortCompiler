
/*
**	@file		dllstring.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Common string class
**
**	Definition of the CDllString class
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\dllstring.h>

#ifndef DLLSTRHDR_DS
class DLLSTRINGARY;
#endif

/* Capsulate the std::string for dll using. */
class DLLSTRHDR CStringArray
{
public:
	// CDCA
	CStringArray();
	CStringArray(const CStringArray &other);
	virtual ~CStringArray();

	// Overided operators
	CStringArray&		operator =	(const CStringArray &other);
	CDllString&			operator[]	(ulong ulIdx);
	const CDllString&	operator[]	(ulong ulIdx) const;

	// Normal operations
	ulong				Size() const;
	void				Clear();
	void				PushBack(const CDllString &str);

protected:
	DLLSTRINGARY*		m_pStrAry;
};
