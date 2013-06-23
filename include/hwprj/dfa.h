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
* @brief		实现DFA数据结构及相关算法
* 封装了CDfaRow的动态数组、始态编号以及终态集合等。实现的算法包括：nfa的确定化、\
* dfa的最小化、字符集的压缩等。
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

	/// @brief		获取DFA的编号。
	ulong				GetId();

	/// @brief		设置DFA的编号。
	/// @param[in] ulId DFA编号。
	void SetId(ulong ulId);

	/// @brief		获取DFA字符集的大小。
	ushort				GetGroupCount() const;

	/// @brief		设置一个DFA的跳转字符集合
	/// @param[in] pGroup 分类后的字符集
	void SetGroups(byte *pGroup);

	/// @brief		从标准ANSI字符集映射到DFA的字符集。
	/// @param[in]	nIdx 标准ANSI字符编码。
	/// @return		与指定字符编码对应的DFA字符。
	byte Char2Group(byte nIdx);

	/// @brief		获取DFA开始状态id
	STATEID GetStartState() const;

	/// @brief		设置DFA开始状态id
	/// @param[in]	id DFA状态编号
	void SetStartState(STATEID id);

	/// @brief		获取DFA终态集合
	CFinalStates& GetFinalStates();

	/// @brief		获取DFA终态集合（常量）
	const CFinalStates& GetFinalStates() const;

	/// @brief		NFA转化为DFA，采用子集构造算法
	/// @param[in]	CNfa类型的一个NFA
	ulong FromNFA(const CNfa &nfa);

	/// @brief		最小化DFA的状态数，采用Hopcrof算法（1976）
	ulong Minimize();

	/// @brief		NFA转化为DFA后，再一次压缩字符集，以减少存储空间
	void MergeColumn();

	/// @brief		计算一个DFA需要的存储空间大小
	ulong CalcStoreSize() const;

	/// @brief		将一个DFA以unsigned char*类型存入内存
	/// @param[in]	beg DFA的二进制字节流
	void Save(byte *beg);

	/// @brief		从内存读取一个DFA，存入unsigned char*类型变量
	/// @param[in]	beg DFA的二进制字节流
	void Load(byte *beg);

	/// @brief		处理数据包与DFA的匹配过程
	/// @param[in]	ByteStream 输入数据包的二进制字节流。
	/// @param[in]	len 数据包长度。
	/// @param[out]	StaSet CDfa状态数组。
	ulong Process(byte *ByteStream, ulong len, STATEARY &StaSet);

	/// @brief		将一个DFA输出到文件中
	/// @param[in]	pFile 输出文件路径
	void Dump(pcstr pFile);

protected:
	/// @brief		私有成员，仅供内部使用
	ulong PartStates(STATEIDARY *pRevTbl);

protected:
	/// @brief		私有成员，仅供内部使用
	ulong m_nId;

	/// @brief		私有成员，仅供内部使用
	ushort m_usColNum;

	/// @brief		私有成员，仅供内部使用
	STATEID m_nStartId;

	/// @brief		私有成员，仅供内部使用
	DFAROWARY* m_pDfa;

	/// @brief		私有成员，仅供内部使用
	CFinalStates m_FinStas;

	/// @brief		私有成员，仅供内部使用
	byte m_pGroup[SC_DFACOLCNT];
};

/// @brief		将分组中多个DFA合并为一个DFA
/// @param[in] nputDfas 输入多个DFA
/// @param[out] mergedDfa 输出一个合并DFA
DFAHDR bool MergeMultipleDfas(class CDfaArray &inputDfas, CDfa &mergedDfa);

/// @brief		DFA存储为矩阵结构，将DFA输出文件保存
/// @param[in] newdfa 输入DFA。
/// @param[in] fileName 输出文件路径
DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName);

