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
* @brief		�洢һ������Ĵ�����Ϣ��
* @remark		����������������id�Լ�DFA��
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
* @brief		�洢���й���Ĵ�����Ϣ��
*/

class SIDDFAHDR CSidDfaIds
{
public:
	/// @brief		���캯����
	CSidDfaIds();

	/// @brief		�������캯����
	CSidDfaIds(const CSidDfaIds& other);

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ��CSidDfaIds����
	/// @return		���������������á�
	CSidDfaIds &operator=(const CSidDfaIds &other);

	/// @brief		����������
	virtual ~CSidDfaIds();

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�ֵ�����á�
	/// @param[in]	nIdx ָ��λ�õ��±�
	/// @return		����ָ��λ��Ԫ�ص����á�
	COMPILEDINFO& operator[](ulong index);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�ֵ�����ã���������
	/// @param[in]	nIdx ָ��λ�õ��±�
	/// @return		����ָ��λ��Ԫ�ص����ã���������
	const COMPILEDINFO& operator[](ulong index) const;

	/// @brief		����vectorԤ���ռ�Ĵ�С��ִ��std::vector::reserve������
	void Reserve(ulong nCount);

	/// @brief		����vector�Ĵ�С��ִ��std::vector::resize������
	void Resize(ulong nSize);

	/// @brief		����vector�Ĵ�С��
	/// @return		������Ϣ��������
	const ulong Size() const;

	/// @brief		���һ���������Ϣ
	/// @param[in]	������Ϣ
	void PushBack(const COMPILEDINFO &sidDfaIds);

	/// @brief		��ȡ���һ��������Ϣ��
	/// @return		���һ��������Ϣ��
	COMPILEDINFO& Back();
private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	COMPILEDINFOARY *m_ruleResult;
};

/*!
@}
*/
