/*!
* @file			pcrechain.h
* @author		Lab 435, Xidian University
* @brief		������CPcreChain�࣬����Snort����ѡ������ԣ�����PCRE��
*/

#pragma once

#include <hwprj\pcreopt.h>

#ifndef SNORTRULEHDR_DS
class PCREVEC;
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief		ʵ��PCRE�������ݽṹ������㷨
* @remark		��װ��std::vector<class CPcreOption>�����Լ�Signature����
*/
class SNORTRULEHDR CPcreChain
{
public:
	/// @brief		���캯����
	CPcreChain();

	/// @brief		�������캯����
	CPcreChain(const CPcreChain &other);

	/// @brief		����������
	virtual ~CPcreChain();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	CPcreChain& operator = (const CPcreChain &other);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�״̬�����顣
	/// @param[in]	nIdx ָ��λ�õ��±ꡣ
	/// @return		����ָ��λ�õ�״̬�����顣
	CPcreOption& operator[] (ulong nIdx);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�CPcreOption����
	/// @param[in]	nIdx ָ��λ�õ��±ꡣ
	/// @return		����ָ��λ�õ�״̬�����顣
	const CPcreOption& operator[] (ulong nIdx) const;

	/// @brief		�õ���������Ԫ��������
	/// @return		��������Ԫ��������
	ulong Size() const;

	/// @brief
	void Clear();

	/// @brief
	CPcreOption& Back() const;

	/// @brief
	void PushBack(const CPcreOption &pcreOpt);

	/// @brief
	void Resize(ulong nSize);

	/// @brief		��ȡPCRE���е�signature��
	CUnsignedArray& GetSigs();

	/// @brief		��ȡPCRE���е�signature����������
	const CUnsignedArray& GetSigs() const;

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	PCREVEC*				m_pPcreVec;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	CUnsignedArray			m_sigs;
};

/*!
@}
*/
