/*!
* @file			dllstring.h
* @author		Lab 435, Xidian University
* @brief		������CDllString�༰�䵼������
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
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
* @addtogroup groupCommon
* @{
*/

/*!
* @brief		��װstd::string������ģ��֮�䴫�ݲ�����
*/
class DLLSTRHDR CDllString
{
public:
	/// @brief		���캯����
	CDllString();

	/// @brief		���ι��캯����
	/// @param[in]	pStr ��'\0'Ϊ��β��ANSI�ַ���ָ�롣
	explicit CDllString(pcstr pStr);

	/// @brief		�������캯����
	CDllString(const CDllString &other);

	/// @brief		����������
	virtual ~CDllString();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	CDllString& operator = (const CDllString &other);

	char& operator[] (ulong nIdx) const;

	/// @brief		����ַ����ĳ��ȡ�
	/// @return		�ַ������ַ��ĸ�����
	ulong Size() const;

	/// @brief		�ж��ַ����Ƿ�Ϊ�ա�
	/// @retval		true �ַ���Ϊ�ա�
	/// @retval		false �ַ����ǿա�
	bool Empty() const;

	/// @brief		����ַ�����
	void Clear();

	/// @brief		ɾ��ָ��λ�õ�һ���ַ���
	/// @param[in]	ulIdx ָ��Ҫɾ����λ�á�
	void Erase(ulong ulIdx);

	///	@brief		��ָ��λ�ò���һ���ַ���
	/// @param[in]	ulIdx ָ��Ҫ�����λ�á�
	/// @param[in]	nChar ָ��Ҫ������ַ���
	/// @remark		ָ�����ַ��������뵽ָ��λ�ô��ַ���ǰ�档
	void Insert(ulong ulIdx, char nChar);

	///	@brief		��ָ��λ�ò���һ���ַ�����
	/// @param[in]	ulIdx ָ��Ҫ�����λ�á�
	/// @param[in]	pChar ָ��Ҫ������ַ�����
	/// @remark		ָ�����ַ����������뵽ָ��λ�ô��ַ���ǰ�档
	void Insert(ulong ulIdx, pcstr pChar);

	///	@brief		�����洢���ַ�����ֵΪָ�����ַ�����
	/// @param[in]	pStr ָ������'\0'��β���ַ�����
	void Assign(const char* pStr);

	///	@brief		��ָ������һ���ַ�����ӵ����ַ�����ĩβ��
	/// @param[in]	other ָ������һ���ַ�������
	void Append(const CDllString &other);

	/// @brief		��ȡ�ַ��������һ���ַ���
	/// @return		�ַ��������һ���ַ���
	char Back() const;

	/// @brief		��ָ���ַ���ӵ��ַ�����ĩβ��
	/// @param		
	void PushBack(char nChar);
	const char* Data() const;

protected:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	DLLSTRING* m_pString;
};

/*!
@}
*/
