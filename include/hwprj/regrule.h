/*!
* @file		regrule.h
**
* @author		Lab 435, Xidian University
**
* @brief			Declaration of the CRegRule class
**
* A rule includes multiple CPcreChain, all store in CRegRule
**
*/

#pragma once

#include <hwprj\pcrechain.h>

#ifndef SNORTRULEHDR_DS
class CHAINVEC;
#endif

class SNORTRULEHDR CRegRule
{
public:
	/// @brief		���캯����
	CRegRule();

	/// @brief		�������캯����
	CRegRule(const CRegRule &other);

	/// @brief		����������
	virtual ~CRegRule();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	CRegRule& operator = (const CRegRule &other);

	CPcreChain& operator[] (ulong nIdx);
	const CPcreChain& operator[] (ulong nIdx) const;

	/// @brief �õ���������CPcreChain�����������
	/// @return CPcreChain�����������
	ulong				Size() const;
	void Erase(ulong ulIdx);
	void Reserve(ulong nCount);
	void Resize(ulong nSize);
	CPcreChain& Back() const;
	void PushBack(const CPcreChain &nRegChain);
	void PopBack();

protected:
	CHAINVEC*			m_pRegVec;
};
