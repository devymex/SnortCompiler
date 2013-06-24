/*!
* @file			regrule.h
* @author		Lab 435, Xidian University
* @brief		定义CRegRule类
*/

#pragma once

#include <hwprj\pcrechain.h>

#ifndef SNORTRULEHDR_DS
class CHAINVEC;
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief 实现了规格化的snort规则数据结构，提供对一条规则的数据进行管理的功能。
* @remark 主要数据结构为CPcreChain的动态数组，封装了std::vector<CPcreChain>。
*/
class SNORTRULEHDR CRegRule
{
public:
	/// @brief		构造函数。
	CRegRule();

	/// @brief		拷贝构造函数。
	CRegRule(const CRegRule &other);

	/// @brief		析构函数。
	virtual ~CRegRule();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		返回自身对象的引用。
	CRegRule& operator = (const CRegRule &other);

	/// @brief		获得指定位置的一条PCRE链。
	/// @param[in]	nIdx 指定的PCRE链的位置。
	/// @return		一个CPcreChain对象的引用。
	CPcreChain& operator[] (ulong nIdx);

	/// @brief		获得指定位置的一条PCRE链（常量）。
	/// @param[in]	nIdx 指定的PCRE链的位置。
	/// @return		一个CPcreChain对象的引用（常量）。
	const CPcreChain& operator[] (ulong nIdx) const;

	/// @brief		得到所包含的CPcreChain对象的数量。
	/// @return		CPcreChain对象的数量。
	ulong Size() const;

	/// @brief		删除指定位置的PCRE链。
	/// @param[in]	nIdx 指定的PCRE链的位置。
	void Erase(ulong ulIdx);

	/// @brief		预留指定大小的数组空间。
	/// @param[in]	nCap 指定预留空间的大小。
	void Reserve(ulong nCap);

	/// @brief		更改数组的大小。
	/// @param[in]	nSize 指定的PCRE链的个数
	void Resize(ulong nSize);

	/// @brief		获得数组中最后一个PCRE链。
	/// @return		一个CPcreChain对象的引用。
	CPcreChain& Back();

	/// @brief		获得数组中最后一个PCRE链（常量）。
	/// @return		一个CPcreChain对象的引用（常量）。
	const CPcreChain& Back() const;

	/// @brief		将指定的PCRE链添加到数组的末尾。
	void PushBack(const CPcreChain &nRegChain);

	/// @brief		删除数组末尾的最后一条PCRE链。
	void PopBack();

protected:
	/// 私有成员，仅供内部使用。
	CHAINVEC* m_pRegVec;
};

/*!
@}
*/
