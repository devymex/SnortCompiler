/*
**	@file		nfa.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CNfa class		
**
**	Functions declaration of nfa's essential algorithm
**
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\nfarow.h>

#ifndef NFAHDR_DS
class NFAROWARY;
#endif

/* Store one nfa in table format. Each row of the table is a CNfaRow */
class NFAHDR CNfa
{
public:
	CNfa();
	CNfa(const CNfa &other);
	virtual ~CNfa();

	CNfa&			operator =	(const CNfa &other);
	CNfaRow&		operator []	(ulong ulIdx);
	const CNfaRow&	operator []	(ulong ulIdx) const;

	ulong			Size() const;
	CNfaRow&		Back();
	void			Clear();
	void			Resize(ulong ulSize);
	void			Reserve(ulong ulSize);
	void			Shrink();
	void			PushBack(const CNfaRow &row);
	void			PopBack();

	/*	函数名：SortAll
	**	功能：NFA结构为矩阵，矩阵行表示NFA状态，矩阵列表示跳转字符
	**	将NFA每一个状态中的元素排序
	*/
	void			SortAll();

	/*	函数名：Dump
	**	参数：pFile-输出文件路径
	**	功能：将一个NFA输出到文件中
	*/
	void			Dump(const char *pFile) const;

protected:
	NFAROWARY*		m_pNfa;
};
