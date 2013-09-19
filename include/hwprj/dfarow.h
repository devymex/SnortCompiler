/*!
* @file			dfarow.h
* @author		Lab 435, Xidian University
* @brief		������CDfaRow�࣬���Դ洢DFA��һ��״̬��
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once

#include <hwprj\common.h>


#ifndef DFAHDR_DS
class STATEIDARY;
#define DFAHDR __declspec(dllimport)
#else
#define DFAHDR __declspec(dllexport)
#endif

/*!
* @addtogroup groupAutomaton
* @{
*/

/*!
* @brief		������DFA����һ��״̬����Ӧ��һ�����ݵ����ݽṹ��
* @remark		��װ��std::vector<STATEID>����DFA��ת����ÿһ�ж�Ӧһ��״̬��
*				����ÿ��Ԫ�ض�Ӧһ���ַ����洢һ��״̬��ţ���ʾ���ж�Ӧ��״̬��
*				������ǰ�ַ�������ת������һ��״̬��
*/
class DFAHDR CDfaRow
{
public:
	typedef byte STATEFLAG;

	/// @brief		��DFA�е�ĳ��״̬Ϊ����״̬������ΪNORMAL��
	static const STATEFLAG NORMAL	= 0;			

	/// @brief		��DFA�е�ĳ��״̬Ϊ��ֹ״̬������ΪFINAL��
	static const STATEFLAG FINAL	= (1 << 0);		

	/// @brief		���캯����
	/// @param[in]	ulColCnt DFA��ת�ַ����ĳ��ȡ�
	explicit CDfaRow(ulong ulColCnt);

	/// @brief		�������캯����
	CDfaRow(const CDfaRow &other);

	/// @brief		����������
	virtual ~CDfaRow();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ��CDfaRow����
	/// @return		���������������á�
	CDfaRow& operator =	(const CDfaRow &other);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�DFA״̬��ŵ����á�
	/// @param[in]	nIdx ָ��λ�õ��±ꡣ
	/// @return		����ָ��λ�õ�DFA״̬��ŵ����ã���������
	const STATEID& operator [] (byte nIdx) const;

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�DFA״̬��ŵ����á�
	/// @param[in]	nIdx ָ��λ�õ��±ꡣ
	/// @return		����ָ��λ�õ�DFA״̬��ŵ����á�
	STATEID& operator [] (byte nIdx);

	/// @brief		�õ�CDfaRow��ǰ�Ĵ�С��
	/// @return		CDfaRow��ǰ�Ĵ�С����������
	ulong Size() const;

	/// @brief		��CDfaRow�е�����Ԫ�ظ�ֵΪָ��DFA״̬��
	/// @param[in]	nState ָ����DFA״̬��
	void Fill(STATEID nState);

	/// @brief		����DFA״̬�����ԡ�
	/// @param[in]	nFlags ����ȡֵ��
	void AddFlags(STATEFLAG nFlags);

	/// @brief		����DFA״̬�����ԡ�
	/// @param[in]	nFlags ����ȡֵ��
	void SetFlags(STATEFLAG nFlags);

	/// @brief		��ȡDFA״̬�����ԡ�
	STATEFLAG GetFlags() const;

	/////////////////////////////////////
	bool operator==(CDfaRow &rhs);
	void PushBack(ushort n);
private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	STATEIDARY* m_pDest;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	STATEFLAG m_nFlags;
};

/*!
@}
*/