/**
**  @file        common.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CDllArray, CDllString
**
*/

#pragma once

#include <string>
#include <hwprj\common.h>

#ifndef DLLSTRHDR_DS
#define DLLSTRHDR __declspec(dllimport)
#else
#define DLLSTRHDR __declspec(dllexport)
#endif

/* Capsulate the std::string for dll using. */
class DLLSTRHDR CDllString
{
public:
	// CDCA
	CDllString();
	explicit CDllString(const char *pStr);
	~CDllString();
	CDllString(const CDllString &other);
	CDllString& operator = (const CDllString &other);

	// Overided operators
	char operator[](ULONG nIdx) const;

	const ULONG Size() const;
	bool Empty();
	void Clear();
	void Append(const char* pChar);
	char Back() const;
	void PushBack(const char nChar);
	const char* GetStr();

protected:
	std::string *m_pString;
};
