/*!
* @file		groups.h
**
* @author		Lab 435, Xidian University
**
* @brief			Definition of the CGroups class
**
**
*/

#pragma once

#include <hwprj\unsary.h>

#ifndef GROUPSHDR_DS
#define GROUPSHDR __declspec(dllimport)
#else
#define GROUPSHDR __declspec(dllexport)
#endif

#ifndef GROUPSHDR_DS
class GROUPVEC;
#endif

/*!
* @brief		分为一组的DFA信息。
* @remark		该组包括哪些DFA、这些DFA的公共Signature、当前采用哪个Signature代表该分组、合并后的DFA。
*/

struct ONEGROUP
{
	CUnsignedArray	DfaIds;
	CUnsignedArray	ComSigs;
	SIGNATURE		currSig;
	ulong			mergeDfaId;
};

/*!
* @brief		存储DFA的所有分组情况。
* @remark		包括哪些DFA分为一组、这些DFA的公共Signature、当前采用哪个Signature代表该分组、合并后的DFA。
*/

class GROUPSHDR CGroups
{
public:
	CGroups();
	CGroups(const CGroups& other);
	virtual ~CGroups();

	CGroups& operator=(const CGroups &other);
	ONEGROUP& operator[](ulong nIdx);
	const ONEGROUP& operator[](ulong nIdx) const;

	ulong Size() const;
	void Resize(ulong nSize);
	void PushBack(ONEGROUP oneGroup);
	ONEGROUP& Back();
	void Clear();
	void Erase(ulong nIdx);

protected:
	GROUPVEC* m_pGroups;
};