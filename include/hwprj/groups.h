/*!
* @file			groups.h
* @author		Lab 435, Xidian University
* @brief		������CGroups��
*/

#pragma once

#include <hwprj\unsary.h>

#ifndef GROUPSHDR_DS
#define GROUPSHDR __declspec(dllimport)
#else
#define GROUPSHDR __declspec(dllexport)
#endif

#ifndef GROUPSHDR_DS
class GROUPVEC;
#endif

/*!
* @brief		��Ϊһ���DFA��Ϣ��
* @remark		���������ЩDFA����ЩDFA�Ĺ���Signature����ǰ�����ĸ�Signature����÷��顢�ϲ����DFA��
*/

struct ONEGROUP
{
	CUnsignedArray	DfaIds;
	CUnsignedArray	ComSigs;
	SIGNATURE		currSig;
	ulong			mergeDfaId;
};

/*!
* @brief		�洢DFA�����з��������
* @remark		������ЩDFA��Ϊһ�顢��ЩDFA�Ĺ���Signature����ǰ�����ĸ�Signature����÷��顢�ϲ����DFA��
*/

class GROUPSHDR CGroups
{
public:
	/// @brief		���캯����
	CGroups();

	/// @brief		�������캯����
	CGroups(const CGroups& other);

	/// @brief		����������
	virtual ~CGroups();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ��CGroups����
	/// @return		���������������á�
	CGroups& operator=(const CGroups &other);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�ֵ�����á�
	/// @param[in]	nIdx ָ��λ�õ��±�
	/// @return		����ָ��λ��Ԫ�ص����á�
	ONEGROUP& operator[](ulong nIdx);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�ֵ�����á�
	/// @param[in]	nIdx ָ��λ�õ��±�
	/// @return		����ָ��λ��Ԫ�ص����ã���������
	const ONEGROUP& operator[](ulong nIdx) const;

	/// @brief		�õ�CGroups���鵱ǰ�Ĵ�С��
	/// @return		CGroups���鵱ǰ�Ĵ�С��
	ulong Size() const;

	/// @brief		����CGroups����Ĵ�С��
	/// @param[in]	uSize ���ĵĳߴ硣
	/// @remark		���ulSize���ڻ���ڵ�ǰ����ߴ磬�򲻻��������Ԫ�أ�
	///				�����ɾ��������ֵ�Ԫ�ء�
	void Resize(ulong nSize);

	/// @brief		��ָ����һ�����������ΪCGroups��������һ��Ԫ�ء�
	/// @param		oneGroup ָ����һ����������
	void PushBack(ONEGROUP oneGroup);

	/// @brief		�õ�CGroups�������һ��Ԫ�ص����á�
	/// @return		���һ��Ԫ�ص����á�
	ONEGROUP& Back();

	/// @brief		���CGroups���������Ԫ�ء�
	void Clear();

	/// @brief		ɾ��CGroups�������±���������Ӧ��Ԫ�ء�
	/// @param[in]	nIdx �����±�������
	void Erase(ulong nIdx);

protected:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	GROUPVEC* m_pGroups;
};

/*!
@}
*/
