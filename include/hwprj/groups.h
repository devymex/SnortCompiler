#pragma once

#include <hwprj\unsary.h>
#include <hwprj\signatures.h>

#ifndef GROUPSHDR_DS
#define GROUPSHDR __declspec(dllimport)
#else
#define GROUPSHDR __declspec(dllexport)
#endif

#ifndef GROUPSHDR_DS
class GROUPVEC;
#endif

struct ONEGROUP
{
	CUnsignedArray DfaIds;
	CSignatures ComSigs;
	SIGNATURE currSig;
	ulong mergeDfaId;
};

class GROUPSHDR CGroups
{
public:
	CGroups();
	CGroups(const CGroups& other);
	CGroups &operator=(const CGroups &other);
	virtual ~CGroups();

	const ulong Size() const;
	void Resize(ulong nSize);
	void PushBack(ONEGROUP oneGroup);
	ONEGROUP& Back();
	ONEGROUP &operator[](ulong nIdx);
	const ONEGROUP &operator[](ulong nIdx) const;
	void Clear();
	void Erase(ulong nIdx);

protected:
	GROUPVEC *m_pGroups;
};