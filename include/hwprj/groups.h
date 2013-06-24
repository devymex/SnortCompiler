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
* @brief		��Ϊһ���DFA��Ϣ��
* @remark		���������ЩDFA����ЩDFA�Ĺ���Signature����ǰ�����ĸ�Signature����÷��顢�ϲ����DFA��
*/

struct ONEGROUP
{
	CUnsignedArray	DfaIds;
	CUnsignedArray	ComSigs;
	SIGNATURE		currSig;
	ulong			mergeDfaId;
};

/*!
* @brief		�洢DFA�����з��������
* @remark		������ЩDFA��Ϊһ�顢��ЩDFA�Ĺ���Signature����ǰ�����ĸ�Signature����÷��顢�ϲ����DFA��
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