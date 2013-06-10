/**
**  @file        common.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CDllArray, CDllString
**
*/

#pragma once

#include <hwprj\common.h>

#ifndef DFAHDR_DS
#define DFAHDR __declspec(dllimport)
#else
#define DFAHDR __declspec(dllexport)
#endif

/*
* Store a row for CDfaRow. Array of std::vector<ULONG>. Each element of the
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
	explicit CDfaRow(ULONG col);
	void Fill(STATEID _Val);
	~CDfaRow();
	CDfaRow(const CDfaRow &other);
	CDfaRow& operator=(const CDfaRow &other);
	STATEID& operator[](BYTE nIdx);
	const STATEID& operator[](BYTE nIdx) const;
	void SetFlag(ULONG nFlag);
	ULONG GetFlag() const;
	ULONG GetColNum() const;
protected:
	ULONG m_nFlag;//标记该状态/行的属性：NORMAL、START、TERMINAL
	ULONG m_nColNum;
	STATEVEC *m_pDest;
};

