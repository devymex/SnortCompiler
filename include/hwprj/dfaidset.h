/*!
* @file			dfaidset.h
* @author		Lab 435, Xidian University
* @brief		������CDfaIdSet�࣬���Դ洢ÿһ��DFA��Ӧ��ID
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
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
* @addtogroup groupAutomaton
* @{
*/

/*!
* @brief		ʵ��CDfaIdSet���ݽṹ������㷨��
* @remark		�洢ÿһ��CDfa��Ӧ��ID��ʵ�ֵ��㷨������
*				���ID������ID���ϵȡ�
*/
class DFAIDSETHDR CDfaIdSet
{
public:
	/// @brief		���캯����
	CDfaIdSet();

	/// @brief		�������캯����
	CDfaIdSet(const CDfaIdSet &other);

	/// @brief		����������
	virtual ~CDfaIdSet();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ��CDfaIdSet����
	/// @return		���������������á�
	CDfaIdSet& operator =	(const CDfaIdSet &other);

	/// @brief		���ص� '==' ��������ж���������Ƿ�����ָ���Ķ�����ͬ��
	/// @param[in]	other ��һ��CDfaIdSet����
	/// @retval		true ����CDfaIdSet��ȡ�
	/// @retval		false ����CDfaIdSet����ȡ�
	/// @remark		����ִ����Ԫ�صıȽϡ�
	bool operator == (const CDfaIdSet &other) const;

	/// @brief		���ص� '<' ��������ж���������Ƿ����ָ���Ķ���С��
	/// @param[in]	other ��һ��CDfaIdSet����
	/// @return		��ǰCDfaIdSet����һ��CDfaIdSetСʱ����true�����򷵻�false��
	/// @remark		����ִ����Ԫ�صıȽϡ������߳��̲�ͬ���ұȽ����϶������ĩβ֮��
	///				�ϳ�������Ϊ��
	bool operator <	(const CDfaIdSet &other) const;

	/// @brief		�õ�CDfaIdSet��ǰ�Ĵ�С��
	/// @return		CDfaIdSet��ǰ�Ĵ�С����������
	ulong Size() const;

	/// @brief		���CDfaIdSet��
	void Clear();

	/// @brief		����һ��DfaIdSet���ӵ���ǰ��DfaIdSet֮��
	/// @param[in]	other ��һ��DfaIdSet��
	void Append(const CDfaIdSet &other);

	/// @brief		���DFA ID��
	/// @param[in]	dfaId DFA��š�
	void AddDfaId(DFAID dfaId);

	/// @brief		��DfaIdSet�е�value���Ƹ�CUnsignedArray���͵����顣
	/// @param[out]	idAry CUnsignedArray���͵����顣
	void CopyTo(CUnsignedArray &idAry) const;

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	DFAIDSET* m_pSet;
};

/*!
@}
*/
