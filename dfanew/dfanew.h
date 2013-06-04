#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include "../common/common.h"

#define DFACOLSIZE 256

#define HASHMODULO 1000000

//#define SC_STATELIMIT 255
//#define SC_STATELIMIT ((1 << (sizeof(STATEID) * 8)) - 1)
#define SC_STATELIMIT 1000
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

//struct PARTSET1
//{
//	PARTSET() : pAble(NULL), nLen(0) { }
//	explicit PARTSET(size_t len) : pAble(NULL), nLen(0)
//	{
//		if (len != 0)
//		{
//			pAble = (BYTE*)VirtualAlloc(NULL, len, MEM_COMMIT, PAGE_READWRITE);
//			nLen = len;
//		}
//	}
//	PARTSET(const PARTSET &other)
//	{
//		*this = other;
//	}
//	const PARTSET& operator = (const PARTSET &other)
//	{
//		Resize(other.nLen);
//		memcpy(pAble, other.pAble, nLen);
//		return *this;
//	}
//	~PARTSET()
//	{
//		VirtualFree(pAble, 0, MEM_RELEASE);
//	}
//	void Resize(size_t len)
//	{
//		if (len != nLen)
//		{
//			if (nLen != 0)
//			{
//				VirtualFree(pAble, 0, MEM_RELEASE);
//				pAble = NULL;
//			}
//			pAble = (BYTE*)VirtualAlloc(NULL, len, MEM_COMMIT, PAGE_READWRITE);
//			nLen = len;
//		}
//	}
//	void Reset()
//	{
//		memset(pAble, 0, nLen);
//	}
//	BYTE& Able(size_t nIdx)
//	{
//		if (nIdx >= nLen || pAble == NULL)
//		{
//			throw 0;
//		}
//		return pAble[nIdx];
//	}
//	size_t GetOnes()
//	{
//		return std::count(pAble, pAble + nLen, (BYTE)1);
//	}
//	std::list<STATEID> StaSet;
//	size_t nLen;
//private:
//	BYTE *pAble;
//};
//
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
	size_t FromNFA(const CNfa &nfa, NFALOG *nfalog, size_t Count, bool combine = false);
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

	void InitPartSet(std::vector<struct PARTSET> &partSet) const;
	void RemoveUnreachable(const std::vector<STATEID> *Tab, const STALIST &begs, 
		const size_t &col, std::vector<STATEID> &reachable);
	void MergeReachable(std::vector<STATEID> &reachable);
	size_t PartitionNonDisState(std::vector<STATEID> *pRevTbl, std::vector<struct PARTSET> &partSet) const;
	void MergeNonDisStates(std::vector<struct PARTSET> &partSet);

};

DFANEWSC void GetDfaSig(CDfaNew &dfa,std::vector<std::vector<BYTE>> &allStr);
DFANEWSC void outPut(CNfa &nfa, const char* fileName);//用于测试输出一个nfa
DFANEWSC void outPutDfa(CDfaNew &dfa, const char* filename);//用于测试输出一个dfa
DFANEWSC void fdisplay(CDfaNew &newdfa, const char* fileName);

