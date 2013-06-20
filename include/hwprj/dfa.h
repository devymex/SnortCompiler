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

	/*	函数名：GetId
	**	功能：获取DFA ID
	*/
	ulong				GetId();
	/*	函数名：SetId
	**	功能：设置DFA ID
	*/
	void				SetId(ulong ulId);

	/*	函数名：GetGroupCount
	**	功能：获取DFA跳转字符的个数
	*/
	ushort				GetGroupCount() const;
	/*	函数名：SetGroups
	**	功能：设置某一DFA的跳转字符集合
	*/
	void				SetGroups(byte *pGroup);
	/*	函数名：Char2Group
	**	功能：压缩字符集，原字符集为ASCII码，共256个，
	**	将等价的字符合并分类，减少字符集的个数
	*/
	byte				Char2Group(byte nIdx);

	/*	函数名：GetStartState
	**	功能：获取DFA开始状态id
	*/
	STATEID				GetStartState() const;
	/*	函数名：SetStartState
	**	功能：设置DFA开始状态id
	*/
	void				SetStartState(STATEID id);
	/*	函数名：GetFinalStates
	**	功能：获取DFA终态集合
	*/
	CFinalStates&		GetFinalStates();
	const CFinalStates&	GetFinalStates() const;

	/*	函数名：FromNFA
	**	参数：CNfa类型的一个NFA
	**	NFA转化为DFA，采用子集构造算法
	*/
	ulong				FromNFA(const CNfa &nfa);
	/*	函数名：Minimize
	**	功能：最小化DFA的状态数，采用Hopcrof算法（1976）
	*/
	ulong				Minimize();

	/*	函数名：CalcStoreSize
	**	功能：计算一个DFA需要的存储空间大小
	*/
	ulong				CalcStoreSize() const;
	/*	函数名：Save
	**	功能：将一个DFA以unsigned char*类型存入内存
	*/
	void				Save(byte *beg);
	/*	函数名：Load
	**	功能：从内存读取一个DFA，存入unsigned char*类型变量中
	*/
	void				Load(byte *beg);

	/*	函数名：Process
	**	参数：ByteStream-输入数据包的二进制字节流，len-数据包长度，StaSet-CDfa状态数组
	**	功能：处理数据包与DFA的匹配过程
	*/
	ulong				Process(byte *ByteStream, ulong len, STATEARY &StaSet);
	/*	函数名：Dump
	**	参数：pFile-输出文件路径
	**	功能：将一个DFA输出到文件中
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

/*	函数名：MergeMultipleDfas
**	参数：inputDfas-输入多个DFA，mergedDfa-输出一个合并DFA
**	功能：将分组中多个DFA合并为一个DFA
*/
DFAHDR bool MergeMultipleDfas(class CDfaArray &inputDfas, CDfa &mergedDfa);

/*	函数名：PrintDfaToGv
**	参数：newdfa-输入DFA，fileName-输出文件路径
**	功能：DFA存储为矩阵结构，将DFA输出文件保存
*/
DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName);

