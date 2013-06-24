/*!
* @file			dllstring.h
* @author		Lab 435, Xidian University
* @brief		定义了CDllString类及其导出符号
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

#include <hwprj\common.h>

#ifndef DLLSTRHDR_DS
class DLLSTRING;
#define DLLSTRHDR __declspec(dllimport)
#else
#define DLLSTRHDR __declspec(dllexport)
#endif

/*!
* @addtogroup groupCommon
* @{
*/

/*!
* @brief		封装std::string，用于模块之间传递参数。
*/
class DLLSTRHDR CDllString
{
public:
	/// @brief		构造函数。
	CDllString();

	/// @brief		带参构造函数。
	/// @param[in]	pStr 以'\0'为结尾的ANSI字符串指针。
	explicit CDllString(pcstr pStr);

	/// @brief		拷贝构造函数。
	CDllString(const CDllString &other);

	/// @brief		析构函数。
	virtual ~CDllString();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		返回自身对象的引用。
	CDllString& operator = (const CDllString &other);

	char& operator[] (ulong nIdx) const;

	/// @brief		获得字符串的长度。
	/// @return		字符串中字符的个数。
	ulong Size() const;

	/// @brief		判断字符串是否为空。
	/// @retval		true 字符串为空。
	/// @retval		false 字符串非空。
	bool Empty() const;

	/// @brief		清空字符串。
	void Clear();

	/// @brief		删除指定位置的一个字符。
	/// @param[in]	ulIdx 指定要删除的位置。
	void Erase(ulong ulIdx);

	///	@brief		在指定位置插入一个字符。
	/// @param[in]	ulIdx 指定要插入的位置。
	/// @param[in]	nChar 指定要插入的字符。
	/// @remark		指定的字符将被插入到指定位置处字符的前面。
	void Insert(ulong ulIdx, char nChar);

	///	@brief		在指定位置插入一个字符串。
	/// @param[in]	ulIdx 指定要插入的位置。
	/// @param[in]	pChar 指定要插入的字符串。
	/// @remark		指定的字符串将被插入到指定位置处字符的前面。
	void Insert(ulong ulIdx, pcstr pChar);

	///	@brief		将所存储的字符串赋值为指定的字符串。
	/// @param[in]	pStr 指定的以'\0'结尾的字符串。
	void Assign(const char* pStr);

	///	@brief		将指定的另一个字符串添加到本字符串的末尾。
	/// @param[in]	other 指定的另一个字符串对象。
	void Append(const CDllString &other);

	/// @brief		获取字符串的最后一个字符。
	/// @return		字符串的最后一个字符。
	char Back() const;

	/// @brief		将指定字符添加到字符串的末尾。
	/// @param		
	void PushBack(char nChar);
	const char* Data() const;

protected:
	/// @brief		私有成员，仅供内部使用。
	DLLSTRING* m_pString;
};

/*!
@}
*/
