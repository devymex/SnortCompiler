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
#include <hwprj\trace.h>

#ifndef UNSARYHDR_DS
#define UNSARYHDR __declspec(dllimport)
#else
#define UNSARYHDR __declspec(dllexport)
#endif

/* Ê¹ÓÃdll·â×°ulong* */
class UNSARYHDR CUnsignedArray
{
public:
	// CDCA
	inline CUnsignedArray();
	inline CUnsignedArray(const CUnsignedArray &other);
	inline virtual ~CUnsignedArray();

	// Overided operators
	inline CUnsignedArray&	operator = (const CUnsignedArray &other);

	inline ulong&			operator [] (ulong nIdx);
	inline const ulong&		operator [] (ulong nIdx) const;
	inline bool				operator == (const CUnsignedArray &other);

	// Access member
	inline ulong			Size() const;
	inline ulong&			Back();
	inline void				Clear();
	inline void				PopBack();
	inline void				PushBack(ulong ulVal);
	inline void				Reserve(ulong nCap);
	inline void				Resize(ulong ulSize);
	inline void				Resize(ulong ulSize, ulong ulVal);
	inline ulong*			Data();
	inline const ulong*		Data() const;

	// Algorithms
	inline void				Fill(ulong _Val);

	void Sort();
	void Unique();

protected:
	inline ulong*			Alloc(ulong ulCap);
	inline void				Free(ulong *pBuf);
	inline void				Increase();
	inline void				Realloc(ulong ulCap);

	void					CopyFrom(ulong *pBuf);

protected:
	ulong*					m_pAry;
	ulong					m_ulSize;
	size_t					m_ulCap;
};

#include <hwprj\unsary.inc>
