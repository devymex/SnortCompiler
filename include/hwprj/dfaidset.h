/*
**	@file		dfaidset.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CDfaIdSet class
**
**	Store the corresponding ID for each CDfa
**
*/

#pragma once

#include <hwprj\unsary.h>

#ifndef DFAIDSETHDR_DS
class DFAIDSET;
#define DFAIDSETHDR __declspec(dllimport)
#else
#define DFAIDSETHDR __declspec(dllexport)
#endif

class DFAIDSETHDR CDfaIdSet
{
public:
	CDfaIdSet();
	CDfaIdSet(const CDfaIdSet &other);
	virtual ~CDfaIdSet();

	CDfaIdSet&	operator =	(const CDfaIdSet &other);
	bool		operator == (const CDfaIdSet &other) const;
	bool		operator <	(const CDfaIdSet &other) const;

	ulong		Size() const;
	void		Clear();
	/*	��������Append
	**	��������һ��DfaIdSet
	**	���ܣ�����һ��DfaIdSet���ӵ���ǰ��DfaIdSet
	*/
	void		Append(const CDfaIdSet &other);
	/*	��������AddDfaId
	**	������dfaId-DFA���
	**	���ܣ����DFA ID
	*/
	void		AddDfaId(DFAID dfaId);
	/*	��������CopyTo
	**	������idAry-DfaIdSet������
	**	���ܣ���DfaIdSet�е���ֵ���Ƹ�DfaIdSet������
	*/
	void		CopyTo(CUnsignedArray &idAry) const;

private:
	DFAIDSET*	m_pSet;
};
