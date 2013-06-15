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

/* Capsulate the ulong* for dll using. */
class UNSARYHDR CUnsignedArray
{
public:
	// CDCA
	__forceinline CUnsignedArray()
		: m_pAry(0), m_ulSize(0), m_ulCap(0)
	{
	}

	__forceinline CUnsignedArray(const CUnsignedArray &other)
		: m_pAry(0), m_ulSize(0), m_ulCap(0)
	{
		*this = other;
	}

	__forceinline virtual ~CUnsignedArray()
	{
		free(m_pAry);
		//delete []m_pAry;
	}

	// Overided operators
	__forceinline const CUnsignedArray& operator = (const CUnsignedArray &other)
	{
		if (m_ulCap < other.m_ulSize)
		{
			ulong *pNewBuf = Alloc(other.m_ulSize);
			free(m_pAry);
			//delete []m_pAry;
			m_pAry = pNewBuf;
			m_ulCap = other.m_ulSize;
		}
		m_ulSize = other.m_ulSize;
		if (m_ulSize != 0)
		{
			CopyFrom(other.m_pAry);
		}
		return *this;
	}

	__forceinline ulong& operator [] (ulong nIdx)
	{
		TASSERT(nIdx < m_ulSize);
		return m_pAry[nIdx];
	}

	__forceinline const ulong& operator [] (ulong nIdx) const
	{
		TASSERT(nIdx < m_ulSize);
		return m_pAry[nIdx];
	}

	__forceinline bool operator == (const CUnsignedArray &other)
	{
		if (m_ulSize != other.m_ulSize)
		{
			return false;
		}
		for (ulong i = 0; i < m_ulSize; ++i)
		{
			if (m_pAry[i] != other.m_pAry[i])
			{
				return false;
			}
		}
		return true;
	}

	// Access member
	__forceinline ulong Size() const
	{
		return m_ulSize;
	}

	__forceinline ulong& Back()
	{
		TASSERT(m_ulSize > 0);
		return m_pAry[(long)m_ulSize - 1];
	}

	__forceinline ulong* Data()
	{
		return m_pAry;
	}

	__forceinline const ulong* Data() const
	{
		return m_pAry;
	}

	__forceinline void Clear()
	{
		m_ulSize = 0;
	}

	__forceinline void PopBack()
	{
		--m_ulSize;
	}

	__forceinline void PushBack(ulong ulVal)
	{
		TASSERT(m_ulCap >= m_ulSize);
		if (m_ulCap <= m_ulSize)
		{
			Increase();
		}
		m_pAry[m_ulSize] = ulVal;
		++m_ulSize;
	}

	__forceinline void Reserve(ulong nCap)
	{
		if (nCap > m_ulCap)
		{
			Realloc(nCap);
		}
	}

	__forceinline void Resize(ulong ulSize)
	{
		TASSERT(m_ulCap >= m_ulSize);
		if (m_ulCap < ulSize)
		{
			Realloc(ulSize);
		}
		m_ulSize = ulSize;
	}

	__forceinline void Resize(ulong ulSize, ulong ulVal)
	{
		TASSERT(m_ulCap >= m_ulSize);
		if (m_ulCap < ulSize)
		{
			Realloc(ulSize);
		}
		for (ulong i = m_ulSize; i < ulSize; ++i)
		{
			m_pAry[i] = ulVal;
		}
		m_ulSize = ulSize;
	}

	// Algorithms
	__forceinline void Fill(ulong _Val)
	{
		for (ulong i = 0; i < m_ulSize; ++i)
		{
			m_pAry[i] = _Val;
		}
	}

	void Sort();

	void Unique();

protected:
	void CopyFrom(ulong *pBuf);

	__forceinline ulong* Alloc(ulong ulCap)
	{
		ulong *pNewBuf = 0;
		if (ulCap != 0)
		{
			//pNewBuf = new ulong[ulCap];
			pNewBuf = (ulong*)malloc(ulCap * sizeof(pNewBuf[0]));
		}
		return pNewBuf;
	}

	__forceinline void Increase()
	{
		if (m_ulCap == 0)
		{
			Realloc(1);
		}
		else
		{
			Realloc(m_ulSize * 2);
		}
	}

	__forceinline void Realloc(ulong ulCap)
	{
		if (m_ulCap != ulCap)
		{
			ulong *pOldBuf = m_pAry;

			m_pAry = Alloc(ulCap);
			m_ulCap = ulCap;

			if (m_ulSize > m_ulCap)
			{
				m_ulSize = m_ulCap;
			}
			if (m_ulSize != 0)
			{
				CopyFrom(pOldBuf);
			}
			delete []pOldBuf;
		}
	}

protected:
	ulong*		m_pAry;
	ulong		m_ulSize;
	size_t		m_ulCap;
};
