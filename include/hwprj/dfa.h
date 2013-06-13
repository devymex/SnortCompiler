/**
**	@file		dfa.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Deterministic Finite Automaton
**
**	Declaration of the CDfa class
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\nfa.h>
#include <hwprj\dfarow.h>
#include <hwprj\finalstates.h>
#include <hwprj\unsary.h>

#include <vector>

/* Hiding the stl object in member of CDfa*/
#ifndef DFAHDR_DS
class DFAROWARY;
class STATEVEC;
class STATELIST;
#endif

typedef CUnsignedArray STATEARY;

class DFAHDR CDfa
{
public:
	CDfa();
	CDfa(const CDfa &other);
	virtual ~CDfa();

	CDfa&			operator =	(const CDfa &other);
	CDfaRow&		operator []	(STATEID index);
	const CDfaRow&	operator []	(STATEID index) const;

	ulong		Size() const;
	CDfaRow&	Back();
	void		Reserve(ulong nSize);
	void		Resize(ulong nSize, ulong nCol);
	void		Clear();
	void		PushBack(CDfaRow &sta);

	ulong		GetId();
	void		SetId(ulong id);

	ushort		GetGroupCount() const;
	void		SetGroups(byte *pGroup);
	byte		Char2Group(byte nIdx);

	ulong		FromNFA(const CNfa &nfa);
	ulong		Minimize();

	STATEID		GetStartState() const;
	void		SetStartState(STATEID id);

	ulong		Process(byte *ByteStream, ulong len, STATEARY &StaSet);

	ulong		Save(byte *beg);
	void		Load(byte *beg, ulong len);

	void		Dump(const char *pFile);

	CFinalStates&		GetFinalState();
	const CFinalStates&	GetFinalState() const;

protected:
	ulong			m_nId;
	ushort			m_nColNum;
	STATEID			m_nStartId;
	byte			m_pGroup[SC_DFACOLCNT];
	DFAROWARY		*m_pDfa;
	CFinalStates	m_FinStas;

	ulong PartStates(STATEVEC *pRevTbl);

};

//需要更改接口，参数不能为stl对象！
DFAHDR bool MergeMultipleDfas(std::vector<CDfa> &dfas, CDfa &lastDfa);

DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName);

