/*!
* @file			snortrule.h
* @author		Lab 435, Xidian University
* @brief		定义CSnortRule类
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\ruleoption.h>

#ifndef SNORTRULEHDR_DS
class OPTIONVEC;
#endif

/*! @addtogroup groupCompiler
* @{
*/

/*!
* @brief
*/
class SNORTRULEHDR CSnortRule
{
public:
	typedef ulong PARSE_INFO;

	/// 一般规则。
	static const PARSE_INFO NORMAL		= 0;
	/// 该规则包含byte_test或byte_jump。
	static const PARSE_INFO HASBYTE		= (1 << 0);
	/// 该规则包含'!'（逻辑非）标识。
	static const PARSE_INFO HASNOT		= (1 << 1);

public:
	/// @brief		构造函数。
	CSnortRule();

	/// @brief		拷贝构造函数。
	CSnortRule(const CSnortRule &other);

	/// @brief		析构函数。
	virtual ~CSnortRule();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		返回自身对象的引用。
	CSnortRule& operator = (const CSnortRule &other);

	/// @brief
	CRuleOption* operator[](ulong nIdx) const;

	/// @brief
	ulong Size() const;

	/// @brief		删除数组中所有的规则选项对象，并释放它们的内存。
	void Clear();

	/// @brief		将一个CRuleOption基类的指针添加到数组的末尾。
	/// @param		pRuleOption 一个CRuleOption基类的指针，可指向其派生类。
	void PushBack(CRuleOption* pRuleOption);

	/// @brief		删除数组中最后一个选项，并释放其内存。
	void PopBack();

	/// @brief		获取数组中最后一个选项的指针。
	/// @return		一个CRuleOption基类的指针。可能指向其派生类。
	CRuleOption* Back();

	/// @brief		获取规则sid。
	ulong GetSid() const;

	/// @brief		设置规则sid。
	/// @param[in]	sid 规则编号。
	void SetSid(ulong sid);

	/// @brief		获取规则标记。
	PARSE_INFO GetFlags() const;

	/// @brief		增加规则标记。
	/// @param[in]	nFlags 规则标记。
	void AddFlags(PARSE_INFO nFlags);

	/// @brief		设置规则标记，用于标记该规则是否处理，规则选项内容中包含取非运算的。
	/// @param[in]	nFlags 规则标记。
	void SetFlags(PARSE_INFO nFlags);

	/// @brief		判断该规则是否包含标记。
	/// @param[in]	nFlags 规则标记。
	bool HasFlags(PARSE_INFO nFlags) const;

private:
	/// @brief		私有成员，仅供内部使用。
	ulong m_nSid;

	/// @brief		私有成员，仅供内部使用。
	PARSE_INFO m_nFlags;

	/// @brief		私有成员，仅供内部使用。
	OPTIONVEC* m_pOptions;
};

/*!
@}
*/
