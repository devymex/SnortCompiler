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

/* Ê¹ÓÃdll·â×°std::string*/
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

	ulong			Size() const;
	bool			Empty() const;
	void			Clear();
	void			Erase(ulong ulIdx);
	void			Insert(ulong ulIdx, char nChar);
	void			Insert(ulong ulIdx, pcstr pChar);
	void			Assign(const char* pStr);
	void			Append(const CDllString &other);
	char			Back() const;
	void			PushBack(char nChar);
	const char*		Data() const;

protected:
	DLLSTRING*		m_pString;
};
