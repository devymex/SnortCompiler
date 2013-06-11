#pragma once

#include <hwprj\dfa.h>

#ifndef DFAARYHDR_DS
class CDFAVEC;
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
	CDfa& operator[](ulong index);
	const CDfa& operator[](ulong index) const;
	void Reserve(ulong nCount);
	void Resize(ulong nSize);
	const ulong Size() const;
	void PushBack(const CDfa &dfa);
	void Clear();
protected:
	CDFAVEC *m_pdfaTbl;
};
