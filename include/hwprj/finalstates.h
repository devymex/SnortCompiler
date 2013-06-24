/*!
* @file			finalstates.h
* @author		Lab 435, Xidian University
* @brief		������CFinalStates�࣬���ڴ洢DFA״̬��ID��DFAID��ӳ���ϵ
*/

#pragma once

#include <hwprj\dfaidset.h>

#ifndef DFAIDSETHDR_DS
class STATEIDARY;
class FINSTAMAP;
#endif

/*!
* @addtogroup groupAutomaton
* @{
*/

/*!
* @brief		DFA����̬�����Լ���̬��Ӧ��DFAID����
*/
class DFAIDSETHDR CFinalStates
{
public:
	/// @brief		���캯����
	CFinalStates();

	/// @brief		�������캯����
	/// @param[in]	other ��һ��CFinalStates����
	CFinalStates(const CFinalStates &other);

	/// @brief		����������
	virtual ~CFinalStates();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ��CFinalStates����
	/// @return		���������������á�
	CFinalStates& operator = (const CFinalStates &other);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�״̬��š�
	/// @param[in]	nIdx ָ��λ�õ��±ꡣ
	/// @return		����ָ��λ�õ�״̬��š�
	const STATEID& operator [] (ulong nIdx) const;

	/// @brief		�õ���������Ԫ��������
	/// @return		��������Ԫ��������
	ulong Size() const;

	/// @brief		ɾ�����������е���̬�����Ӧ��DFA��ż��ϡ�
	void Clear();

	/// @brief		��̬����������DFA��̬�ı�š�
	/// @param[in]	nStaId ״̬��š�
	/// @return		�¼������̬����Ӧ��DFA��ż��϶�������á�
	CDfaIdSet& AddState(STATEID nStaId);

	/// @brief		����һ��DFA����̬�����滻��ǰDFA����̬���ϡ�
	/// @param[in]	other DFA��̬���ϡ�
	void Swap(CFinalStates &other);

	/// @brief		��ȡDFA��̬��Ӧ��DFA��ż��϶�������á�
	/// @param[in]	nStaId ״̬��š�
	/// @return		DFA��ż��϶�������á�
	CDfaIdSet& GetDfaIdSet(STATEID nStaId);

	/// @brief		��ȡDFA��̬��Ӧ��DFA��ż��϶�������ã���������
	/// @param[in]	nStaId ״̬��š�
	/// @return		DFA��ż��϶�������ã���������
	const CDfaIdSet& GetDfaIdSet(STATEID nStaId) const;

	/// @brief		����ȫ����̬����Ӧ��DFA��ŵ�������
	/// @return		DFA���������
	ulong CountDfaIds() const;

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	STATEIDARY* m_pStates;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	FINSTAMAP* m_pDfaIds;
};

/*!
@}
*/