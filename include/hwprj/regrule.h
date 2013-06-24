/*!
* @file			regrule.h
* @author		Lab 435, Xidian University
* @brief		����CRegRule��
*/

#pragma once

#include <hwprj\pcrechain.h>

#ifndef SNORTRULEHDR_DS
class CHAINVEC;
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief ʵ���˹�񻯵�snort�������ݽṹ���ṩ��һ����������ݽ��й���Ĺ��ܡ�
* @remark ��Ҫ���ݽṹΪCPcreChain�Ķ�̬���飬��װ��std::vector<CPcreChain>��
*/
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

	/// @brief		���ָ��λ�õ�һ��PCRE����
	/// @param[in]	nIdx ָ����PCRE����λ�á�
	/// @return		һ��CPcreChain��������á�
	CPcreChain& operator[] (ulong nIdx);

	/// @brief		���ָ��λ�õ�һ��PCRE������������
	/// @param[in]	nIdx ָ����PCRE����λ�á�
	/// @return		һ��CPcreChain��������ã���������
	const CPcreChain& operator[] (ulong nIdx) const;

	/// @brief		�õ���������CPcreChain�����������
	/// @return		CPcreChain�����������
	ulong Size() const;

	/// @brief		ɾ��ָ��λ�õ�PCRE����
	/// @param[in]	nIdx ָ����PCRE����λ�á�
	void Erase(ulong ulIdx);

	/// @brief		Ԥ��ָ����С������ռ䡣
	/// @param[in]	nCap ָ��Ԥ���ռ�Ĵ�С��
	void Reserve(ulong nCap);

	/// @brief		��������Ĵ�С��
	/// @param[in]	nSize ָ����PCRE���ĸ���
	void Resize(ulong nSize);

	/// @brief		������������һ��PCRE����
	/// @return		һ��CPcreChain��������á�
	CPcreChain& Back();

	/// @brief		������������һ��PCRE������������
	/// @return		һ��CPcreChain��������ã���������
	const CPcreChain& Back() const;

	/// @brief		��ָ����PCRE����ӵ������ĩβ��
	void PushBack(const CPcreChain &nRegChain);

	/// @brief		ɾ������ĩβ�����һ��PCRE����
	void PopBack();

protected:
	/// ˽�г�Ա�������ڲ�ʹ�á�
	CHAINVEC* m_pRegVec;
};

/*!
@}
*/
