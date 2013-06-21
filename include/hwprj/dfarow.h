/*
**	@file		dfarow.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CDfaRow class
**
**	Store a row for CDfaRow. Array of std::vector<ulong>. Each element of the
**	array stand by a column that indicates the jumping character.
**	Each number in vector is the next state would be jump to by the character.
**
*/

#pragma once

#include <hwprj\common.h>


#ifndef DFAHDR_DS
class STATEVEC;
#define DFAHDR __declspec(dllimport)
#else
#define DFAHDR __declspec(dllexport)
#endif

class DFAHDR CDfaRow
{
public:
	//��Ǹ�״̬/�е����ԣ�NORMAL��START��FINAL
	typedef ulong STATEFLAG;
	static const STATEFLAG NORMAL	= 0x0001;
	static const STATEFLAG START	= 0x0002;
	static const STATEFLAG FINAL	= 0x0004;

	explicit CDfaRow(ulong ulColCnt);
	CDfaRow(const CDfaRow &other);
	virtual ~CDfaRow();

	CDfaRow&		operator =	(const CDfaRow &other);
	const STATEID&	operator []	(byte nIdx) const;
	STATEID&		operator []	(byte nIdx);

	ulong			Size() const;
	void			Fill(STATEID nState);

	/*	��������AddFlags
	**	������nFlags-����ȡֵ
	**	���ܣ�����״̬����
	*/
	void			AddFlags(STATEFLAG nFlags);
	/*	��������SetFlags
	**	������nFlags-����ȡֵ
	**	���ܣ�����״̬����
	*/
	void			SetFlags(STATEFLAG nFlags);
	/*	��������GetFlags
	**	���ܣ���ȡ״̬����
	*/
	STATEFLAG		GetFlags() const;

protected:
	STATEVEC*		m_pDest;
	STATEFLAG		m_nFlags;
};

