/*!
* @file			dfa.h
* @author		Lab 435, Xidian University
* @brief		������CDfa�࣬���Ա�ʾDFA�����ݽṹ
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\nfa.h>
#include <hwprj\dfarow.h>
#include <hwprj\finalstates.h>
#include <hwprj\unsary.h>

#include <vector>

#ifndef DFAHDR_DS
class DFAROWARY;
#endif

typedef CUnsignedArray STATEARY;

/*!
* @addtogroup groupAutomaton
* @{
*/

/*!
* @brief		ʵ��DFA���ݽṹ������㷨
* @remark		��װ��CDfaRow�Ķ�̬���顢ʼ̬����Լ���̬���ϵȡ�ʵ�ֵ��㷨������
*				nfa��ȷ������dfa����С�����ַ�����ѹ���ȡ�
*/
class DFAHDR CDfa
{
public:
	/// @brief		���캯����
	CDfa();

	/// @brief		�������캯����
	CDfa(const CDfa &other);

	/// @brief		����������
	virtual ~CDfa();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ��CDfa����
	/// @return		���������������á�
	CDfa& operator =	(const CDfa &other);

	/// @brief		���ص� '[]' �������ȡ��DFA״̬�����Ӧ��CDfaRow��������á�
	/// @param[in]	nIdx DFA״̬���
	/// @return		����״̬��Ŷ�Ӧ��CDfaRow��������á�
	CDfaRow& operator [] (STATEID nIdx);

	/// @brief		���ص� '[]' �������ȡ��DFA״̬�����Ӧ��CDfaRow��������á�
	/// @param[in]	nIdx DFA״̬���
	/// @return		����״̬��Ŷ�Ӧ��CDfaRow��������ã���������
	const CDfaRow& operator [] (STATEID nIdx) const;

	/// @brief		�õ�DFA��ǰ�Ĵ�С��
	/// @return		DFA��ǰ�Ĵ�С����������
	ulong Size() const;

	/// @brief		�õ�DFA���һ��״̬�����á�
	/// @return		���һ��״̬��Ŷ�Ӧ��CDfaRow��������á�
	CDfaRow& Back();

	/// @brief		ΪDFAԤ���ڴ�ռ䡣
	/// @param		ulSize ָ����Ԥ���Ŀռ䡣
	void Reserve(ulong ulSize);

	/// @brief		����DFA�Ĵ�С��
	/// @param[in]	ulSize ���ĵĳߴ硣
	/// @param[in]	ulColNum DFA��ת�ַ����ĳ���
	/// @remark		���ulSize���ڻ���ڵ�ǰ�ߴ磬�򲻻��������Ԫ�أ�
	///				�����ɾ��������ֵ�Ԫ�ء�
	void Resize(ulong ulSize, ulong ulColNum);

	/// @brief		���DFA
	void Clear();

	/// @brief		��ָ����DFA״̬���ΪDFA��ת�����һ��Ԫ�ء�
	/// @param		sta ָ����DFA״̬��
	void PushBack(CDfaRow &sta);

	/// @brief		��ȡDFA�ı�š�
	ulong GetId();

	/// @brief		����DFA�ı�š�
	/// @param[in]	ulId DFA��š�
	void SetId(ulong ulId);

	/// @brief		��ȡDFA��ת�ַ����Ĵ�С��
	ushort GetGroupCount() const;

	/// @brief		����һ��DFA����ת�ַ�����
	/// @param[in]	pGroup ѹ������ַ���
	void SetGroups(byte *pGroup);

	/// @brief		�ӱ�׼ANSI�ַ���ӳ�䵽DFA���ַ�����
	/// @param[in]	nIdx ��׼ANSI�ַ����롣
	/// @return		��ָ���ַ������Ӧ��DFA�ַ���
	byte Char2Group(byte nIdx);

	/// @brief		��ȡDFA��ʼ״̬id
	STATEID GetStartState() const;

	/// @brief		����DFA��ʼ״̬id
	/// @param[in]	id DFA״̬���
	void SetStartState(STATEID id);

	/// @brief		��ȡDFA��̬����
	CFinalStates& GetFinalStates();

	/// @brief		��ȡDFA��̬���ϣ�������
	const CFinalStates& GetFinalStates() const;

	/// @brief		NFAת��ΪDFA��ִ��ȷ���������������Ӽ������㷨��
	/// @param[in]	һ��CNfa����
	ulong FromNFA(const CNfa &nfa);

	/// @brief		��С��DFA��״̬��������Hopcrof�㷨��Hopcrof1976��
	ulong Minimize();

	/// @brief		NFAת��ΪDFA����һ��ѹ���ַ������Լ��ٴ洢�ռ䡣
	///	@todo		�˺���δ��ɡ�
	void MergeColumn();

	/// @brief		����һ��DFA��Ҫ�Ĵ洢�ռ��С��
	ulong CalcStoreSize() const;

	/// @brief		��һ��DFA��unsigned char*���ʹ����ڴ档
	/// @param[in]	beg DFA�Ķ������ֽ���
	void Save(byte *beg);

	/// @brief		���ڴ��ȡһ��DFA������unsigned char*���ͱ�����
	/// @param[in]	beg DFA�Ķ������ֽ���
	void Load(byte *beg);

	/// @brief		�������ݰ���DFA��ƥ����̡�
	/// @param[in]	ByteStream �������ݰ��Ķ������ֽ�����
	/// @param[in]	len ���ݰ����ȡ�
	/// @param[out]	StaSet CDfa״̬���顣
	ulong Process(byte *ByteStream, ulong len, STATEARY &StaSet);

	/// @brief		��һ��DFA������ļ��С�
	/// @param[in]	pFile ����ļ�·����
	void Dump(pcstr pFile);

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	ulong PartStates(STATEIDARY *pRevTbl);

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	ulong m_nId;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	ushort m_usColNum;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	STATEID m_nStartId;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	DFAROWARY* m_pDfa;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	CFinalStates m_FinStas;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	byte m_pGroup[SC_DFACOLCNT];
};

/// @brief		�������ж��DFA�ϲ�Ϊһ��DFA��
/// @param[in]	nputDfas ������DFA��
/// @param[out] mergedDfa ���һ���ϲ�DFA��
DFAHDR bool MergeMultipleDfas(class CDfaArray &inputDfas, CDfa &mergedDfa);

/// @brief		��һ��DFA�������Ϊgv��ʽ���Թ�GraphVis�����ʾ֮�á�
/// @param[in]	newdfa �����CDfa����
/// @param[in]	fileName ����ļ�������'\0'Ϊ��β���ַ���ָ�롣
DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName);

/*!
@}
*/
