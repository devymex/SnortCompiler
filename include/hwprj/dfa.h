/*!
* @file			dfa.h
* @author		Lab 435, Xidian University
* @brief		定义了CDfa类，用以表示DFA的数据结构
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
* @brief		实现DFA数据结构及相关算法
* @remark		封装了CDfaRow的动态数组、始态编号以及终态集合等。实现的算法包括：
*				nfa的确定化、dfa的最小化、字符集的压缩等。
*/
class DFAHDR CDfa
{
public:
	/// @brief		构造函数。
	CDfa();

	/// @brief		拷贝构造函数。
	CDfa(const CDfa &other);

	/// @brief		析构函数。
	virtual ~CDfa();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个CDfa对象。
	/// @return		返回自身对象的引用。
	CDfa& operator =	(const CDfa &other);

	/// @brief		重载的 '[]' 运算符，取得DFA状态编号相应的CDfaRow对象的引用。
	/// @param[in]	nIdx DFA状态编号
	/// @return		返回状态编号对应的CDfaRow对象的引用。
	CDfaRow& operator [] (STATEID nIdx);

	/// @brief		重载的 '[]' 运算符，取得DFA状态编号相应的CDfaRow对象的引用。
	/// @param[in]	nIdx DFA状态编号
	/// @return		返回状态编号对应的CDfaRow对象的引用（常量）。
	const CDfaRow& operator [] (STATEID nIdx) const;

	/// @brief		得到DFA当前的大小。
	/// @return		DFA当前的大小（常量）。
	ulong Size() const;

	/// @brief		得到DFA最后一个状态的引用。
	/// @return		最后一个状态编号对应的CDfaRow对象的引用。
	CDfaRow& Back();

	/// @brief		为DFA预留内存空间。
	/// @param		ulSize 指定的预留的空间。
	void Reserve(ulong ulSize);

	/// @brief		更改DFA的大小。
	/// @param[in]	ulSize 更改的尺寸。
	/// @param[in]	ulColNum DFA跳转字符集的长度
	/// @remark		如果ulSize大于或等于当前尺寸，则不会更改现有元素；
	///				否则会删除多出部分的元素。
	void Resize(ulong ulSize, ulong ulColNum);

	/// @brief		清空DFA
	void Clear();

	/// @brief		将指定的DFA状态添加为DFA跳转表最后一个元素。
	/// @param		sta 指定的DFA状态。
	void PushBack(CDfaRow &sta);

	/// @brief		获取DFA的编号。
	ulong GetId();

	/// @brief		设置DFA的编号。
	/// @param[in]	ulId DFA编号。
	void SetId(ulong ulId);

	/// @brief		获取DFA跳转字符集的大小。
	ushort GetGroupCount() const;

	/// @brief		设置一个DFA的跳转字符集合
	/// @param[in]	pGroup 压缩后的字符集
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

	/// @brief		NFA转化为DFA，执行确定化操作，采用子集构造算法。
	/// @param[in]	一个CNfa对象。
	ulong FromNFA(const CNfa &nfa);

	/// @brief		最小化DFA的状态数，采用Hopcrof算法（Hopcrof1976）
	ulong Minimize();

	/// @brief		NFA转化为DFA后，再一次压缩字符集，以减少存储空间。
	///	@todo		此函数未完成。
	void MergeColumn();

	/// @brief		计算一个DFA需要的存储空间大小。
	ulong CalcStoreSize() const;

	/// @brief		将一个DFA以unsigned char*类型存入内存。
	/// @param[in]	beg DFA的二进制字节流
	void Save(byte *beg);

	/// @brief		从内存读取一个DFA，存入unsigned char*类型变量。
	/// @param[in]	beg DFA的二进制字节流
	void Load(byte *beg);

	/// @brief		处理数据包与DFA的匹配过程。
	/// @param[in]	ByteStream 输入数据包的二进制字节流。
	/// @param[in]	len 数据包长度。
	/// @param[out]	StaSet CDfa状态数组。
	ulong Process(byte *ByteStream, ulong len, STATEARY &StaSet);

	/// @brief		将一个DFA输出到文件中。
	/// @param[in]	pFile 输出文件路径。
	void Dump(pcstr pFile);

private:
	/// @brief		私有成员，仅供内部使用。
	ulong PartStates(STATEIDARY *pRevTbl);

private:
	/// @brief		私有成员，仅供内部使用。
	ulong m_nId;

	/// @brief		私有成员，仅供内部使用。
	ushort m_usColNum;

	/// @brief		私有成员，仅供内部使用。
	STATEID m_nStartId;

	/// @brief		私有成员，仅供内部使用。
	DFAROWARY* m_pDfa;

	/// @brief		私有成员，仅供内部使用。
	CFinalStates m_FinStas;

	/// @brief		私有成员，仅供内部使用。
	byte m_pGroup[SC_DFACOLCNT];
};

/// @brief		将分组中多个DFA合并为一个DFA。
/// @param[in]	nputDfas 输入多个DFA。
/// @param[out] mergedDfa 输出一个合并DFA。
DFAHDR bool MergeMultipleDfas(class CDfaArray &inputDfas, CDfa &mergedDfa);

/// @brief		将一个DFA对象输出为gv格式，以供GraphVis软件显示之用。
/// @param[in]	newdfa 输入的CDfa对象。
/// @param[in]	fileName 输出文件名，以'\0'为结尾的字符串指针。
DFAHDR void PrintDfaToGv(CDfa &newdfa, const char* fileName);

/*!
@}
*/
