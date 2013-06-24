/*!
* @file				siddfas.h
**
* @author			Lab 435, Xidian University
**
* @brief			Declaration of the CSidDfaIds class
**
* Store the mapping between SID and DFAID
**
*/

#pragma once

#include <hwprj\unsary.h>

#ifndef SIDDFAHDR_DS
class COMPILEDINFOARY;
#define SIDDFAHDR __declspec(dllimport)
#else
#define SIDDFAHDR __declspec(dllexport)
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief		存储一条规则的处理信息。
* @remark		包括处理结果、规则id以及DFA。
*/

struct COMPILEDINFO
{
	typedef ulong COMPRETCODE;
	static const COMPRETCODE RES_SUCCESS		= 0;
	static const COMPRETCODE RES_PCREERROR		= (1 << 0); 
	static const COMPRETCODE RES_OPTIONERROR	= (1 << 1); 
	static const COMPRETCODE RES_HASBYTE		= (1 << 2); 
	static const COMPRETCODE RES_HASNOT			= (1 << 3); 
	static const COMPRETCODE RES_EMPTY			= (1 << 4);
	static const COMPRETCODE RES_HASNOSIG		= (1 << 5);
	static const COMPRETCODE RES_EXCEEDLIMIT	= (1 << 6);
	ulong m_nSid;
	ulong m_nResult;
	CUnsignedArray m_dfaIds;
};

/*!
* @brief		存储所有规则的处理信息。
*/

class SIDDFAHDR CSidDfaIds
{
public:
	/// @brief		构造函数。
	CSidDfaIds();

	/// @brief		拷贝构造函数。
	CSidDfaIds(const CSidDfaIds& other);

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个CSidDfaIds对象。
	/// @return		返回自身对象的引用。
	CSidDfaIds &operator=(const CSidDfaIds &other);

	/// @brief		析构函数。
	virtual ~CSidDfaIds();

	/// @brief		重载的 '[]' 运算符，取得指定位置的值的引用。
	/// @param[in]	nIdx 指定位置的下标
	/// @return		返回指定位置元素的引用。
	COMPILEDINFO& operator[](ulong index);

	/// @brief		重载的 '[]' 运算符，取得指定位置的值的引用（常量）。
	/// @param[in]	nIdx 指定位置的下标
	/// @return		返回指定位置元素的引用（常量）。
	const COMPILEDINFO& operator[](ulong index) const;

	/// @brief		更改vector预留空间的大小，执行std::vector::reserve操作。
	void Reserve(ulong nCount);

	/// @brief		更改vector的大小，执行std::vector::resize操作。
	void Resize(ulong nSize);

	/// @brief		返回vector的大小。
	/// @return		规则信息的数量。
	const ulong Size() const;

	/// @brief		添加一个规则的信息
	/// @param[in]	规则信息
	void PushBack(const COMPILEDINFO &sidDfaIds);

	/// @brief		获取最后一个规则信息。
	/// @return		最后一个规则信息。
	COMPILEDINFO& Back();
private:
	/// @brief		私有成员，仅供内部使用。
	COMPILEDINFOARY *m_ruleResult;
};

/*!
@}
*/
