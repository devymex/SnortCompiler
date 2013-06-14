/*
**	@file		unsary.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Common unsgined long array
**
*/

#pragma once

#include <hwprj\common.h>

#ifndef UNSARYHDR_DS
class DLLULONGVEC;
#define UNSARYHDR __declspec(dllimport)
#else
#define UNSARYHDR __declspec(dllexport)
#endif

/* Capsulate the std::vector<ulong> for dll using. */
class UNSARYHDR CUnsignedArray
{
public:
	// CDCA
	CUnsignedArray();
	CUnsignedArray(const CUnsignedArray &other);
	virtual ~CUnsignedArray();

	// Overided operators
	const CUnsignedArray&	operator =	(const CUnsignedArray &other);
	ulong&					operator []	(ulong nIdx);
	const ulong&			operator []	(ulong nIdx) const;
	bool					operator ==	(const CUnsignedArray &other);

	// Access member
	void		Clear();
	ulong		Size() const;
	void		PopBack();
	void		PushBack(ulong nState);
	void		Reserve(ulong nCount);
	void		Resize(ulong nSize);
	ulong&		Back();

	// Algorithms
	void		Sort();
	void		Unique();
	void		Fill(ulong _Val);

protected:
	DLLULONGVEC *m_pSet;
};
