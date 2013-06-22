/*
**	@file		byteary.h
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

#ifndef BYTEARYHDR_DS
class DLLBYTEARY;
#define BYTEARYHDR __declspec(dllimport)
#else
#define BYTEARYHDR __declspec(dllexport)
#endif

/* Ê¹ÓÃdll·â×°std::string */
class BYTEARYHDR CByteArray
{
public:
	CByteArray();
	CByteArray(const CByteArray &other);
	virtual ~CByteArray();

	CByteArray&		operator =	(const CByteArray &other);
	byte&			operator []	(ulong ulIdx);
	const byte&		operator []	(ulong ulIdx) const;

	ulong			Size() const;
	bool			Empty() const;
	void			Clear();
	void			PushBack(byte byVal);
	void			PopBack();
	void			Resize(ulong ulSize);
	void			Reserve(ulong ulCap);
	byte*			Data();
	const byte*		Data() const;

protected:
	DLLBYTEARY*		m_pAry;
};
