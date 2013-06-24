/*!
* @file				contopt.h
**
* @author			Lab 435, Xidian University
**
* @brief			Declaration of the CContentOption class
**
* inherit CRuleOption class
* process “content” attribute in a Snort rule
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\ruleoption.h>
#include <hwprj\pcreopt.h>
#include <hwprj\byteary.h>

class CContentOption : public CRuleOption
{
public:
	static const OPTIONFLAG NOCASE		= (1 << 2);		/// 忽略大小写
	static const OPTIONFLAG OFFSET		= (1 << 3);		/// 带有offset修饰项
	static const OPTIONFLAG DEPTH		= (1 << 4);		/// 带有depth修饰项
	static const OPTIONFLAG DISTANCE	= (1 << 5);		/// 带有distance修饰项
	static const OPTIONFLAG WITHIN		= (1 << 6);		/// 带有within修饰项

public:
	/// @brief		构造函数。
	CContentOption();

	/// @brief		拷贝构造函数。
	CContentOption(const CContentOption &other);

	/// @brief		析构函数。
	virtual ~CContentOption();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		返回自身对象的引用。
	CContentOption& operator = (const CContentOption &other);

	/// @brief		将content及其修饰选项转换为PCRE链
	/// @param		pcreOpt PCRE链类型
	void ToPcre(CPcreOption &pcreOpt) const;

	/// @brief		解析Snort规则中的content选项及其属性
	/// @param		strPat Snort规则中content及其属性字符串
	virtual void FromPattern(const CDllString &strPat);

	/// @brief		复制当前content选项中内容
	virtual CRuleOption* Clone() const;

public:
	/// @brief		私有成员，仅供内部使用
	int m_nOffset;

	/// @brief		私有成员，仅供内部使用
	int m_nDepth;

	/// @brief		私有成员，仅供内部使用
	int m_nDistance;

	/// @brief		私有成员，仅供内部使用
	int m_nWithin;

protected:
	/// @brief		私有成员，仅供内部使用
	CByteArray m_data;
};
