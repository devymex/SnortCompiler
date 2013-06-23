/*!
* @file			nfarow.h
* @author		Lab 435, Xidian University
* @brief		Declaration of CNfaRow
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\unsary.h>

#ifndef NFAHDR_DS
#define NFAHDR __declspec(dllimport)
#else
#define NFAHDR __declspec(dllexport)
#endif

/*!
* @brief		��װ��std::vector<CUnsignedArray>���󣬱�ʾNFA����һ�е����ݽṹ��
* @remark		NFA����ÿһ�ж�Ӧһ��״̬������ÿ��Ԫ�ض�Ӧһ���ַ����洢һ��״̬���
*				�Ķ�̬���飬��ʾ���ж�Ӧ��״̬��������ǰ�ַ�������ת����һ��״̬��
*/
class NFAHDR CNfaRow
{
public:
	/// @brief		NFA�������
	static const ulong COLUMNCNT = 260;

	/// @brief		���캯����
	CNfaRow();

	/// @brief		�������캯����
	CNfaRow(const CNfaRow &other);

	/// @brief		����������
	virtual ~CNfaRow();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	CNfaRow& operator = (const CNfaRow &other);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�״̬����������á�
	/// @param[in]	nIdx ָ��λ�õ��±ꡣ
	/// @return		����ָ��λ�õ�״̬����������á�
	CUnsignedArray& operator [] (ulong ulCol);


	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�״̬����������ã���������
	/// @param[in]	nIdx ָ��λ�õ��±ꡣ
	/// @return		����ָ��λ�õ�״̬����������ã���������
	const CUnsignedArray& operator [] (ulong ulCol) const;
	
	/// @brief		��NFA���е�ÿһ��״̬�������������
	/// @todo		�Ƿ���Ҫ����Unique������
	void SortAll();

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	CUnsignedArray* m_pDestSet;
};
