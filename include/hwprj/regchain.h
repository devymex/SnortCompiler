#pragma once

#include <hwprj\dllstring.h>
#include <hwprj\signatures.h>

#ifndef REGRULEHDR_DS
class STRINGVEC;
#define REGRULEHDR __declspec(dllimport)
#else
#define REGRULEHDR __declspec(dllexport)
#endif

class REGRULEHDR CRegChain
{
public:
	CRegChain();
	~CRegChain();
	CRegChain(const CRegChain &other);
	const CRegChain& operator = (const CRegChain &other);

	CDllString& operator[](ulong nIdx);

	ulong Size() const;
	CDllString& Back() const;
	void PushBack(const CDllString &pcreStr);
	void Resize(ulong nSize);

	CSignatures& GetSigs();
	const CSignatures& GetSigs() const;

protected:
	STRINGVEC *m_pRegList;
	CSignatures m_sigs;
};
