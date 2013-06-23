/*!
* @file		dfa.h
* @author		Lab 435, Xidian University
* @brief		Deterministic Finite Automaton
* Declaration of the CDfa class
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
* @brief		ʵ��DFA���ݽṹ������㷨
* ��װ��CDfaRow�Ķ�̬���顢ʼ̬����Լ���̬���ϵȡ�ʵ�ֵ��㷨������nfa��ȷ������\
* dfa����С�����ַ�����ѹ���ȡ�
*/
class DFAHDR CDfa
{
public:
	CDfa();
	CDfa(const CDfa &other);
	virtual ~CDfa();

	CDfa& operator =	(const CDfa &other);
	CDfaRow& operator []	(STATEID nIdx);
	const CDfaRow& operator []	(STATEID nIdx) const;

	ulong				Size() const;
	CDfaRow& Back();
	void Reserve(ulong ulSize);
	void Resize(ulong ulSize, ulong ulColNum);
	void Clear();
	void PushBack(CDfaRow &sta);

	/// @brief		��ȡDFA�ı�š�
	ulong				GetId();

	/// @brief		����DFA�ı�š�
	/// @param[in] ulId DFA��š�
	void SetId(ulong ulId);

	/// @brief		��ȡDFA�ַ����Ĵ�С��
	ushort				GetGroupCount() const;

	/// @brief		����һ��DFA����ת�ַ�����
	/// @param[in] pGroup �������ַ���
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

	/// @brief		NFAת��ΪDFA�������Ӽ������㷨
	/// @param[in]	CNfa���͵�һ��NFA
	ulong FromNFA(const CNfa &nfa);

	/// @brief		��С��DFA��״̬��������Hopcrof�㷨��1976��
	ulong Minimize();

	/// @brief		NFAת��ΪDFA����һ��ѹ���ַ������Լ��ٴ洢�ռ�
	void MergeColumn();

	/// @brief		����һ��DFA��Ҫ�Ĵ洢�ռ��С
	ulong CalcStoreSize() const;

	/// @brief		��һ��DFA��unsigned char*���ʹ����ڴ�
	/// @param[in]	beg DFA�Ķ������ֽ���
	void Save(byte *beg);

	/// @brief		���ڴ��ȡһ��DFA������unsigned char*���ͱ���
	/// @param[in]	beg DFA�Ķ������ֽ���
	void Load(byte *beg);

	/// @brief		�������ݰ���DFA��ƥ�����
	/// @param[in]	ByteStream �������ݰ��Ķ������ֽ�����
	/// @param[in]	len ���ݰ����ȡ�
	/// @param[out]	StaSet CDfa״̬���顣
	ulong Process(byte *ByteStream, ulong len, STATEARY &StaSet);

	/// @brief		��һ��DFA������ļ���
	/// @param[in]	pFile ����ļ�·��
	void Dump(pcstr pFile);

protected:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	ulong PartStates(STATEIDARY *pRevTbl);

protected:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	ulong m_nId;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	ushort m_usColNum;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	STATEID m_nStartId;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	DFAROWARY* m_pDfa;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	CFinalStates m_FinStas;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	byte m_pGroup[SC_DFACOLCNT];
};

/// @brief		�������ж��DFA�ϲ�Ϊһ��DFA
/// @param[in] nputDfas ������DFA
/// @param[out] mergedDfa ���һ���ϲ�DFA
DFAHDR bool MergeMultipleDfas(class CDfaArray &inputDfas, CDfa &mergedDfa);

/// @brief		DFA�洢Ϊ����ṹ����DFA����ļ�����
/// @param[in] newdfa ����DFA��
/// @param[in] fileName ����ļ�·��
DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName);

