/*!
* @file			unsary.h
* @author		Lab 435, Xidian University
* @brief		������CUnsignedArray�༰�䵼������
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\trace.h>

#ifndef UNSARYHDR_DS
#define UNSARYHDR __declspec(dllimport)
#else
#define UNSARYHDR __declspec(dllexport)
#endif

/*!
* @brief	ʵ��(ulong*)�Ķ�̬���顣
* @remark	�������std::vector<unsigned long>����ģ��֮�䴫�ݲ�����
*/
class UNSARYHDR CUnsignedArray
{
public:
	/// @brief		���캯����
	inline CUnsignedArray();

	/// @brief		�������캯����
	inline CUnsignedArray(const CUnsignedArray &other);

	/// @brief		����������
	inline virtual ~CUnsignedArray();

	/// @brief		���ص� '=' �������
	/// @param[in]	other ��һ����̬�������
	/// @return		���������������á�
	inline CUnsignedArray& operator = (const CUnsignedArray &other);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�ֵ�����á�
	/// @param[in]	nIdx ָ��λ�õ��±�
	/// @return		����ָ��λ��Ԫ�ص����á�
	inline ulong& operator [] (ulong nIdx);

	/// @brief		���ص� '[]' �������ȡ��ָ��λ�õ�ֵ�����á�
	/// @param[in]	nIdx ָ��λ�õ��±�
	/// @return		����ָ��λ��Ԫ�ص����ã���������
	inline const ulong& operator [] (ulong nIdx) const;

	/// @brief		���ص� '==' ��������ж���������Ƿ�����ָ���Ķ�����ͬ��
	/// @param[in]	other ��һ����̬�������
	/// @return		������̬�������ʱ����true�����򷵻�false��
	/// @remark		����ִ����Ԫ�صıȽϡ�
	inline bool operator == (const CUnsignedArray &other) const;

	/// @brief		���ص� '<' ��������ж���������Ƿ����ָ���Ķ���С��
	/// @param[in]	other ��һ����̬�������
	/// @return		��ǰ�Ķ�̬�������һ����̬����Сʱ����true�����򷵻�false��
	/// @remark		����ִ����Ԫ�صıȽϡ������߳��̲�ͬ���ұȽ����϶������ĩβ֮��
	///				�ϳ�������Ϊ��
	inline bool operator < (const CUnsignedArray &other) const;

	/// @brief		�õ���̬���鵱ǰ�Ĵ�С��
	/// @return		��̬���鵱ǰ�Ĵ�С��
	inline ulong Size() const;

	/// @brief		�õ���̬�������һ��Ԫ�ص����á�
	/// @return		���һ��Ԫ�ص����á�
	inline ulong& Back();

	/// @brief		������������Ԫ�ء�
	inline void Clear();

	/// @brief		ɾ����������һ��Ԫ�ء�
	inline void PopBack();

	/// @brief		��ָ������ֵ���Ϊ��������һ��Ԫ�ء�
	/// @param		ulval ָ������ֵ��
	inline void PushBack(ulong ulVal);

	/// @brief		Ϊ��̬����Ԥ���ڴ�ռ䡣
	/// @param		ulCap ָ����Ԥ���Ŀռ䡣
	inline void Reserve(ulong ulCap);

	/// @brief		���Ķ�̬����Ĵ�С��
	/// @param[in]	ulSize ���ĵĳߴ硣
	/// @remark		���ulSize���ڻ���ڵ�ǰ����ߴ磬�򲻻��������Ԫ�أ�
	///				�����ɾ��������ֵ�Ԫ�ء�
	inline void Resize(ulong ulSize);

	/// @brief		���Ķ�̬����Ĵ�С��
	/// @param[in]	ulSize ���ĵĳߴ硣
	/// @remark		���ulSize���ڻ���ڵ�ǰ����ߴ磬�򲻻��������Ԫ�ز�Ϊ����Ԫ�ظ�ֵΪulVal��
	///				�����ɾ��������ֵ�Ԫ�ء�
	inline void Resize(ulong ulSize, ulong ulVal);

	/// @brief		�������������ָ��
	/// @return		���������ָ�룬ָ���һ��Ԫ��
	inline ulong* Data();

	/// @brief		�������������ָ��
	/// @return		���������ָ�루��������ָ���һ��Ԫ��
	inline const ulong* Data() const;

	/// @brief		�������е�����Ԫ�ظ�ֵΪָ����ֵ
	/// @param[in]	ulVal ָ������ֵ��
	inline void Fill(ulong ulVal);

	/// @brief		�������в���ָ������ֵ��
	/// @param[in]	ulVal �����ҵ���ֵ��
	/// @return		���ҵ����򷵻��ҵ���Ԫ�ص��±꣬���򷵻�ulong(-1)��
	inline ulong Find(ulong ulVal) const;

	/// @brief		���������е�����Ԫ�أ�����std::sort
	void Sort();

	/// @brief		ʹ�����е�����Ԫ��Ψһ��������std::unique
	void Unique();

protected:
	/// @brief		�ַ�ָ����С���ڴ棬�����滻new��
	inline ulong* Alloc(ulong ulCap) const;

	/// @brief		�ͷ�ָ�����ڴ棬�����滻delete[]��
	inline void Free(ulong *pBuf) const;

	/// @brief		���·���ָ����С��
	inline void Realloc(ulong ulCap);

	/// @brief		��ָ�����ڴ��п������ݡ�
	void CopyFrom(ulong *pBuf);

private:
	/// @brief		˽�г�Ա�������ڲ����á�
	ulong* m_pAry;

	/// @brief		˽�г�Ա�������ڲ����á�
	ulong m_ulSize;

	/// @brief		˽�г�Ա�������ڲ����á�
	size_t m_ulCap;
};

#include <hwprj\unsary.inl>
