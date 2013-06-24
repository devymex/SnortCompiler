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
	CFinalStates(const CFinalStates &other);

	/// @brief		����������
	virtual ~CFinalStates();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ��CFinalStates����
	/// @return		���������������á�
	CFinalStates& operator = (const CFinalStates &other);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�DFA״̬��š�
	/// @param[in]	nIdx ָ��λ�õ��±ꡣ
	/// @return		����ָ��λ�õ�DFA״̬��š�
	STATEID operator [] (ulong nIdx) const;

	/// @brief		�õ�CFinalStates��������Ԫ��������
	/// @return		CFinalStates��������Ԫ��������
	ulong Size() const;

	/// @brief		ɾ��CFinalStates�����е���̬�����Ӧ��DFA��ż��ϡ�
	void Clear();

	/// @brief		��̬����������DFA��̬�ı�š�
	/// @param[in]	nStaId ��̬��š�
	/// @return		�¼������̬����Ӧ��CDfaIdSet��������á�
	CDfaIdSet& AddState(STATEID nStaId);

	/// @brief		����һ��CDfaIdSet�滻��ǰ��CDfaIdSet��
	/// @param[in]	other ��һ��CDfaIdSet����
	void Swap(CFinalStates &other);

	/// @brief		��ȡDFA��̬��Ӧ��CDfaIdSet��������á�
	/// @param[in]	nStaId ״̬��š�
	/// @return		CDfaIdSet��������á�
	CDfaIdSet& GetDfaIdSet(STATEID nStaId);

	/// @brief		��ȡDFA��̬��Ӧ��CDfaIdSet��������ã���������
	/// @param[in]	nStaId ״̬��š�
	/// @return		CDfaIdSet��������ã���������
	const CDfaIdSet& GetDfaIdSet(STATEID nStaId) const;

	/// @brief		����ȫ����̬����Ӧ��DFAID��������
	/// @return		DFAID������
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