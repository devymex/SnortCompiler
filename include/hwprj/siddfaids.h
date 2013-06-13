#pragma once

#include <hwprj\unsary.h>

#ifndef SIDDFAHDR_DS
#define SIDDFAHDR __declspec(dllimport)
#else
#define SIDDFAHDR __declspec(dllexport)
#endif

struct COMPILEDRULENEW
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
	ulong m_nSid;
	ulong m_nResult;
	CUnsignedArray m_dfaIds;
};

class SIDDFAHDR CSidDfaIds
{
public:
	CSidDfaIds();
	CSidDfaIds(const CSidDfaIds& other);
	const CSidDfaIds &operator=(const CSidDfaIds &other);
	~CSidDfaIds();

	COMPILEDRULENEW& operator[](ulong index);
	const COMPILEDRULENEW& operator[](ulong index) const;
	void Reserve(ulong nCount);
	void Resize(ulong nSize);
	const ulong Size() const;
	void PushBack(const COMPILEDRULENEW &sidDfaIds);
	COMPILEDRULENEW& Back();
protected:
	std::vector<COMPILEDRULENEW> *m_ruleResult;
};
