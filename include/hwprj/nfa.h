/*!
* @file		nfa.h
**
* @author		Lab 435, Xidian University
**
* @brief			Declaration of the CNfa class		
**
* Functions declaration of nfa's essential algorithm
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\nfarow.h>

#ifndef NFAHDR_DS
class NFAROWARY;
#endif

/*!
* @brief		ʵ��NFA�����ݽṹ��
* NFA�ṹΪ���󣬾����б�ʾNFA״̬�������б�ʾ��ת�ַ���\
* ������ÿһ��Ԫ�ش���������ָ�����ַ���ת������һ��״̬���ϡ�
*/
class NFAHDR CNfa
{
public:
	CNfa();
	CNfa(const CNfa &other);
	virtual ~CNfa();

	CNfa& operator = (const CNfa &other);
	CNfaRow& operator [] (ulong ulIdx);
	const CNfaRow& operator [] (ulong ulIdx) const;

	ulong Size() const;
	CNfaRow& Back();
	void Clear();
	void Resize(ulong ulSize);
	void Reserve(ulong ulSize);
	void Shrink();
	void PushBack(const CNfaRow &row);
	void PopBack();

	/// @brief		��NFAÿһ��״̬�е�Ԫ������
	///	
	void SortAll();

	/// @brief		��һ��NFA������ļ��С�
	/// @param	pFile ����ļ�·����
	void Dump(const char *pFile) const;

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	NFAROWARY*		m_pNfa;
};
