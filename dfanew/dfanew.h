#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include "../common/common.h"

#ifndef DFANEW_H_
#define DFANEWSC __declspec(dllimport)
#else
#define DFANEWSC __declspec(dllexport)
#endif

class DFANEWSC CFinalStates
{
	typedef std::unordered_map<STATEID, std::set<size_t>> FINSTAMAP;
	typedef FINSTAMAP::iterator FINSTAMAP_ITER;
	typedef FINSTAMAP::const_iterator FINSTAMAP_CITER;
public:
	CFinalStates();
	virtual ~CFinalStates();
	CFinalStates(const CFinalStates &other);
	CFinalStates& operator=(const CFinalStates &other);

	STATEID operator[](size_t nIdx) const;

	size_t Size() const;
	void Clear();
	void PushBack(STATEID nStaId, size_t nDfaId);
	size_t GetDfaIdCount(STATEID nStaId) const;
	size_t GetAllDfaIdCount() const;
	void GetDfaIds(STATEID nStaId, CVectorUnsigned &ids) const;

	std::set<size_t>& _GetDfaIds(STATEID nStaId);
	const std::set<size_t>& _GetDfaIds(STATEID nStaId) const;

protected:
	std::vector<STATEID> *m_pStates;
	FINSTAMAP *m_pDfaIds;
};

/*
* Store a row for CDfa. Array of std::vector<size_t>. Each element of the
* array stand by a column that indicates the jumping character.
* Each number in vector is the next state would be jump to by the character.
*/
class DFANEWSC CDfaRow
{
public:
	enum STATEFLAG
	{
		NORMAL   = 1 << 0,
		START    = 1 << 1,
		TERMINAL = 1 << 2
	};
	explicit CDfaRow(size_t col);
	void Fill(STATEID _Val);
	~CDfaRow();
	CDfaRow(const CDfaRow &other);
	CDfaRow& operator=(const CDfaRow &other);
	STATEID& operator[](BYTE nIdx);
	const STATEID& operator[](BYTE nIdx) const;
	void SetFlag(size_t nFlag);
	size_t GetFlag() const;
	size_t GetColNum() const;
protected:
	size_t m_nFlag;//标记该状态/行的属性：NORMAL、START、TERMINAL
	size_t m_nColNum;
	STATEVEC *m_pDest;
};


class DFANEWSC CDfaNew
{
public:
	CDfaNew();
	~CDfaNew();
	CDfaNew(const CDfaNew &other);
	CDfaNew& operator=(const CDfaNew &other);

	CDfaRow& operator[](STATEID index);
	const CDfaRow& operator[](STATEID index) const;

	size_t Size() const;
	CDfaRow& Back();
	void Reserve(size_t nSize);
	void Resize(size_t nSize, size_t nCol);
	void Init(BYTE *pGroup);
	void Clear();
	void PushBack(CDfaRow &sta);

	void SetId(size_t id);
	size_t GetId();
	WORD GetGroupCount() const;
	const BYTE GetGroup(STATEID charNum) const;

	CFinalStates& GetFinalState();
	const CFinalStates& GetFinalState() const;

	size_t FromNFA(const CNfa &nfa);
	size_t Minimize();

	BYTE Char2Group(BYTE nIdx);
	STATEID GetStartId() const;
	void SetStartId(STATEID id);
	size_t Process(BYTE *ByteStream, size_t len, CStateSet &StaSet);

	size_t Save(BYTE *beg);
	void Load(BYTE *beg, size_t len);

	void Dump(const char *pFile);

protected:
	size_t m_nId;
	WORD m_nColNum;
	STATEID m_nStartId;
	BYTE m_pGroup[DFACOLSIZE];

	std::vector<class CDfaRow> *m_pDfa;
	CFinalStates m_FinStas;

	void InitPartSet(std::vector<struct PARTSET> &partSet) const;
	void RemoveUnreachable(const STATEVEC *Tab, const STATELIST &begs, 
		const size_t &col, STATEVEC &reachable);
	void MergeReachable(STATEVEC &reachable);
	size_t PartitionNonDisState(STATEVEC *pRevTbl, std::vector<struct PARTSET> &partSet) const;
	void MergeNonDisStates(std::vector<struct PARTSET> &partSet);

};

struct NSTATESET_HASH
{
	enum {MAX_SIZE = 20000};
	size_t operator()(const std::vector<size_t> &set)
	{
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;

		const size_t *pData = set.data();
		const size_t *pEnd = pData + set.size();

		size_t nr = _FNV_offset_basis;
		for (; pData != pEnd; ++pData)
		{
			nr ^= *pData;
			nr *= _FNV_prime;
		}
		return nr;
	}
};

DFANEWSC bool MergeMultipleDfas(std::vector<CDfaNew> &dfas, CDfaNew &lastDfa);

DFANEWSC void PrintDfaToGv(CDfaNew &newdfa, const char* fileName);

