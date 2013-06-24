/*!
* @file			nfa.h
* @author		Lab 435, Xidian University
* @brief		定义了CNfa类，用以表示NFA的数据结构
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\nfarow.h>

#ifndef NFAHDR_DS
class NFAROWARY;
#endif

/*!
* @addtogroup groupAutomaton
* @{
*/

/*!
* @brief		实现NFA的数据结构。
* @remark		封装了CNfaRow的动态数组。NFA结构为矩阵，矩阵行表示NFA状态，矩阵列表示跳转字符，
*				矩阵中每一个元素代表经所在列指定的字符跳转到的下一组状态集合。
*/
class NFAHDR CNfa
{
public:
	/// @brief		构造函数。
	CNfa();

	/// @brief		拷贝构造函数。
	CNfa(const CNfa &other);

	/// @brief		析构函数。
	virtual ~CNfa();

	/// @brief		重载的 '=' 运算符。
	/// @param[in]	other 另一个NFA对象
	/// @return		返回自身对象的引用。
	CNfa& operator = (const CNfa &other);

	/// @brief		重载的 '[]' 运算符，取得指定位置的CNfaRow对象的引用。
	/// @param[in]	ulIdx 指定位置的下标
	/// @return		返回指定位置的CNfaRow对象的引用。
	CNfaRow& operator [] (ulong ulIdx);

	/// @brief		重载的 '[]' 运算符，取得指定位置的CNfaRow对象的引用。
	/// @param[in]	ulIdx 指定位置的下标
	/// @return		返回指定位置的CNfaRow对象的引用。
	const CNfaRow& operator [] (ulong ulIdx) const;

	/// @brief		得到NFA当前的大小。
	/// @return		NFA当前的大小（常量）。
	ulong Size() const;

	/// @brief		得到CNfa最后一个位置的CNfaRow对象的引用。
	/// @return		最后一个位置的CNfaRow对象的引用。
	CNfaRow& Back();

	/// @brief		清空NFA。
	void Clear();

	/// @brief		更改NFA的大小。
	/// @param[in]	ulSize 更改的尺寸
	/// @remark		如果ulSize大于或等于当前尺寸，则不会更改现有元素；
	///				否则会删除多出部分的元素。
	void Resize(ulong ulSize);

	/// @brief		为NFA预留内存空间。
	/// @param		ulSize 指定的预留的空间
	void Reserve(ulong ulSize);

	/// @brief		释放CNfa中多余的空间。
	void Shrink();

	/// @brief		将指定的NFA状态添加为NFA跳转表最后一个元素。
	/// @param[in]	row 指定的NFA状态
	void PushBack(const CNfaRow &row);

	/// @brief		删除CNfa的最后一个状态。
	void PopBack();

	/// @brief		将NFA每一个状态中的元素排序。
	void SortAll();

	/// @brief		将一个NFA输出到文件中。
	/// @param[in]	pFile 输出文件路径（常量）。
	void Dump(const char *pFile) const;

private:
	/// @brief		私有成员，仅供内部使用
	NFAROWARY*		m_pNfa;
};

/*!
@}
*/