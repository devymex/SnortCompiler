/*!
* @file			dfaarray.h
* @author		Lab 435, Xidian University
* @brief		������CDfaArray�࣬���Դ洢���CDfa����DFA����
*/

#pragma once

#include <hwprj\dfa.h>

#ifndef DFAARYHDR_DS
class CDFAVEC;
#define DFAARYHDR __declspec(dllimport)
#else
#define DFAARYHDR __declspec(dllexport)
#endif

/*!
* @addtogroup groupAutomaton
* @{
*/

/*!
* @brief		��װ��std::vector<class CDfa>���󣬱�ʾDFA��������ݽṹ��
* @remark		������ÿһ��Ԫ�ض���һ��CDfa����
*/
class DFAARYHDR CDfaArray
{
public:
	/// @brief		���캯����
	CDfaArray();

	/// @brief		�������캯����
	CDfaArray(const CDfaArray& other);

	/// @brief		����������
	virtual ~CDfaArray();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ��DFA�������
	/// @return		���������������á�
	CDfaArray& operator = (const CDfaArray &other);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�CDfa���������á�
	/// @param[in]	nIdx ָ��λ�õ��±�
	/// @return		����ָ��λ�õ�CDfa���������á�
	CDfa& operator [] (ulong ulIdx);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�CDfa���������á�
	/// @param[in]	nIdx ָ��λ�õ��±�
	/// @return		����ָ��λ�õ�CDfa���������á�
	const CDfa& operator [] (ulong ulIdx) const;

	/// @brief		���DFA���鵱ǰ�ĸ�����
	/// @return		DFA���鵱ǰ�ĸ�������������
	ulong Size() const;

	/// @brief		���DFA����
	void Clear();

	/// @brief		Ϊ�洢DFA����Ԥ���ڴ�ռ䡣
	/// @param		ulCount ָ����Ԥ���Ŀռ䡣
	void Reserve(ulong ulCount);

	/// @brief		����DFA����Ĵ�С��
	/// @param[in]	ulSize ���ĵĳߴ硣
	/// @remark		���ulSize���ڻ���ڵ�ǰ�ߴ磬�򲻻��������Ԫ�أ�
	///				�����ɾ��������ֵ�Ԫ�ء�
	void Resize(ulong ulSize);

	/// @brief		��ָ����DFA���ΪDFA��������һ��Ԫ�ء�
	/// @param		dfa ָ����DFA��
	void PushBack(const CDfa &dfa);

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	CDFAVEC *m_pdfaTbl;
};

/*!
@}
*/
