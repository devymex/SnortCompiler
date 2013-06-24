/*!
* @file			finalstates.h
* @author		Lab 435, Xidian University
* @brief		定义了CFinalStates类，用于存储DFA状态的ID与DFAID的映射关系
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
* @brief		DFA的终态集合以及终态对应的DFAID集合
*/
class DFAIDSETHDR CFinalStates
{
public:
	/// @brief		构造函数。
	CFinalStates();

	/// @brief		拷贝构造函数。
	CFinalStates(const CFinalStates &other);

	/// @brief		析构函数。
	virtual ~CFinalStates();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个CFinalStates对象。
	/// @return		返回自身对象的引用。
	CFinalStates& operator = (const CFinalStates &other);

	/// @brief		重载的 '[]' 运算符，取得指定位置的DFA状态编号。
	/// @param[in]	nIdx 指定位置的下标。
	/// @return		返回指定位置的DFA状态编号。
	STATEID operator [] (ulong nIdx) const;

	/// @brief		得到CFinalStates所包含的元素数量。
	/// @return		CFinalStates所包含的元素数量。
	ulong Size() const;

	/// @brief		删除CFinalStates中所有的终态及其对应的DFA编号集合。
	void Clear();

	/// @brief		终态集合中增加DFA终态的编号。
	/// @param[in]	nStaId 终态编号。
	/// @return		新加入的终态所对应的CDfaIdSet对象的引用。
	CDfaIdSet& AddState(STATEID nStaId);

	/// @brief		将另一个CDfaIdSet替换当前的CDfaIdSet。
	/// @param[in]	other 另一个CDfaIdSet对象。
	void Swap(CFinalStates &other);

	/// @brief		获取DFA终态对应的CDfaIdSet对象的引用。
	/// @param[in]	nStaId 状态编号。
	/// @return		CDfaIdSet对象的引用。
	CDfaIdSet& GetDfaIdSet(STATEID nStaId);

	/// @brief		获取DFA终态对应的CDfaIdSet对象的引用（常量）。
	/// @param[in]	nStaId 状态编号。
	/// @return		CDfaIdSet对象的引用（常量）。
	const CDfaIdSet& GetDfaIdSet(STATEID nStaId) const;

	/// @brief		计算全部终态所对应的DFAID的总数。
	/// @return		DFAID总数。
	ulong CountDfaIds() const;

private:
	/// @brief		私有成员，仅供内部使用
	STATEIDARY* m_pStates;

	/// @brief		私有成员，仅供内部使用
	FINSTAMAP* m_pDfaIds;
};

/*!
@}
*/