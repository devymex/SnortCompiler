/**
**	@file		dllstring.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Common string class
**
**	Definiation of the CDllString class
**
*/

#pragma once

#include <string>
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
	explicit CDllString(const char *pStr);
	virtual ~CDllString();

	CDllString&		operator =	(const CDllString &other);
	char			operator[]	(ulong nIdx) const;

	// Overided operators

	const ulong		Size() const;
	bool			Empty();
	void			Clear();
	void			Append(const char* pChar);
	char			Back() const;
	void			PushBack(const char nChar);
	const char*		GetStr() const;

protected:
	DLLSTRING *m_pString;
};
