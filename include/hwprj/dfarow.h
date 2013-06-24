/*!
* @file			dfarow.h
* @author		Lab 435, Xidian University
* @brief		������CDfaRow�࣬���Դ洢DFA��һ��״̬
* @remark		CDfaRowΪһ��һά���飬�����е�ÿ��Ԫ�������б�ʾ
*				DFA�ַ����е�һ����ת�ַ��������е�ÿ��Ԫ�ر�ʾ��ǰDFA״̬
*				������ת�ַ��������һ��״̬�ı�š�
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

class DFAHDR CDfaRow
{
public:
	typedef byte STATEFLAG;

	/// @brief ��Ǹ�������Ӧ��״̬������
	static const STATEFLAG NORMAL	= 0;			/// һ��״̬
	static const STATEFLAG FINAL	= (1 << 1);		/// ��̬

	/// @brief		���캯����
	/// @param[in]	ulColCnt DFA�ַ����ĸ�����
	explicit CDfaRow(ulong ulColCnt);

	/// @brief		�������캯����
	/// @param[in]	other ��һ��CDfaRow����
	CDfaRow(const CDfaRow &other);

	/// @brief		����������
	virtual ~CDfaRow();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ��CDfaRow����
	/// @return		���������������á�
	CDfaRow& operator =	(const CDfaRow &other);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�ֵ�����á�
	/// @param[in]	nIdx ָ��λ�õ��±�
	/// @return		����ָ��λ��Ԫ�ص����á�
	const STATEID& operator [] (byte nIdx) const;

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�ֵ�����á�
	/// @param[in]	nIdx ָ��λ�õ��±�
	/// @return		����ָ��λ��Ԫ�ص����ã���������
	STATEID& operator [] (byte nIdx);

	/// @brief		�õ�CDfaRow��ǰ�Ĵ�С��
	/// @return		CDfaRow��ǰ�Ĵ�С����������
	ulong Size() const;

	/// @brief		��CDfaRow�е�����Ԫ�ظ�ֵΪָ��״̬
	/// @param[in]	nState ָ����״̬��
	void Fill(STATEID nState);

	/// @brief		����״̬����
	/// @param[in]	nFlags ����ȡֵ
	void AddFlags(STATEFLAG nFlags);

	/// @brief		����״̬����
	/// @param[in]	nFlags ����ȡֵ
	void SetFlags(STATEFLAG nFlags);

	/// @brief		��ȡ״̬����
	STATEFLAG GetFlags() const;

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	STATEIDARY* m_pDest;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	STATEFLAG m_nFlags;
};

/*!
@}
*/