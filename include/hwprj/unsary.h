/*!
* @file			unsary.h
* @author		Lab 435, Xidian University
* @brief		定义了CUnsignedArray类及其导出符号
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\trace.h>

#ifndef UNSARYHDR_DS
#define UNSARYHDR __declspec(dllimport)
#else
#define UNSARYHDR __declspec(dllexport)
#endif

/*!
* @brief	实现(ulong*)的动态数组。
* @remark	用于替代std::vector<unsigned long>，在模块之间传递参数。
*/
class UNSARYHDR CUnsignedArray
{
public:
	/// @brief		构造函数。
	inline CUnsignedArray();

	/// @brief		拷贝构造函数。
	inline CUnsignedArray(const CUnsignedArray &other);

	/// @brief		析构函数。
	inline virtual ~CUnsignedArray();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		返回自身对象的引用。
	inline CUnsignedArray& operator = (const CUnsignedArray &other);

	/// @brief		重载的 '[]' 运算符，取得指定位置的值的引用。
	/// @param[in]	nIdx 指定位置的下标
	/// @return		返回指定位置元素的引用。
	inline ulong& operator [] (ulong nIdx);

	/// @brief		重载的 '[]' 运算符，取得指定位置的值的引用。
	/// @param[in]	nIdx 指定位置的下标
	/// @return		返回指定位置元素的引用（常量）。
	inline const ulong& operator [] (ulong nIdx) const;

	/// @brief		重载的 '==' 运算符，判断自身对象是否与所指定的对象相同。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		两个动态数组相等时返回true，否则返回false。
	/// @remark		将会执行逐元素的比较。
	inline bool operator == (const CUnsignedArray &other) const;

	/// @brief		重载的 '<' 运算符，判断自身对象是否比所指定的对象小。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		当前的动态数组比另一个动态数组小时返回true，否则返回false。
	/// @remark		将会执行逐元素的比较。若二者长短不同，且比较至较短数组的末尾之后，
	///				较长的数组为大。
	inline bool operator < (const CUnsignedArray &other) const;

	/// @brief		得到动态数组当前的大小。
	/// @return		动态数组当前的大小。
	inline ulong Size() const;

	/// @brief		得到动态数组最后一个元素的引用。
	/// @return		最后一个元素的引用。
	inline ulong& Back();

	/// @brief		清空数组的所有元素。
	inline void Clear();

	/// @brief		删除数组的最后一个元素。
	inline void PopBack();

	/// @brief		将指定的数值添加为数组的最后一个元素。
	/// @param		ulval 指定的数值。
	inline void PushBack(ulong ulVal);

	/// @brief		为动态数组预留内存空间。
	/// @param		ulCap 指定的预留的空间。
	inline void Reserve(ulong ulCap);

	/// @brief		更改动态数组的大小。
	/// @param[in]	ulSize 更改的尺寸。
	/// @remark		如果ulSize大于或等于当前数组尺寸，则不会更改现有元素；
	///				否则会删除多出部分的元素。
	inline void Resize(ulong ulSize);

	/// @brief		更改动态数组的大小。
	/// @param[in]	ulSize 更改的尺寸。
	/// @remark		如果ulSize大于或等于当前数组尺寸，则不会更改现有元素并为新增元素赋值为ulVal；
	///				否则会删除多出部分的元素。
	inline void Resize(ulong ulSize, ulong ulVal);

	/// @brief		返回数组的数据指针
	/// @return		数组的数据指针，指向第一个元素
	inline ulong* Data();

	/// @brief		返回数组的数据指针
	/// @return		数组的数据指针（常量），指向第一个元素
	inline const ulong* Data() const;

	/// @brief		将数组中的所有元素赋值为指定数值
	/// @param[in]	ulVal 指定的数值。
	inline void Fill(ulong ulVal);

	/// @brief		在数组中查找指定的数值。
	/// @param[in]	ulVal 待查找的数值。
	/// @return		若找到，则返回找到的元素的下标，否则返回ulong(-1)。
	inline ulong Find(ulong ulVal) const;

	/// @brief		排序数组中的所有元素，利用std::sort
	void Sort();

	/// @brief		使数组中的所有元素唯一化，利用std::unique
	void Unique();

protected:
	/// @brief		分非指定大小的内存，用来替换new。
	inline ulong* Alloc(ulong ulCap) const;

	/// @brief		释放指定的内存，用来替换delete[]。
	inline void Free(ulong *pBuf) const;

	/// @brief		重新分配指定大小。
	inline void Realloc(ulong ulCap);

	/// @brief		从指定的内存中拷入数据。
	void CopyFrom(ulong *pBuf);

private:
	/// @brief		私有成员，仅供内部调用。
	ulong* m_pAry;

	/// @brief		私有成员，仅供内部调用。
	ulong m_ulSize;

	/// @brief		私有成员，仅供内部调用。
	size_t m_ulCap;
};

#include <hwprj\unsary.inl>
