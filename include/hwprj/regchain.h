#pragma once

#include <hwprj\dllstring.h>
#include <hwprj\signatures.h>

#ifndef REGRULEHDR_DS
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

	CDllString& operator[](ULONG nIdx);

	ULONG Size() const;
	CDllString& Back() const;
	void PushBack(const CDllString &pcreStr);
	void Resize(ULONG nSize);

	CSignatures& GetSigs();
	const CSignatures& GetSigs() const;

protected:
	std::vector<CDllString> *m_pRegList;
	CSignatures m_sigs;
};
