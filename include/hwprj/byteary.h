/*!
* @file			byteary.h
* @author		Lab 435, Xidian University
* @brief		定义了CByteArry类及其导出符号。
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
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
* @brief		封装了vector<unsigned char>，用于在模块之间传递参数。
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

	/// @brief		重载的 '[]' 运算符，取得指定位置的值的引用。
	/// @param[in]	nIdx 指定位置的下标
	/// @return		返回指定位置元素的引用。
	byte& operator [] (ulong ulIdx);

	/// @brief		重载的 '[]' 运算符，取得指定位置的值的引用（常量）。
	/// @param[in]	nIdx 指定位置的下标
	/// @return		返回指定位置元素的引用（常量）。
	const byte& operator [] (ulong ulIdx) const;

	/// @brief		返回数组的大小。
	/// @return		字节的数量。
	ulong Size() const;

	/// @brief		判断数组是否为空
	/// @return		返回true表示数组为空，返回false表示数组非空。
	bool Empty() const;

	/// @brief		清空数组
	void Clear();

	/// @brief		在数组的末尾添加一个字节的数据
	/// @param		byVal 添加的字节值
	void PushBack(byte byVal);

	/// @brief		删除数组末尾的一个参数
	void PopBack();

	/// @brief		更改数组的大小，执行std::vector::resize操作。
	void Resize(ulong ulSize);

	/// @brief		更改数组预留空间的大小，执行std::vector::reserve操作。
	void Reserve(ulong ulCap);

	/// @brief		返回数组的数据指针。
	/// @return		数组第一个字节的指针。
	byte* Data();

	/// @brief		返回数组的数据指针（常量）。
	/// @return		数组第一个字节的指针（常量）。
	const byte* Data() const;

private:
	/// @brief		私有成员，仅供内部使用
	DLLBYTEARY* m_pAry;
};
