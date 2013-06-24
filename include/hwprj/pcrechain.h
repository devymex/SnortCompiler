/*!
* @file			pcrechain.h
* @author		Lab 435, Xidian University
* @brief		定义了CPcreChain类，根据Snort规则选项的属性，构造PCRE链
*/

#pragma once

#include <hwprj\pcreopt.h>

#ifndef SNORTRULEHDR_DS
class PCREVEC;
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief		实现PCRE链的数据结构及相关算法
* @remark		封装了std::vector<class CPcreOption>对象以及Signature数组
*/
class SNORTRULEHDR CPcreChain
{
public:
	/// @brief		构造函数。
	CPcreChain();

	/// @brief		拷贝构造函数。
	CPcreChain(const CPcreChain &other);

	/// @brief		析构函数。
	virtual ~CPcreChain();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		返回自身对象的引用。
	CPcreChain& operator = (const CPcreChain &other);

	/// @brief		重载的 '[]' 运算符，取得指定位置的状态集数组。
	/// @param[in]	nIdx 指定位置的下标。
	/// @return		返回指定位置的状态集数组。
	CPcreOption& operator[] (ulong nIdx);

	/// @brief		重载的 '[]' 运算符，取得指定位置的CPcreOption对象。
	/// @param[in]	nIdx 指定位置的下标。
	/// @return		返回指定位置的状态集数组。
	const CPcreOption& operator[] (ulong nIdx) const;

	/// @brief		得到所包含的元素数量。
	/// @return		所包含的元素数量。
	ulong Size() const;

	/// @brief
	void Clear();

	/// @brief
	CPcreOption& Back() const;

	/// @brief
	void PushBack(const CPcreOption &pcreOpt);

	/// @brief
	void Resize(ulong nSize);

	/// @brief		获取PCRE链中的signature。
	CUnsignedArray& GetSigs();

	/// @brief		获取PCRE链中的signature（常量）。
	const CUnsignedArray& GetSigs() const;

private:
	/// @brief		私有成员，仅供内部使用。
	PCREVEC*				m_pPcreVec;

	/// @brief		私有成员，仅供内部使用。
	CUnsignedArray			m_sigs;
};

/*!
@}
*/
