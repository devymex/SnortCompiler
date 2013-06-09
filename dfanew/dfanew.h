#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include "../common/common.h"
#define DFACOLSIZE 256

#define HASHMODULO 1000000

//#define SC_STATELIMIT 255
//#define SC_STATELIMIT ((1 << (sizeof(STATEID) * 8)) - 1)
#define SC_STATELIMIT 500
#define DFA_SIZE_LIMIT 255

#define EMPTY 256

#ifndef DFANEW_H_
#define DFANEWSC __declspec(dllimport)
#else
#define DFANEWSC __declspec(dllexport)
#endif

struct DFANEWSC NFALOG
{
	size_t nfaStateId;
	size_t dfaId;
};

struct DFANEWSC TERMSET
{
	TERMSET(STATEID dfaStateId = (STATEID)-1, size_t nDfaId = (size_t)1)
		: dfaSta(dfaStateId), dfaId(nDfaId) {}
	STATEID dfaSta;
	size_t dfaId;
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
	STATEID& operator[](STATEID index);
	const STATEID& operator[](STATEID index) const;
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
	size_t Size() const;
	CDfaRow& operator[](STATEID index);
	const CDfaRow& operator[](STATEID index) const;
	CDfaRow& BackRow();
	void ReservRow(size_t nCount);
	void ResizeRow(size_t nSize, size_t nCol);
	void Init(BYTE *pGroup);
	void Clear();
	void reserve(size_t Maxnum);
	void PushBackDfa(CDfaRow &sta);
	void PushBackTermSet(TERMSET &term);
	void UniqueTermSet();
	//size_t AddTermIntoDFA(STATEID sta, const CDfaNew &other, STATEID thisSta);//根据other的sta查找termset，将找到的TERMSET插入到的this的m_TermSet中，其中this的状态是thisSta中
	TERMSET& BackTermSet();
	//size_t FromNFA(const CNfa &nfa, NFALOG *nfalog, size_t Count, bool combine = false);
	size_t FromNFA(const CNfa &nfa);
	size_t Minimize();
	WORD GetGroupCount() const;
	size_t GetTermCnt() const;
	TERMSET& GetTerm(size_t nIdx) const;//取出第nIdx个TERMSET
	BYTE Char2Group(BYTE nIdx);
	const BYTE* GetGroup() const;
	const BYTE GetOneGroup(STATEID charNum) const;
	STATEID GetStartId() const;
	void SetStartId(size_t id);
	void SetId(size_t id);
	size_t GetId();
	size_t Process(BYTE *ByteStream, size_t len, CStateSet &StaSet);
	void GetAcceptedId(STATEID id, CVectorUnsigned &dfaIds);
	size_t Save(BYTE *beg);
	void Load(BYTE *beg, size_t len);
	void printTerms();
protected:
	size_t m_nId;
	WORD m_nColNum;
	STATEID m_StartId;
	BYTE m_pGroup[DFACOLSIZE];
	std::vector<class CDfaRow> *m_pDfa;
	//pair.first 用来存放dfa的某一终态, pair.second 用来存放该终态对应哪一个dfaid
	std::vector<TERMSET> *m_TermSet;

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

		size_t nr = 2166136261U;
		for (; pData != pEnd; ++pData)
		{
			nr ^= *pData;
			nr *= _FNV_prime;
		}
		return nr;
	}
};

DFANEWSC void NAvaiEdges(const CNfa &oneNfaTab, BYTE *group);

DFANEWSC void GetDfaSig(CDfaNew &dfa,std::vector<std::vector<BYTE>> &allStr);
DFANEWSC void PrintDfaToText(CDfaNew &dfa, const char* filename);//用于测试输出一个dfa
DFANEWSC void PrintDfaToGv(CDfaNew &newdfa, const char* fileName);

