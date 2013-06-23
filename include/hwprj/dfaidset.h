/*!
* @file			dfaidset.h
* @author		Lab 435, Xidian University
* @brief		������CDfaIdSet�࣬���Դ洢ÿһ��DFA��Ӧ��ID
*/

#pragma once

#include <hwprj\unsary.h>

#ifndef DFAIDSETHDR_DS
class DFAIDSET;
#define DFAIDSETHDR __declspec(dllimport)
#else
#define DFAIDSETHDR __declspec(dllexport)
#endif

/*!
* @brief		ʵ��DFAID���ݽṹ������㷨
* @remark		�洢ÿһ��CDfa��Ӧ��ID��ʵ�ֵ��㷨������
*				���ID������ID���ϵȡ�
*/
class DFAIDSETHDR CDfaIdSet
{
public:
	/// @brief		���캯����
	CDfaIdSet();

	/// @brief		�������캯����
	/// @param[in]	other ��һ��CDfaIdSet����
	CDfaIdSet(const CDfaIdSet &other);

	/// @brief		����������
	virtual ~CDfaIdSet();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	CDfaIdSet& operator =	(const CDfaIdSet &other);

	/// @brief		���ص� '==' ��������ж���������Ƿ�����ָ���Ķ�����ͬ��
	/// @param[in]	other ��һ��CDfaIdSet����
	/// @return		����CDfaIdSet���ʱ����true�����򷵻�false��
	/// @remark		����ִ����Ԫ�صıȽϡ�
	bool		operator == (const CDfaIdSet &other) const;

	/// @brief		���ص� '<' ��������ж���������Ƿ����ָ���Ķ���С��
	/// @param[in]	other ��һ��CDfaIdSet����
	/// @return		��ǰCDfaIdSet����һ��CDfaIdSetСʱ����true�����򷵻�false��
	/// @remark		����ִ����Ԫ�صıȽϡ������߳��̲�ͬ���ұȽ����϶������ĩβ֮��
	///				�ϳ�������Ϊ��
	bool		operator <	(const CDfaIdSet &other) const;

	/// @brief		�õ�CDfaIdSet��ǰ�Ĵ�С��
	/// @return		CDfaIdSet��ǰ�Ĵ�С����������
	ulong		Size() const;

	/// @brief		���CDfaIdSet
	void Clear();

	/// @brief		����һ��DfaIdSet���ӵ���ǰ��DfaIdSet
	/// @param[in]	other ��һ��DfaIdSet
	void Append(const CDfaIdSet &other);

	/// @brief		���DFA ID
	/// @param[in]	dfaId DFA���
	void AddDfaId(DFAID dfaId);

	/// @brief		��DfaIdSet�е���ֵ���Ƹ�DfaIdSet������
	/// @param[out]		idAry DfaIdSet������
	void CopyTo(CUnsignedArray &idAry) const;

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	DFAIDSET*	m_pSet;
};
