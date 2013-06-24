/*!
* @file				dllstring.h
**
* @author			Lab 435, Xidian University
**
* @brief			Common string class
**
* Definition of the CDllString class
**
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
* @brief		封装std::string，用于模块之间传递参数
*/
class DLLSTRHDR CDllString
{
public:
	/// @brief		构造函数
	CDllString();

	/// @brief		带参构造函数
	/// @param[in]	pStr 以'\0'为结尾的ANSI字符串指针
	explicit CDllString(pcstr pStr);

	/// @brief		拷贝构造函数
	CDllString(const CDllString &other);

	/// @brief		析构函数
	virtual ~CDllString();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个动态数组对象。
	/// @return		返回自身对象的引用。
	CDllString& operator = (const CDllString &other);


	char			operator[]	(ulong nIdx) const;

	ulong			Size() const;
	bool			Empty() const;
	void Clear();
	void Erase(ulong ulIdx);
	void Insert(ulong ulIdx, char nChar);
	void Insert(ulong ulIdx, pcstr pChar);
	void Assign(const char* pStr);
	void Append(const CDllString &other);
	char			Back() const;
	void PushBack(char nChar);
	const char*		Data() const;

protected:
	/// @brief		私有成员，仅供内部使用
	DLLSTRING*		m_pString;
};
