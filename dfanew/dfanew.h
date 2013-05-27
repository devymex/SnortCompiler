#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include "../common/common.h"

#define DFACOLSIZE 256

#define HASHMODULO 1000000

//#define SC_STATELIMIT 255
#define SC_STATELIMIT 1000

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
	TERMSET(STATEID dfaStateId = (STATEID)-1, size_t nDfaId = (size_t)-1)
		: dfaSta(dfaStateId), dfaId(nDfaId) {}
	STATEID dfaSta;
	size_t dfaId;
};

class DFANEWSC CDfanew
{
public:
	CDfanew();
	~CDfanew();
	CDfanew(const CDfanew &other);
	CDfanew& operator=(const CDfanew &other);
	size_t Size() const;
	CDfaRow& operator[](STATEID index);
	const CDfaRow& operator[](STATEID index) const;
	CDfaRow& BackRow();
	void ReservRow(size_t nCount);
	void ResizeRow(size_t nSize, size_t nCol);
	void Init(BYTE *pGroup);
	void Clear();
	void reserve(STATEID Maxnum);
	void PushBackDfa(CDfaRow &sta);
	void PushBackTermSet(TERMSET &term);
	TERMSET& BackTermSet();
	size_t FromNFA(const CNfa &nfa, NFALOG *nfalog, size_t Count, bool combine = false);
	size_t Minimize();
	WORD GetGroupCount() const;
	BYTE Char2Group(BYTE nIdx);
	const BYTE* GetGroup() const;
	const BYTE GetOneGroup(STATEID charNum) const;
	STATEID GetStartId() const;
	void SetStartId(size_t id);
	void SetId(size_t id);
	size_t GetId();
	size_t Process(BYTE *ByteStream, size_t len, CStateSet &StaSet);
	void GetAcceptedId(STATEID id, CVectorNumber &dfaIds);
	size_t Save(BYTE *beg);
	void Load(BYTE *beg, size_t len);
	void printTerms();
private:
	size_t m_nId;
	WORD m_nColNum;
	STATEID m_StartId;
	BYTE m_pGroup[DFACOLSIZE];
	std::vector<CDfaRow> *m_pDfa;
	//pair.first 用来存放dfa的某一终态, pair.second 用来存放该终态对应哪一个dfaid
	std::vector<TERMSET> *m_TermSet;

	void RemoveUnreachable(const std::vector<STATEID> *Tab, const STALIST &begs, 
		const size_t &col, std::vector<STATEID> &reachable);
	void MergeReachable(std::vector<STATEID> &reachable);
	void PartitionNonDisState(std::vector<STATEID> *pRevTbl, SETLIST &pSets) const;
	void MergeNonDisStates(SETLIST &Partition);

};

DFANEWSC void GetDfaSig(CDfanew &dfa,std::vector<std::vector<BYTE>> &allStr);
DFANEWSC void outPut(CNfa &nfa, const char* fileName);//用于测试输出一个nfa
DFANEWSC void outPutDfa(CDfanew &dfa, const char* filename);//用于测试输出一个dfa

