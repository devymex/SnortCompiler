/*!
* @file			ctimer.h
* @author		Devymex, Xidian University
* @brief		�����˸߾��ȼ�ʱ����CTimer
* @warning		���ļ�������windows.h��������������ͻ�����ڰ��˱��ļ�֮��������غ궨�塣
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once
#include "windows.h"

/*!
* @addtogroup groupCommon
* @{
*/

/*!
* @brief		С�ɷ���ļ�ʱ���ࡣ
* @remark		��Ա������Ϊ���������������ڵ��ÿ�������ʱ���ȸߡ�
*/
class CTimer
{
public:
	/// @brief		���캯����
	/// @remark		�����������ʱ��
	inline CTimer()
	{
		QueryPerformanceFrequency((PLARGE_INTEGER)&m_nFreq);
		QueryPerformanceCounter((PLARGE_INTEGER)&m_nStart);
	}

	/// @brief		��ôӼ�ʱ��ʼ�����ھ�����ʱ�䡣
	/// @return		��λΪ�롣
	inline double Cur()
	{
		__int64 nCur;
		QueryPerformanceCounter((PLARGE_INTEGER)&nCur);
		return double(nCur - m_nStart) / double(m_nFreq);
	}

	/// @brief		������ʱ��������ôӼ�ʱ��ʼ�����ھ�����ʱ�䡣
	/// @return		��λΪ�롣
	inline double Reset()
	{
		__int64 nCur;
		double dCur;

		QueryPerformanceCounter((PLARGE_INTEGER)&nCur);
		dCur = double(nCur - m_nStart) / double(m_nFreq);
		m_nStart = nCur;

		return dCur;
	}
private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	__int64 m_nFreq;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	__int64 m_nStart;
};

/*!
@}
*/
