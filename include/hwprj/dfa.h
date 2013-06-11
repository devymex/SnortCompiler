/**
**  @file        dfa.h
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
#include <hwprj\nfa.h>
#include <hwprj\dfarow.h>
#include <hwprj\finalstates.h>
#include <hwprj\unsary.h>

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
	~CDfa();
	CDfa(const CDfa &other);
	CDfa& operator=(const CDfa &other);

	CDfaRow& operator[](STATEID index);
	const CDfaRow& operator[](STATEID index) const;

	ulong Size() const;
	CDfaRow& Back();
	void Reserve(ulong nSize);
	void Resize(ulong nSize, ulong nCol);
	void Init(byte *pGroup);
	void Clear();
	void PushBack(CDfaRow &sta);

	void SetId(ulong id);
	ulong GetId();
	ushort GetGroupCount() const;
	const byte GetGroup(STATEID charNum) const;

	CFinalStates& GetFinalState();
	const CFinalStates& GetFinalState() const;

	ulong FromNFA(const CNfa &nfa);
	ulong Minimize();

	byte Char2Group(byte nIdx);
	STATEID GetStartId() const;
	void SetStartId(STATEID id);
	ulong Process(byte *ByteStream, ulong len, STATEARY &StaSet);

	ulong Save(byte *beg);
	void Load(byte *beg, ulong len);

	void Dump(const char *pFile);

protected:
	ulong m_nId;
	ushort m_nColNum;
	STATEID m_nStartId;
	byte m_pGroup[SC_DFACOLCNT];

	DFAROWARY *m_pDfa;
	CFinalStates m_FinStas;

	void InitPartSet(std::vector<struct PARTSET> &partSet) const;
	void RemoveUnreachable(const STATEVEC *Tab, const STATELIST &begs, 
		const ulong &col, STATEVEC &reachable);
	void MergeReachable(STATEVEC &reachable);
	ulong PartitionNonDisState(STATEVEC *pRevTbl, std::vector<struct PARTSET> &partSet) const;
	void MergeNonDisStates(std::vector<struct PARTSET> &partSet);

};

//需要更改接口，参数不能为stl对象！
DFAHDR bool MergeMultipleDfas(std::vector<CDfa> &dfas, CDfa &lastDfa);

DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName);

