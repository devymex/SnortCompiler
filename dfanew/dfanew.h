#pragma once

#include <windows.h>
#include <vector>
#include "../common/common.h"

#define DFACOLSIZE 256

#define HASHMODULO 1000000

#define SC_STATELIMIT 254

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
	TERMSET(STATEID dfaStateId, size_t nDfaId)
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
	STATEID Size() const;
	CDfaRow& operator[](STATEID index);
	const CDfaRow& operator[](STATEID index) const;
	void Init(BYTE *pGroup);
	void Clear();
	size_t FromNFA(CNfa &nfa, NFALOG *nfalog, size_t Count, bool combine = false);
	size_t Minimize();
	size_t GetGroupCount() const;
	BYTE Char2Group(BYTE nIdx);
	const BYTE* GetGroup() const;
	STATEID GetStartId() const;
	void SetId(size_t id);
	size_t GetId();
	size_t Process(BYTE *ByteStream, size_t len, CStateSet &StaSet);
	void GetAcceptedId(STATEID id, CVectorNumber &dfaIds);
private:
	size_t m_nId;
	size_t m_nColNum;
	STATEID m_StartId;
	BYTE m_pGroup[DFACOLSIZE];
	std::vector<CDfaRow> *m_pDfa;
	//pair.first 用来存放dfa的某一终态, pair.second 用来存放该终态对应哪一个dfaid
	std::vector<TERMSET> *m_TermSet;
};
