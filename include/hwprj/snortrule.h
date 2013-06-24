/*!
* @file			snortrule.h
* @author		Lab 435, Xidian University
* @brief		定义CSnortRule类
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
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
* @brief	提取Snort规则选项内容。
* @remark	封装std::vector<class CRuleOption*>对象，用于记录规则选项内容，
*			标记能够处理的规则和规则对应的sid
*/
class SNORTRULEHDR CSnortRule
{
public:
	typedef ulong PARSE_INFO;

	///@brief		一般规则。
	static const PARSE_INFO NORMAL		= 0;

	/// @brief		该规则包含byte_test或byte_jump。
	static const PARSE_INFO HASBYTE		= (1 << 0);

	/// @brief		 该规则包含'!'（逻辑非）标识。
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

	/// @brief		重载的 '[]' 运算符，取得指定位置的规则选项的引用。
	/// @param[in]	nIdx 指定位置的下标
	/// @return		返回指定位置的规则选项的引用（常量）。
	CRuleOption* operator[](ulong nIdx) const;

	/// @brief		得到CSnortRule当前的大小。
	/// @return		CSnortRule当前的大小（常量）。
	ulong Size() const;

	/// @brief		清空CSnortRule。
	void Clear();

	/// @brief		将指定的CRuleOption对象添加为CSnortRule最后一个元素。
	/// @param[in]	pRuleOption 指定的CRuleOption对象。
	void PushBack(CRuleOption* pRuleOption);

	/// @brief		删除CSnortRule的最后一个元素。
	void PopBack();

	/// @brief		得到CSnortRule数组最后一个pRuleOption对象的引用。
	/// @return		最后一个pRuleOption对象的引用。
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
