
/*!
* @file		dllstring.h
**
* @author		Lab 435, Xidian University
**
* @brief			Common string class
**
* Definition of the CDllString class
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\dllstring.h>

#ifndef DLLSTRHDR_DS
class DLLSTRINGARY;
#endif

/*!
* @brief	��װ��std::vector<CDllString>�����ṩ�ַ�����̬��������ݽṹ��
*/
class DLLSTRHDR CStringArray
{
public:
	/// @brief		���캯����
	CStringArray();
	CStringArray(const CStringArray &other);
	virtual ~CStringArray();

	CStringArray& operator = (const CStringArray &other);
	CDllString& operator[] (ulong ulIdx);
	const CDllString& operator[] (ulong ulIdx) const;

	ulong Size() const;
	void Clear();
	void PushBack(const CDllString &str);

protected:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	DLLSTRINGARY*		m_pStrAry;
};
