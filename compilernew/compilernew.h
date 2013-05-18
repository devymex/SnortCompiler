#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include <windows.h>

#ifndef COMPILERNEW_H_
#define COMPILERNEW __declspec(dllimport)
#else
#define COMPILERNEW __declspec(dllexport)
#endif

struct COMPILERNEW COMPILEDRULENEW
{
	enum
	{
		RES_SUCCESS = 0x0000,
		RES_ERROR = 0x0001, 
		RES_EXCEED = 0x0002, 
		RES_HASBYTE = 0x0004, 
		RES_HASNOT = 0x0008, 
		RES_EMPTY = 0x0010,
		RES_HASNOSIG = 0x0020,
		RES_EXCEEDLIMIT = 0x0040,
	};
	size_t m_nSid;
	size_t m_nResult;
	CVectorNumber m_dfaIds;
};

class COMPILERNEW CDfaTblNew
{
public:
	CDfaTblNew();
	CDfaTblNew(const CDfaTblNew& other);
	const CDfaTblNew &operator=(const CDfaTblNew &other);
	~CDfaTblNew();
	CDfanew& operator[](size_t index);
	const CDfanew& operator[](size_t index) const;
	void Reserve(size_t nCount);
	void Resize(size_t nSize);
	const size_t Size() const;
	void PushBack(const CDfanew &dfa);
private:
	std::vector<CDfanew> *m_pdfaTbl;
};

class COMPILERNEW CSidDfaIdsNew
{
public:
	CSidDfaIdsNew();
	CSidDfaIdsNew(const CSidDfaIdsNew& other);
	const CSidDfaIdsNew &operator=(const CSidDfaIdsNew &other);
	~CSidDfaIdsNew();
	COMPILEDRULENEW& operator[](size_t index);
	const COMPILEDRULENEW& operator[](size_t index) const;
	void Reserve(size_t nCount);
	void Resize(size_t nSize);
	const size_t Size() const;
	void PushBack(const COMPILEDRULENEW &sidDfaIds);
	COMPILEDRULENEW& Back();
private:
	std::vector<COMPILEDRULENEW> *m_ruleResult;
};
//ÆúÓÃ
//struct DFAINFO
//{
//	size_t dfaId;
//	size_t chainId;
//};
//
//class COMPILERNEW CDFASINFO
//{
//public:
//	CDFASINFO();
//	CDFASINFO(const CDFASINFO& other);
//	const CDFASINFO &operator=(const CDFASINFO &other);
//	~CDFASINFO();
//	DFAINFO& operator[](size_t index);
//	const DFAINFO& operator[](size_t index) const;
//	void Reserve(size_t nCount);
//	void Resize(size_t nSize);
//	const size_t Size() const;
//	void PushBack(const DFAINFO &dfaDetail);
//	DFAINFO& Back();
//private:
//	std::vector<DFAINFO> *m_pDfas;
//};

class COMPILERNEW CResNew
{
private:
	CDfaTblNew m_dfaTbl;
	CSidDfaIdsNew m_sidDfaIds;
	//CDFASINFO m_DfasInfo;
	CRegRule m_RegexTbl;
public:
	CDfaTblNew& GetDfaTable();
	CSidDfaIdsNew& GetSidDfaIds();
	//CDFASINFO &GetDfasInfo();
	CRegRule &GetRegexTbl();
	const CDfaTblNew& GetDfaTable() const;
	const CSidDfaIdsNew& GetSidDfaIds() const;
	//const CDFASINFO &GetDfasInfo() const;
	const CRegRule &GetRegexTbl() const;
	size_t WriteToFile(LPCTSTR filename);
	size_t ReadFromFile(LPCTSTR filename);
};

COMPILERNEW void compilenew(LPCTSTR filename, CResNew &result);