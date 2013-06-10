/**
**  @file        dfa.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CDllArray, CDllString
**
*/

#pragma once

#include <windows.h>
#include <vector>
#include <map>

#include <hwprj\common.h>
#include <hwprj\dfarow.h>
#include <hwprj\finalstates.h>


class CNfa;

class DFAHDR CDfa
{
public:
	CDfa();
	~CDfa();
	CDfa(const CDfa &other);
	CDfa& operator=(const CDfa &other);

	CDfaRow& operator[](STATEID index);
	const CDfaRow& operator[](STATEID index) const;

	ULONG Size() const;
	CDfaRow& Back();
	void Reserve(ULONG nSize);
	void Resize(ULONG nSize, ULONG nCol);
	void Init(BYTE *pGroup);
	void Clear();
	void PushBack(CDfaRow &sta);

	void SetId(ULONG id);
	ULONG GetId();
	WORD GetGroupCount() const;
	const BYTE GetGroup(STATEID charNum) const;

	CFinalStates& GetFinalState();
	const CFinalStates& GetFinalState() const;

	ULONG FromNFA(const CNfa &nfa);
	ULONG Minimize();

	BYTE Char2Group(BYTE nIdx);
	STATEID GetStartId() const;
	void SetStartId(STATEID id);
	ULONG Process(BYTE *ByteStream, ULONG len, CStateSet &StaSet);

	ULONG Save(BYTE *beg);
	void Load(BYTE *beg, ULONG len);

	void Dump(const char *pFile);

protected:
	ULONG m_nId;
	WORD m_nColNum;
	STATEID m_nStartId;
	BYTE m_pGroup[DFACOLSIZE];

	std::vector<class CDfaRow> *m_pDfa;
	CFinalStates m_FinStas;

	void InitPartSet(std::vector<struct PARTSET> &partSet) const;
	void RemoveUnreachable(const STATEVEC *Tab, const STATELIST &begs, 
		const ULONG &col, STATEVEC &reachable);
	void MergeReachable(STATEVEC &reachable);
	ULONG PartitionNonDisState(STATEVEC *pRevTbl, std::vector<struct PARTSET> &partSet) const;
	void MergeNonDisStates(std::vector<struct PARTSET> &partSet);

};

struct NSTATESET_HASH
{
	enum {MAX_SIZE = 20000};
	ULONG operator()(const std::vector<ULONG> &set)
	{
		const ULONG _FNV_offset_basis = 2166136261U;
		const ULONG _FNV_prime = 16777619U;

		const ULONG *pData = set.data();
		const ULONG *pEnd = pData + set.size();

		ULONG nr = _FNV_offset_basis;
		for (; pData != pEnd; ++pData)
		{
			nr ^= *pData;
			nr *= _FNV_prime;
		}
		return nr;
	}
};

DFAHDR bool MergeMultipleDfas(std::vector<CDfa> &dfas, CDfa &lastDfa);

DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName);

