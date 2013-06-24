/*!
* @file				dllstring.h
**
* @author			Lab 435, Xidian University
**
* @brief			Common string class
**
* Definition of the CDllString class
**
*/

#pragma once

#include <hwprj\common.h>

#ifndef DLLSTRHDR_DS
class DLLSTRING;
#define DLLSTRHDR __declspec(dllimport)
#else
#define DLLSTRHDR __declspec(dllexport)
#endif

/*!
* @brief		��װstd::string������ģ��֮�䴫�ݲ���
*/
class DLLSTRHDR CDllString
{
public:
	/// @brief		���캯��
	CDllString();

	/// @brief		���ι��캯��
	/// @param[in]	pStr ��'\0'Ϊ��β��ANSI�ַ���ָ��
	explicit CDllString(pcstr pStr);

	/// @brief		�������캯��
	CDllString(const CDllString &other);

	/// @brief		��������
	virtual ~CDllString();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	CDllString& operator = (const CDllString &other);


	char			operator[]	(ulong nIdx) const;

	ulong			Size() const;
	bool			Empty() const;
	void Clear();
	void Erase(ulong ulIdx);
	void Insert(ulong ulIdx, char nChar);
	void Insert(ulong ulIdx, pcstr pChar);
	void Assign(const char* pStr);
	void Append(const CDllString &other);
	char			Back() const;
	void PushBack(char nChar);
	const char*		Data() const;

protected:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	DLLSTRING*		m_pString;
};
