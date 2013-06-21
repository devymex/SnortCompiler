/*
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
#endif

typedef CUnsignedArray STATEARY;

class DFAHDR CDfa
{
public:
	CDfa();
	CDfa(const CDfa &other);
	virtual ~CDfa();

	CDfa&				operator =	(const CDfa &other);
	CDfaRow&			operator []	(STATEID nIdx);
	const CDfaRow&		operator []	(STATEID nIdx) const;

	ulong				Size() const;
	CDfaRow&			Back();
	void				Reserve(ulong ulSize);
	void				Resize(ulong ulSize, ulong ulColNum);
	void				Clear();
	void				PushBack(CDfaRow &sta);

	ulong				GetId();
	void				SetId(ulong ulId);

	ushort				GetGroupCount() const;
	void				SetGroups(byte *pGroup);
	byte				Char2Group(byte nIdx);

	STATEID				GetStartState() const;
	void				SetStartState(STATEID id);
	CFinalStates&		GetFinalStates();
	const CFinalStates&	GetFinalStates() const;

	ulong				FromNFA(const CNfa &nfa);
	ulong				Minimize();
	void				MergeColumn();

	ulong				CalcStoreSize() const;
	void				Save(byte *beg);
	void				Load(byte *beg);

	ulong				Process(byte *ByteStream, ulong len, STATEARY &StaSet);
	void				Dump(pcstr pFile);

protected:
	ulong				PartStates(STATEIDARY *pRevTbl);

protected:
	ulong				m_nId;
	ushort				m_usColNum;
	STATEID				m_nStartId;
	DFAROWARY*			m_pDfa;
	CFinalStates		m_FinStas;
	byte				m_pGroup[SC_DFACOLCNT];
};

DFAHDR bool MergeMultipleDfas(class CDfaArray &inputDfas, CDfa &mergedDfa);

DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName);

