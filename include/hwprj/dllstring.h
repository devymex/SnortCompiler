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

#ifndef DLLSTRHDR_DS
class DLLSTRING;
#define DLLSTRHDR __declspec(dllimport)
#else
#define DLLSTRHDR __declspec(dllexport)
#endif

/* Capsulate the std::string for dll using. */
class DLLSTRHDR CDllString
{
public:
	// CDCA
	CDllString();
	CDllString(const CDllString &other);
	explicit CDllString(pcstr pStr);
	virtual ~CDllString();

	CDllString&		operator =	(const CDllString &other);
	char			operator[]	(ulong nIdx) const;

	// Overided operators

	ulong	Size() const;
	bool	Empty() const;
	void	Clear();
	void	Append(const char* pChar);
	char	Back() const;
	void	PushBack(const char nChar);
	pcstr	GetStr() const;

protected:
	DLLSTRING *m_pString;
};
