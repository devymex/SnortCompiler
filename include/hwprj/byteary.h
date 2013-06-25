/*!
* @file			byteary.h
* @author		Lab 435, Xidian University
* @brief		������CByteArry�༰�䵼�����š�
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once

#include <hwprj\common.h>

#ifndef BYTEARYHDR_DS
class DLLBYTEARY;
#define BYTEARYHDR __declspec(dllimport)
#else
#define BYTEARYHDR __declspec(dllexport)
#endif

/*!
* @addtogroup groupCommon
* @{
*/

/*!
* @brief		��װ��vector<unsigned char>��������ģ��֮�䴫�ݲ�����
*/
class BYTEARYHDR CByteArray
{
public:
	/// @brief		���캯����
	CByteArray();

	/// @brief		�������캯����
	CByteArray(const CByteArray &other);

	/// @brief		����������
	virtual ~CByteArray();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	CByteArray& operator = (const CByteArray &other);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�ֵ�����á�
	/// @param[in]	nIdx ָ��λ�õ��±ꡣ
	/// @return		����ָ��λ��Ԫ�ص����á�
	byte& operator [] (ulong ulIdx);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�ֵ�����ã���������
	/// @param[in]	nIdx ָ��λ�õ��±ꡣ
	/// @return		����ָ��λ��Ԫ�ص����ã���������
	const byte& operator [] (ulong ulIdx) const;

	/// @brief		��������Ĵ�С��
	/// @return		�ֽڵ�������
	ulong Size() const;

	/// @brief		�ж������Ƿ�Ϊ�ա�
	/// @retval		true ����Ϊ�ա�
	/// @retval		false ����ǿա�
	bool Empty() const;

	/// @brief		������顣
	void Clear();

	/// @brief		�������ĩβ���һ���ֽڵ����ݡ�
	/// @param[in]	byVal ��ӵ��ֽ�ֵ��
	void PushBack(byte byVal);

	/// @brief		ɾ������ĩβ��һ��������
	void PopBack();

	/// @brief		��������Ĵ�С��ִ��std::vector::resize������
	void Resize(ulong ulSize);

	/// @brief		��������Ԥ���ռ�Ĵ�С��ִ��std::vector::reserve������
	void Reserve(ulong ulCap);

	/// @brief		�������������ָ�롣
	/// @return		�����һ���ֽڵ�ָ�롣
	byte* Data();

	/// @brief		�������������ָ�루��������
	/// @return		�����һ���ֽڵ�ָ�루��������
	const byte* Data() const;

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	DLLBYTEARY* m_pAry;
};

/*!
@}
*/
