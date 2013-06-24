/*!
* @file			nfa.h
* @author		Lab 435, Xidian University
* @brief		������CNfa�࣬���Ա�ʾNFA�����ݽṹ
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\nfarow.h>

#ifndef NFAHDR_DS
class NFAROWARY;
#endif

/*!
* @addtogroup groupAutomaton
* @{
*/

/*!
* @brief		ʵ��NFA�����ݽṹ��
* @remark		��װ��CNfaRow�Ķ�̬���顣NFA�ṹΪ���󣬾����б�ʾNFA״̬�������б�ʾ��ת�ַ���
*				������ÿһ��Ԫ�ش���������ָ�����ַ���ת������һ��״̬���ϡ�
*/
class NFAHDR CNfa
{
public:
	/// @brief		���캯����
	CNfa();

	/// @brief		�������캯����
	CNfa(const CNfa &other);

	/// @brief		����������
	virtual ~CNfa();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ��NFA����
	/// @return		���������������á�
	CNfa& operator = (const CNfa &other);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�CNfaRow��������á�
	/// @param[in]	ulIdx ָ��λ�õ��±�
	/// @return		����ָ��λ�õ�CNfaRow��������á�
	CNfaRow& operator [] (ulong ulIdx);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�CNfaRow��������á�
	/// @param[in]	ulIdx ָ��λ�õ��±�
	/// @return		����ָ��λ�õ�CNfaRow��������á�
	const CNfaRow& operator [] (ulong ulIdx) const;

	/// @brief		�õ�NFA��ǰ�Ĵ�С��
	/// @return		NFA��ǰ�Ĵ�С����������
	ulong Size() const;

	/// @brief		�õ�CNfa���һ��λ�õ�CNfaRow��������á�
	/// @return		���һ��λ�õ�CNfaRow��������á�
	CNfaRow& Back();

	/// @brief		���NFA��
	void Clear();

	/// @brief		����NFA�Ĵ�С��
	/// @param[in]	ulSize ���ĵĳߴ�
	/// @remark		���ulSize���ڻ���ڵ�ǰ�ߴ磬�򲻻��������Ԫ�أ�
	///				�����ɾ��������ֵ�Ԫ�ء�
	void Resize(ulong ulSize);

	/// @brief		ΪNFAԤ���ڴ�ռ䡣
	/// @param		ulSize ָ����Ԥ���Ŀռ�
	void Reserve(ulong ulSize);

	/// @brief		�ͷ�CNfa�ж���Ŀռ䡣
	void Shrink();

	/// @brief		��ָ����NFA״̬���ΪNFA��ת�����һ��Ԫ�ء�
	/// @param[in]	row ָ����NFA״̬
	void PushBack(const CNfaRow &row);

	/// @brief		ɾ��CNfa�����һ��״̬��
	void PopBack();

	/// @brief		��NFAÿһ��״̬�е�Ԫ������
	void SortAll();

	/// @brief		��һ��NFA������ļ��С�
	/// @param[in]	pFile ����ļ�·������������
	void Dump(const char *pFile) const;

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	NFAROWARY*		m_pNfa;
};

/*!
@}
*/