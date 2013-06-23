/*!
* @file			byteary.h
* @author		Lab 435, Xidian University
* @brief		Common string class
* @remark		Definition of the CDllString class
*/

#pragma once

#include <hwprj\common.h>

#ifndef BYTEARYHDR_DS
class DLLBYTEARY;
#define BYTEARYHDR __declspec(dllimport)
#else
#define BYTEARYHDR __declspec(dllexport)
#endif

/*!
* @brief		封装了vector<unsigned char>，作为该类的代理
*/
class BYTEARYHDR CByteArray
{
public:
	/// @brief		构造函数
	CByteArray();

	/// @brief		拷贝构造函数
	/// @param[in]	other 指定的另一个对象
	CByteArray(const CByteArray &other);

	/// @brief		析构函数
	virtual ~CByteArray();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		返回自身对象的引用。
	CByteArray& operator = (const CByteArray &other);

	byte& operator [] (ulong ulIdx);
	const byte& operator [] (ulong ulIdx) const;

	/// @brief		返回数组的大小。
	/// @return		字节的数量。
	ulong Size() const;
	bool Empty() const;
	void Clear();
	void PushBack(byte byVal);
	void PopBack();
	void Resize(ulong ulSize);
	void Reserve(ulong ulCap);
	byte* Data();
	const byte* Data() const;

protected:
	/// @brief		私有成员，仅供内部使用
	DLLBYTEARY* m_pAry;
};
