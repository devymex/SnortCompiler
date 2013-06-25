/*!
* @file			dllstring.h
* @author		Lab 435, Xidian University
* @brief		������CStringArray�༰�䵼������
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\dllstring.h>

#ifndef DLLSTRHDR_DS
class DLLSTRINGARY;
#endif

/*!
* @addtogroup groupCommon
* @{
*/

/*!
* @brief	��װ��std::vector<CDllString>�����ṩ�ַ�����̬��������ݽṹ��
*/
class DLLSTRHDR CStringArray
{
public:
	/// @brief		���캯����
	CStringArray();

	/// @brief		�������캯����
	CStringArray(const CStringArray &other);

	/// @brief		����������
	virtual ~CStringArray();

	/// @brief		���صĸ�ֵ�������
	/// @param[in]	other ��һ��CStringArray����
	CStringArray& operator = (const CStringArray &other);

	/// @brief		����ָ��λ�õ��ַ�����
	/// @param[in]	ulIdx ָ����λ�á�
	/// @return		һ��CDllString��������á�
	CDllString& operator [] (ulong ulIdx);

	/// @brief		����ָ��λ�õ��ַ�������������
	/// @param[in]	ulIdx ָ����λ�á�
	/// @return		һ��CDllString��������á�
	const CDllString& operator [] (ulong ulIdx) const;

	/// @brief		������������ַ�����������
	/// @return		�ַ����ĸ�����
	ulong Size() const;

	/// @brief		ɾ���������������ַ�����
	void Clear();

	/// @brief		��ָ�����ַ���������ӵ����顣
	/// @param[in]	str ����ӽ�����ַ�������
	void PushBack(const CDllString &str);

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	DLLSTRINGARY* m_pStrAry;
};

/*!
@}
*/
