/*!
* @file		regrule.h
**
* @author		Lab 435, Xidian University
**
* @brief			Declaration of the CRegRule class
**
* A rule includes multiple CPcreChain, all store in CRegRule
**
*/

#pragma once

#include <hwprj\pcrechain.h>

#ifndef SNORTRULEHDR_DS
class CHAINVEC;
#endif

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

	CPcreChain& operator[] (ulong nIdx);
	const CPcreChain& operator[] (ulong nIdx) const;

	/// @brief 得到所包含的CPcreChain对象的数量。
	/// @return CPcreChain对象的数量。
	ulong				Size() const;
	void Erase(ulong ulIdx);
	void Reserve(ulong nCount);
	void Resize(ulong nSize);
	CPcreChain& Back() const;
	void PushBack(const CPcreChain &nRegChain);
	void PopBack();

protected:
	CHAINVEC*			m_pRegVec;
};
