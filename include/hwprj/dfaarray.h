#pragma once

#include <hwprj\dfa.h>

#ifndef DFAARYHDR_DS
#define DFAARYHDR __declspec(dllimport)
#else
#define DFAARYHDR __declspec(dllexport)
#endif

class DFAARYHDR CDfaArray
{
public:
	CDfaArray();
	CDfaArray(const CDfaArray& other);
	const CDfaArray &operator=(const CDfaArray &other);
	~CDfaArray();
	CDfa& operator[](ULONG index);
	const CDfa& operator[](ULONG index) const;
	void Reserve(ULONG nCount);
	void Resize(ULONG nSize);
	const ULONG Size() const;
	void PushBack(const CDfa &dfa);
	void Clear();
protected:
	std::vector<CDfa> *m_pdfaTbl;
};
