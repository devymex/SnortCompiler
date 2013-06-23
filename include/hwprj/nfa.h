/*!
* @file		nfa.h
**
* @author		Lab 435, Xidian University
**
* @brief			Declaration of the CNfa class		
**
* Functions declaration of nfa's essential algorithm
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\nfarow.h>

#ifndef NFAHDR_DS
class NFAROWARY;
#endif

/*!
* @brief		实现NFA的数据结构。
* NFA结构为矩阵，矩阵行表示NFA状态，矩阵列表示跳转字符，\
* 矩阵中每一个元素代表经所在列指定的字符跳转到的下一组状态集合。
*/
class NFAHDR CNfa
{
public:
	CNfa();
	CNfa(const CNfa &other);
	virtual ~CNfa();

	CNfa& operator = (const CNfa &other);
	CNfaRow& operator [] (ulong ulIdx);
	const CNfaRow& operator [] (ulong ulIdx) const;

	ulong Size() const;
	CNfaRow& Back();
	void Clear();
	void Resize(ulong ulSize);
	void Reserve(ulong ulSize);
	void Shrink();
	void PushBack(const CNfaRow &row);
	void PopBack();

	/// @brief		将NFA每一个状态中的元素排序。
	///	
	void SortAll();

	/// @brief		将一个NFA输出到文件中。
	/// @param	pFile 输出文件路径。
	void Dump(const char *pFile) const;

private:
	/// @brief		私有成员，仅供内部使用
	NFAROWARY*		m_pNfa;
};
