/**
**  @file        common.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CUnsignedArray, CDllString
**
*/

#pragma once

#include <hwprj\common.h>


#ifndef DFAHDR_DS

class STATEVEC;

#define DFAHDR __declspec(dllimport)

#else

#define DFAHDR __declspec(dllexport)

#endif

/*
* Store a row for CDfaRow. Array of std::vector<ulong>. Each element of the
* array stand by a column that indicates the jumping character.
* Each number in vector is the next state would be jump to by the character.
*/
class DFAHDR CDfaRow
{
public:
	enum STATEFLAG
	{
		NORMAL   = 1 << 0,
		START    = 1 << 1,
		TERMINAL = 1 << 2
	};
	explicit CDfaRow(ulong col);
	void Fill(STATEID _Val);
	~CDfaRow();
	CDfaRow(const CDfaRow &other);
	CDfaRow& operator=(const CDfaRow &other);
	STATEID& operator[](byte nIdx);
	const STATEID& operator[](byte nIdx) const;
	void SetFlag(ulong nFlag);
	ulong GetFlag() const;
	ulong GetColNum() const;
protected:
	ulong m_nFlag;//标记该状态/行的属性：NORMAL、START、TERMINAL
	ulong m_nColNum;
	STATEVEC *m_pDest;
};

