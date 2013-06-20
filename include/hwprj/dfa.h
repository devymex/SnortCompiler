/*
**	@file		dfa.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Deterministic Finite Automaton
**
**	Declaration of the CDfa class
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\nfa.h>
#include <hwprj\dfarow.h>
#include <hwprj\finalstates.h>
#include <hwprj\unsary.h>

#include <vector>

/* Hiding the stl object in member of CDfa*/
#ifndef DFAHDR_DS
class DFAROWARY;
#endif

typedef CUnsignedArray STATEARY;

class DFAHDR CDfa
{
public:
	CDfa();
	CDfa(const CDfa &other);
	virtual ~CDfa();

	CDfa&				operator =	(const CDfa &other);
	CDfaRow&			operator []	(STATEID nIdx);
	const CDfaRow&		operator []	(STATEID nIdx) const;

	ulong				Size() const;
	CDfaRow&			Back();
	void				Reserve(ulong ulSize);
	void				Resize(ulong ulSize, ulong ulColNum);
	void				Clear();
	void				PushBack(CDfaRow &sta);

	/*	��������GetId
	**	���ܣ���ȡDFA ID
	*/
	ulong				GetId();
	/*	��������SetId
	**	���ܣ�����DFA ID
	*/
	void				SetId(ulong ulId);

	/*	��������GetGroupCount
	**	���ܣ���ȡDFA��ת�ַ��ĸ���
	*/
	ushort				GetGroupCount() const;
	/*	��������SetGroups
	**	���ܣ�����ĳһDFA����ת�ַ�����
	*/
	void				SetGroups(byte *pGroup);
	/*	��������Char2Group
	**	���ܣ�ѹ���ַ�����ԭ�ַ���ΪASCII�룬��256����
	**	���ȼ۵��ַ��ϲ����࣬�����ַ����ĸ���
	*/
	byte				Char2Group(byte nIdx);

	/*	��������GetStartState
	**	���ܣ���ȡDFA��ʼ״̬id
	*/
	STATEID				GetStartState() const;
	/*	��������SetStartState
	**	���ܣ�����DFA��ʼ״̬id
	*/
	void				SetStartState(STATEID id);
	/*	��������GetFinalStates
	**	���ܣ���ȡDFA��̬����
	*/
	CFinalStates&		GetFinalStates();
	const CFinalStates&	GetFinalStates() const;

	/*	��������FromNFA
	**	������CNfa���͵�һ��NFA
	**	NFAת��ΪDFA�������Ӽ������㷨
	*/
	ulong				FromNFA(const CNfa &nfa);
	/*	��������Minimize
	**	���ܣ���С��DFA��״̬��������Hopcrof�㷨��1976��
	*/
	ulong				Minimize();

	/*	��������CalcStoreSize
	**	���ܣ�����һ��DFA��Ҫ�Ĵ洢�ռ��С
	*/
	ulong				CalcStoreSize() const;
	/*	��������Save
	**	���ܣ���һ��DFA��unsigned char*���ʹ����ڴ�
	*/
	void				Save(byte *beg);
	/*	��������Load
	**	���ܣ����ڴ��ȡһ��DFA������unsigned char*���ͱ�����
	*/
	void				Load(byte *beg);

	/*	��������Process
	**	������ByteStream-�������ݰ��Ķ������ֽ�����len-���ݰ����ȣ�StaSet-CDfa״̬����
	**	���ܣ��������ݰ���DFA��ƥ�����
	*/
	ulong				Process(byte *ByteStream, ulong len, STATEARY &StaSet);
	/*	��������Dump
	**	������pFile-����ļ�·��
	**	���ܣ���һ��DFA������ļ���
	*/
	void				Dump(pcstr pFile);

protected:
	ulong				PartStates(STATEVEC *pRevTbl);

protected:
	ulong				m_nId;
	ushort				m_nColNum;
	STATEID				m_nStartId;
	DFAROWARY*			m_pDfa;
	CFinalStates		m_FinStas;
	byte				m_pGroup[SC_DFACOLCNT];
};

/*	��������MergeMultipleDfas
**	������inputDfas-������DFA��mergedDfa-���һ���ϲ�DFA
**	���ܣ��������ж��DFA�ϲ�Ϊһ��DFA
*/
DFAHDR bool MergeMultipleDfas(class CDfaArray &inputDfas, CDfa &mergedDfa);

/*	��������PrintDfaToGv
**	������newdfa-����DFA��fileName-����ļ�·��
**	���ܣ�DFA�洢Ϊ����ṹ����DFA����ļ�����
*/
DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName);

