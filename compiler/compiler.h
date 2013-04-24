#include "../common/common.h"
#include <windows.h>

#ifndef COMPILER_H_
#define COMPILER __declspec(dllimport)
#else
#define COMPILER __declspec(dllexport)
#endif

struct COMPILER COMPILEDRULE
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

class COMPILER CDfaTbl
{
public:
	CDfaTbl();
	CDfaTbl(const CDfaTbl& other);
	const CDfaTbl &operator=(const CDfaTbl &other);
	~CDfaTbl();
	CDfa& operator[](size_t index);
	const CDfa& operator[](size_t index) const;
	void Reserve(size_t nCount);
	void Resize(size_t nSize);
	const size_t Size() const;
	void PushBack(const CDfa &dfa);
private:
	std::vector<CDfa> *m_dfaTbl;
};

class COMPILER CSidDfaIds
{
public:
	CSidDfaIds();
	CSidDfaIds(const CSidDfaIds& other);
	const CSidDfaIds &operator=(const CSidDfaIds &other);
	~CSidDfaIds();
	COMPILEDRULE& operator[](size_t index);
	const COMPILEDRULE& operator[](size_t index) const;
	void Reserve(size_t nCount);
	void Resize(size_t nSize);
	const size_t Size() const;
	void PushBack(const COMPILEDRULE &sidDfaIds);
	COMPILEDRULE& Back();
private:
	std::vector<COMPILEDRULE> *m_ruleResult;
};

class COMPILER CRes
{
private:
	CDfaTbl m_dfaTbl;
	CSidDfaIds m_sidDfaIds;
public:
	CDfaTbl& GetDfaTable();
	CSidDfaIds& GetSidDfaIds();
	const CDfaTbl& GetDfaTable() const;
	const CSidDfaIds& GetSidDfaIds() const;
	size_t WriteToFile(LPCTSTR filename);
	size_t ReadFromFile(LPCTSTR filename);
};

COMPILER void compile(LPCTSTR filename, CRes &result);